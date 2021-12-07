#include "../inc/Response.hpp"
#include <sstream>

Response::Response(Response const & src)
    : _request(src._request)
    , _client(src._client)
    , _response(src._response)
    , _headers(src._headers)
    , _status_code(src._status_code)
    , _location(src._location)
    , _resulting_uri(src._resulting_uri)
    , _file(src._file)
    , _content_type(src._file)
    , _sent(src._sent)
{
    _body << src._body.rdbuf();
}

std::map<std::string, int (Response::*)(void)>	Response::initPrepareFunc(void)
{
    std::map<std::string, int (Response::*)(void)> map;
    map["GET"] = &Response::prepareGET;
    map["POST"] = &Response::preparePOST;
    map["DELETE"] = &Response::prepareDELETE;
    return (map);
}

std::map<std::string, int (Response::*)(void)>  Response::_prepare_func = Response::initPrepareFunc();

void Response::handleError(void)
{
    std::string file = _request->_virt_serv->getPage(_request->_status_code);
    if (!file.empty())
        _resulting_uri = file;
}

void Response::processRedirection(void)
{
    Redirect * redir = _request->_location->getRedir();
    _status_code = redir->get_code();
    _headers["Location"] = redir->rewrite_url(_resulting_uri);
}

void Response::assembleResponse(void)
{
    _response.append(_request->_http_version);
    _response.push_back(' ');
    _response.append(numToStr(_status_code));
    _response.push_back(' ');
    _response.append(constants::codes_description.find(_status_code)->second);
    _body.seekg(0, std::ios::end);
    _headers["Content-Length"] = numToStr(_body.tellg());
    _body.seekg(0, std::ios::beg);
    for (std::map<std::string,std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        _response.append("\r\n");
        _response.append((*it).first);
        _response.append(": ");
        _response.append((*it).second);
    }
    _response.append("\r\n\r\n");
    _response.append(_body.str());
}

inline bool Response::isRequestedADirectory(void)
{
    return (_resulting_uri[_resulting_uri.size() - 1] == '/');
}

inline bool Response::isFileExist(std::string const & temp_file)
{
    struct stat buffer;   
    return (stat(&temp_file[0], &buffer) == 0);
}

inline int Response::errorCode(int code)
{
    _status_code = code;
    return (processing::Type::error);
}

int Response::prepareGET(void)
{
    if (isRequestedADirectory())
    {
        std::cout << "is DIRECTORY GET METHOD" << std::endl;
        std::vector<std::string>    index_files = _location->getIndexFiles();
        std::string                 temp_uri;
        Location *                  temp_loc;
        std::string                 temp_file;
        while (!index_files.empty())
        {
            temp_uri = _resulting_uri + index_files.front();
            temp_loc = _request->_virt_serv->chooseLocation(temp_uri);
            temp_file = temp_loc->getResoursePath(temp_uri);
            std::cout << "Temp uri: " << temp_uri << std::endl;
            std::cout << "Temp location:\n" << *temp_loc << std::endl;
            std::cout << "Temp file: " << temp_file << std::endl;
            if (isFileExist(temp_file))
            {
                _location = temp_loc;
                _resulting_uri = temp_uri;
                _file = temp_file;
                return (processing::Type::file);
            }
            index_files.erase(index_files.begin());
        }
        if (index_files.empty() && _location->getAutoindex())
            return (processing::Type::autoindex);
        else
            return (errorCode(404));
    }
    if (_location->getType() == location::Type::cgi)
        return (processing::Type::cgi);
    else
        return (processing::Type::file);
}

int Response::preparePOST(void)
{
    bool isDirectory = isRequestedADirectory();
    size_t pos = 0;

    if (_location->getType() == location::Type::cgi && !isDirectory)
        return (processing::Type::cgi);
    
    std::map<std::string,std::string>::const_iterator it = _request->_headers.find("Content-Type");
    if (it == _request->_headers.end())
        return (errorCode(415));

    
    if ((pos = it->second.find(';')) != std::string::npos)
        _content_type = it->second.substr(0, pos);
    else
        _content_type = it->second;
    if (_content_type == "application/x-www-form-urlencoded")
    {
        if (isDirectory)
            return (errorCode(400));
        if (isFileExist(_file))
            return (errorCode(409));    // Conflict ?
        return (processing::Type::file);
    }
    else if (_content_type == "multipart/form-data")
    {
        if (isDirectory)
            return (processing::Type::file);
        return (errorCode(400));
    }
    else
        return (errorCode(415));    // Unsupported Media Type
}

