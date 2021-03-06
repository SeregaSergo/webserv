#include "../inc/Response.hpp"

Response::Response(Request * req, Client * client)
        : _request(req)
        , _client(client)
        , _sent(0)
        , _status_code(0)
        , _handle_err(true)
        , _virt_serv(NULL)
        , _location(NULL)
        , _in_CGI(NULL)
        , _out_CGI(NULL)
        , _pid(0)
{}

Response::Response(Response const & src)
    : _request(src._request)
    , _client(src._client)
    , _response(src._response)
    , _sent(src._sent)
    , _status_code(src._status_code)
    , _set_cookies(src._set_cookies)
    , _headers(src._headers)
    , _handle_err(src._handle_err)
    , _virt_serv(src._virt_serv)
    , _location(src._location)
    , _resulting_uri(src._resulting_uri)
    , _file(src._file)
    , _in_CGI(src._in_CGI)
    , _out_CGI(src._out_CGI)
    , _pid(src._pid)
{
    _body << src._body.rdbuf();
}

Response::~Response(void)
{
    if (_in_CGI)
        removeInCGI();
    if (_out_CGI)
        removeOutCGI();
    if (_pid)
        finishCGI();
}


/////////////////////////////////////
//        CGI implementation       //
/////////////////////////////////////

bool Response::runCGI(void)
{
    int cgi_out_pipe[2];
    int cgi_in_pipe[2];

    pipe(cgi_out_pipe);
    pipe(cgi_in_pipe);

    _pid = fork();
    if (_pid < 0)
    {
        _status_code = 500;
        return (false);
    }
    if (_pid == 0)
    { 
        dup2(cgi_in_pipe[0], 0);
        dup2(cgi_out_pipe[1], 1);

        close(cgi_out_pipe[0]);
        close(cgi_in_pipe[1]);

        freopen("/dev/null", "w", stderr);  // mute stderr

        std::vector<char*>  argv;
        std::vector<char*>  envp;

        if (execve(&_file[0], getArgv(argv), getEnvp(envp)) == -1)
            exit(502);
    }
    else
    {
        _in_CGI = new InCGI(cgi_in_pipe[1], _location->getCgiTimout(), _request->_body, *this, &Response::callbackFuncInCGI);
        _out_CGI = new OutCGI(cgi_out_pipe[0], _location->getCgiTimout(), _body, *this, &Response::callbackFuncOutCGI);

        _request->_server->addHandler(_in_CGI);
        _request->_server->addHandler(_out_CGI);

        close(cgi_out_pipe[1]);
        close(cgi_in_pipe[0]);
    }
    DISPLAY(std::cout << "[pid " << _pid << "] CGI was run" << std::endl);
    return (true);
}

void Response::finishCGI(void)
{
    int status;
    pid_t ret= waitpid(_pid, &status, WNOHANG);
    if (ret == 0)
    {
        kill(_pid, SIGKILL);
        ret = waitpid(_pid, &status, 0);
        _status_code = 502;
    }
    DISPLAY(std::cout << "[pid " << _pid << "] was stopped" << std::endl);
    _pid = 0;
}

/////////////////////////////////////
//        Autoindex function       //
/////////////////////////////////////

