#include "../inc/Response.hpp"
#include <sstream>

Response::Response(Request * req, int * cl_state)
        : _request(req)
        , _client_state(cl_state)
        , _sent(0)
        , _status_code(0)
        , _virt_serv(NULL)
        , _location(NULL)
        , _in_CGI(NULL)
        , _out_CGI(NULL)
        , _pid(0)
{}

Response::Response(Response const & src)
    : _request(src._request)
    , _client_state(src._client_state)
    , _response(src._response)
    , _sent(src._sent)
    , _status_code(src._status_code)
    , _headers(src._headers)
    , _location(src._location)
    , _resulting_uri(src._resulting_uri)
    , _file(src._file)
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

        std::vector<char*>  argv;
        std::vector<char*>  envp;

        if (execve(_location->getCgiInterpreter(), getArgv(argv), getEnvp(envp)) == -1)
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
    std::cout << "CGI was run" << std::endl;
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
    }
    std::cout << "[pid " << _pid << "] was stopped" << std::endl;
    // if (WIFEXITED(status))
    // {
    //     std::cout << "Script exited with error " << WEXITSTATUS(status) << std::endl;
    //     _status_code = 502;
    // }
    _pid = 0;
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
            if ((it = _headers.find("Status")) != _headers.end())
            {
                _status_code = atoi(it->second.c_str());
                _headers.erase(it);
            }
            if ((it = _headers.find("Location")) != _headers.end() && \
                it->second[0] == '/')           // Server redirection, see RFC 3875 6.2.2 
            {
                if (it->second == _resulting_uri || it->second == _request->_uri)
                    break;
                _request->_uri = it->second;
                if ((_request->_location = _virt_serv->chooseLocation(_request->_uri)) == NULL)
                {
                    _status_code = 404;
                    return (response::CGI::document);
                }
                return (response::CGI::server_redir);
            }
            return (response::CGI::document);
        }
    }
    std::cout << "502 is HERE" << std::endl;
    _status_code = 502;
    return (response::CGI::document);
}

// Parses header line in CGI response.
// Returns: true if run out of headers;
//          false in oposite
bool Response::parseHeaderLine(std::string & line)
{
    std::cout << "Parse line: " << line << std::endl;
    if (line.empty() || line == "\r")
        return (true);
    size_t pos_delim = line.find(':');
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
        *_client_state = client::State::writing;
    }
}

char * const * Response::getArgv(std::vector<char*> & argv)
{
    argv.push_back((char *)_location->getCgiInterpreter());
    argv.push_back(&_file[0]);
    argv.push_back(NULL);
    return (&argv[0]);
}

char * const * Response::getEnvp(std::vector<char*> & envp)
{
    envp.push_back(std::getenv("PATH"));
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
    _virt_serv = _request->_virt_serv;
    _status_code = _request->_status_code;
    _resulting_uri = _request->_uri;
    _location = _request->_location;
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
    std::cout << "Handle error: "<< _status_code << std::endl;
    clear();
    _resulting_uri = _virt_serv->getPage(_status_code);
    if (_resulting_uri.empty())
        return;

    if (!(_location = _virt_serv->chooseLocation(_resulting_uri)))
        return;

    int saved_code = _status_code;
    if (constants::method["GET"]->process(*this) != processing::Type::done)
        _status_code = saved_code;
}

void Response::assembleResponse(void)
{   
    if (_status_code >= 400)
        handleError();
    _response.append(_request->_http_version);
    _response.push_back(' ');
    _response.append(numToStr(_status_code));
    _response.push_back(' ');
    _response.append(constants::codes_description.find(_status_code)->second);
    _headers["Content-Length"] = numToStr(_body.tellp() - _body.tellg());
    for (std::map<std::string,std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        _response.append("\r\n");
        _response.append((*it).first);
        _response.append(": ");
        _response.append((*it).second);
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
        break;
    }

    assembleResponse();
    *_client_state = client::State::writing;
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
    _headers.clear();
    _body.str(std::string());
    _sent = 0;
}

std::ostream & operator<<(std::ostream & o, Response const & resp)
{
    o << "\n***** Response *****\n" << std::endl;
    o << "Location:\n" << *resp._location << std::endl;
    o << "Resulting_URI: " << resp._resulting_uri << std::endl;
    o << "File: " << resp._file << std::endl;
    o << "Sent: " << resp._sent << std::endl << std::endl;
    o << resp._response << std::endl;
    return (o);
}