int Response::prepareDELETE(void)
{
    if (isRequestedADirectory())
        return (errorCode(400));
    if (!isFileExist(_file))
        return (errorCode(404));
    if (_location->getType() == location::Type::cgi)
        return (processing::Type::cgi);
    else
        return (processing::Type::file);
}

int Response::prepareForProcessing(void)
{
    _status_code = _request->_status_code;
    _resulting_uri = _request->_uri;
    _location = _request->_location;
    _file = _location->getResoursePath(_resulting_uri);

    if (_status_code != 200)
        return (processing::Type::error);
    if (_location->getType() == location::Type::redirection)
        return (processing::Type::redirection);
    return (this->*Response::_prepare_func[_request->_method])();
}

size_t Response::split(const std::string &txt, std::vector<std::string> &strs, char c)
{
    size_t pos = txt.find(c);
    size_t initialPos = 0;

    while( pos != std::string::npos ) {
        strs.push_back(txt.substr( initialPos, pos - initialPos) );
        initialPos = pos + 1;

        pos = txt.find(c, initialPos);
    }
    strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

    return strs.size();
}
std::string Response::decodeURLencoded(std::string str)
{
    size_t pos = str.find('%');
    std::stringstream stream;
    int num_symbol;

    while( pos != std::string::npos ) {
        stream << str.substr(pos + 1, 2);
        stream >> std::hex >> num_symbol;
        str.erase(pos, 3);
        str.insert(pos, 1, (char)num_symbol);
        pos = str.find('%');
    }
    return (str);
}

std::string Response::getBoundary(std::string const & cont_type)
{
    size_t pos = cont_type.find("boundary=", 0);
    if (pos == std::string::npos)
    {
        _status_code = 400;
        return ("");
    }
    else
        return (cont_type.substr(pos + 9, std::string::npos));
}

std::string Response::getExtention(std::string const & uri)
{
    std::size_t found = uri.find_last_of(".");
	if (found == std::string::npos)
        return ("");
    else
        return (uri.substr(found + 1));
}

void Response::processRequest()
{
    switch (prepareForProcessing()) // looking for final location and uri
    {
    case processing::Type::redirection:
        processRedirection();
        break;
    
    case processing::Type::cgi:
    {
        break;
    }
    
    case processing::Type::autoindex:
    {
        break;
    }

    case processing::Type::file:  // временная затычка
    {
        if (_request->_method == "GET")
        {
            std::ifstream file(_file.c_str());
            if (file.is_open())
            {
                _body << file.rdbuf();
                if (file.fail())
                    _status_code = 500;
                std::string ext = getExtention(_resulting_uri);
                _headers["Content-Type"] = _client->_master_serv->getMimeType(ext);
                file.close();
            }
            else
                _status_code = 404;
        }
        else if (_request->_method == "POST")
        {
            if (_content_type == "application/x-www-form-urlencoded")
            {
                std::ofstream file(_file.c_str(), std::ios::trunc);
                if (file.is_open())
                {
                    std::vector<std::string>    tuples;
                    split(_request->_body, tuples, '&');
                    std::cout << tuples.size() << std::endl;
                    while (!tuples.empty())
                    {
                        file << decodeURLencoded(tuples[0]) << "\n";
                        tuples.erase(tuples.begin());
                    }
                    file.close();
                }
                else
                    _status_code = 403; // Forbidden
            }
            else if (_content_type == "multipart/form-data")
            {
                std::string boundary = "--" + getBoundary(_request->_headers.find("Content-Type")->second);
                
            }
        }

        break;
    }

    case processing::Type::error:
        break;
    }
    handleError();  // пока ничего нет
    assembleResponse();
    _client->_state = client::State::writing;
}

int Response::sendResponse(void)
{
    int ret = send(_client->_fd, &_response[_sent], _response.size(), 0);

    std::cout << "Sent " << ret << " bytes" << std::endl;
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
    _resulting_uri.clear();
    _headers.clear();
    _body.clear();
    _sent = 0;
}

std::ostream & operator<<(std::ostream & o, Response const & resp)
{
    o << "\n***** Response *****\n" << std::endl;
    o << "Location:\n" << *resp._location << std::endl;
    o << "Resulting_URI: " << resp._resulting_uri << std::endl;
    o << "File: " << resp._file << std::endl << std::endl;
    o << resp._response << std::endl;
    return (o);
}