void Response::processAI()
{
    DIR             *dir;
    struct stat     t_stat;
    std::string     creation_time;
    off_t           file_size;
    struct dirent   *ent;
    
    if ((dir = opendir (_file.c_str())) != NULL)
    {
        _body   << "<!DOCTYPE html><html lang=\"en\"><html>\n<head>\n<meta charset=\"UTF-8\"><title>"
            << _request->_headers["Host"] << "</title>\n<style> * {box-sizing: border-box;}"
            << "a {display: inline-block;position:relative;padding: 0 2em; margin: 0 -2em;}"
            << ".column {float: left; padding: 10px; height: 20px;}.left {width: 50%;},"
            << ".right {width: 25%;}.middle {width: 25%;}"
            << ".row:after {content: "";display: table;clear: both;}"
            << "</style></head>\n<body>\n" << "<h2>Autoindex: " << _resulting_uri << "</h2>\n"
            << "<div class=\"row\"><div class=\"column left\"><p><a href=\"" << _resulting_uri
            << "..\">..</a></p></div> <div class=\"column middle\"><p></p></div>"
            << "<div  class=\"column right\"><p>-</p></div></div>";
            
        while ((ent = readdir (dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                continue;
            file_size = -1;
            creation_time.clear();
            stat((_file + ent->d_name).c_str(), &t_stat);
            _body << "<div class=\"row\">";
            _body  <<  "<div class=\"column left\"><p><a href=\"" << _resulting_uri << ent->d_name;
            if (ent->d_type == DT_DIR)
                _body << '/';
            else 
                file_size = t_stat.st_size;
            if (ent->d_name[0] != '.')
                creation_time = asctime(localtime(&t_stat.st_ctime));
            
            _body <<  "\">" <<  ent->d_name << "</a>" << "</p></div>"
            << "<div class=\"column middle\"><p>" << creation_time << "</p></div>"
            << "<div  class=\"column right\"><p>" << (file_size == -1 ? "-" : numToStr(file_size)) << "</p></div>"
            << "</div>";
        }

        _body << "</body>\n</html>\n";
        closedir (dir);
    } 
    else
        _status_code = 404;
}

int Response::processResponseCGI(void)
{
    if (_status_code != 200)
        return (response::CGI::document);
    for (std::string line; std::getline(_body, line); )
    {
        if (parseHeaderLine(line))
        {
            std::map<std::string,std::string>::iterator it;
            if ((it = _headers.find("Location")) != _headers.end() && \
                it->second[0] == '/')           // Server redirection, see RFC 3875 6.2.2 
            {
                if (it->second == _resulting_uri || it->second == _request->_uri)
                    break;
                _request->_uri = it->second;
                if ((_request->_location = _virt_serv->chooseLocation(it->second)) == NULL)
                {
                    _status_code = 404;
                    return (response::CGI::document);
                }
                return (response::CGI::server_redir);
            }
            if ((it = _headers.find("Status")) != _headers.end())
            {
                _status_code = atoi(it->second.c_str());
                _headers.erase(it);
                _handle_err = false;
            }
            return (response::CGI::document);
        }
    }
    _body.clear();
    _status_code = 502;
    return (response::CGI::document);
}

// Parses header line in CGI response.
// Returns: true if run out of headers;
//          false in oposite
bool Response::parseHeaderLine(std::string & line)
{
    if (line.empty() || line == "\r")
        return (true);
    size_t pos_delim = line.find(':');
    if (*line.rbegin() == '\r')
        line.erase(line.size() - 1, 1);
    if (pos_delim != std::string::npos)
    {
        size_t  pos_val = line.find_first_not_of(" ", pos_delim + 1);
        _headers[line.substr(0, pos_delim)] = line.substr(pos_val);
    }
    return (false);
}

void Response::removeInCGI(void)
{
    _request->_server->removeHandler(_in_CGI);
    delete _in_CGI;
    _in_CGI = NULL;
}

void Response::removeOutCGI(void)
{
    _request->_server->removeHandler(_out_CGI);
    delete _out_CGI;
    _out_CGI = NULL;
}

void Response::callbackFuncInCGI(int ret)
{
    if (ret)
        _status_code = ret;
    removeInCGI();
}

void Response::callbackFuncOutCGI(int ret)
{
    if (ret)
        _status_code = ret;
    removeOutCGI();
    finishCGI();
    if (processResponseCGI() == response::CGI::server_redir)
    {
        clear();
        processRequest();
        return;
    }
    else
    {
        assembleResponse();
        _client->setState(client::State::writing);
    }
}

char * const * Response::getArgv(std::vector<char*> & argv)
{
    argv.push_back(&_file[0]);
    argv.push_back(NULL);
    return (&argv[0]);
}

void	Response::put_env_into_vec(std::vector<char *> &envp, std::string new_env) {
	envp.push_back(strdup(new_env.c_str()));
}

int convertHeader(int c)
{
    if (c == '-')
        return ('_');
    return (::toupper(c));
}

char * const * Response::getEnvp(std::vector<char*> & envp)
{
	std::string str;
	std::map<std::string, std::string>::iterator it;

	put_env_into_vec(envp, "AUTH_TYPE=NULL");
    put_env_into_vec(envp, "REQUEST_METHOD=" + _request->_method);
    put_env_into_vec(envp, "QUERY_STRING=" + _request->_query);
    put_env_into_vec(envp, "PATH_INFO=" + _file);
    put_env_into_vec(envp, "PATH_TRANSLATED=" + _file);
    put_env_into_vec(envp, "DOCUMENT_ROOT=" + _virt_serv->getDocRoot());
    put_env_into_vec(envp, "SCRIPT_NAME=" + _request->_uri);
	put_env_into_vec(envp, "CONTENT_LENGTH=" + numToStr(_request->_body.size()));
	put_env_into_vec(envp, "GATEWAY_INTERFACE=CGI/1.1");
	put_env_into_vec(envp, "REMOTE_ADDR=" + _client->getHost());
    put_env_into_vec(envp, "REMOTE_HOST=NULL");
	put_env_into_vec(envp, "REMOTE_PORT=" + numToStr(_client->getPort()));
    put_env_into_vec(envp, "SERVER_NAME=" + _request->_headers["Host"]);
    put_env_into_vec(envp, "SERVER_PORT=" + numToStr(_request->_server->getPort()));
    put_env_into_vec(envp, "SERVER_PROTOCOL=HTTP/1.1");
    put_env_into_vec(envp, "SERVER_SOFTWARE=webserv_5000");

    if ((it = (_request->_headers).find("Content-Type")) != _request->_headers.end())
    {
        put_env_into_vec(envp, "CONTENT_TYPE=" + it->second);
        _request->_headers.erase("Content-Type");
    }
    _request->_headers.erase("Host");

    for (it = _request->_headers.begin(); it !=  _request->_headers.end(); ++it)
	{
		str = it->first;
		std::transform(it->first.begin(), it->first.end(), str.begin(), convertHeader);
		put_env_into_vec(envp, "HTTP_" + str + "=" + it->second);
	}

	envp.push_back(NULL);

    return (&envp[0]);
}


/////////////////////////////////////
//      Redirection function       //
/////////////////////////////////////

void Response::processRedirection(void)
{
    Redirect * redir = _location->getRedir();
    _status_code = redir->get_code();
    _headers["Location"] = redir->rewrite_url(_resulting_uri);
}


/////////////////////////////////////
// Methods implementation function //
/////////////////////////////////////

int Response::processMethod(void)
{   
    _status_code = _request->_status_code;
    _virt_serv = _request->_virt_serv;
    if (!_virt_serv)
        return (processing::Type::error);

    _virt_serv->init_session(_request->_headers, _set_cookies);
    _resulting_uri = _request->_uri;
    _location = _request->_location;
    if (!_location)
        return (processing::Type::error);

    _file = _location->getResoursePath(_resulting_uri);
    int location_type = _location->getType();
    
    if (_status_code != 200)
        return (processing::Type::error);
    if (location_type == location::Type::redirection)
        return (processing::Type::redirection);
    if (location_type == location::Type::cgi)
        return (processing::Type::cgi);

    return constants::method[_request->_method]->process(*this);
}


/////////////////////////////////////
//  Assembling response functions  //
/////////////////////////////////////

void Response::handleError(void)
{
    if (!_virt_serv)
        return;

    clear();
    _resulting_uri = _virt_serv->getPage(_status_code);
    if (_resulting_uri.empty())
        return;

    if (!(_location = _virt_serv->chooseLocation(_resulting_uri)))
        return;

    int saved_code = _status_code;
    _file = _location->getResoursePath(_resulting_uri);
    if (constants::method["GET"]->process(*this) != processing::Type::done)
        _status_code = saved_code;
}

void Response::assembleResponse(void)
{   
    if (_status_code >= 400 && _handle_err)
        handleError();
    _response.append(_request->getHttpVersion());
    _response.push_back(' ');
    _response.append(numToStr(_status_code));
    _response.push_back(' ');
    _response.append(constants::codes_description.find(_status_code)->second);
    _headers["Content-Length"] = numToStr(_body.tellp() - _body.tellg());
    if (_request->isLastRequest())
        _headers["Connection"] = "close";
    for (std::map<std::string,std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        _response.append("\r\n");
        _response.append((*it).first);
        _response.append(": ");
        _response.append((*it).second);
    }
    for (std::vector<std::string>::iterator it = _set_cookies.begin(); it != _set_cookies.end(); ++it)
    {
        _response.append("\r\nSet-Cookie: ");
        _response.append(*it);
    }
    _response.append("\r\n\r\n");
    _response.append(std::istreambuf_iterator<char>(_body), std::istreambuf_iterator<char>());
}


/////////////////////////////////////
//     Public member functions     //
/////////////////////////////////////

void Response::processRequest()
{
    switch (processMethod())
    {

    case processing::Type::redirection:
        processRedirection();
        break;
    
    case processing::Type::cgi:
        if (!runCGI())
            break;
        return;
    
    case processing::Type::autoindex:
        processAI();
        break;
    
    }

    assembleResponse();
    _client->setState(client::State::writing);
}

int Response::sendResponse(int fd)
{
    int ret = send(fd, &_response[_sent], _response.size(), 0);

    if (ret < 0)
        return (response::ReturnCode::error);

    _sent += ret;
    if (_sent == _response.size())
        return (response::ReturnCode::completed);
    else
        return (response::ReturnCode::unfinished);
}

void Response::clear(void)
{
    _response.clear();
    _set_cookies.clear();
    _headers.clear();
    _body.str(std::string());
    _sent = 0;
    _handle_err = true;
}

int Response::getStatusCode(void) {
    return (_status_code);
}

unsigned long Response::getBytesSent(void) {
    return (_sent);
}

std::ostream & operator<<(std::ostream & o, Response const & resp)
{
    o << "\n***** Response *****\n" << std::endl;
    DEBUG(o << "Location:\n" << *resp._location << std::endl);
    DEBUG(o << "Resulting_URI: " << resp._resulting_uri << std::endl);
    DEBUG(o << "File: " << resp._file << std::endl);
	DEBUG(o << (resp._set_cookies.empty() ? "None cookies" : resp._set_cookies[0]) << std::endl);
    DEBUG(o << "Sent: " << resp._sent << std::endl << std::endl);
    o << resp._response.substr(0, 1000) << std::endl;
    return (o);
}
