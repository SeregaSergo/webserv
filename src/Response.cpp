#include "../inc/Response.hpp"
#include <sstream>

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
    _response.append(std::to_string(_status_code));
    _response.push_back(' ');
    _response.append(constants::codes_description.find(_status_code)->second);
    if (!_body.empty())
        _headers["Content-Length"] = std::to_string(_body.size());
    for (std::map<std::string,std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        _response.append("\r\n");
        _response.append((*it).first);
        _response.append(": ");
        _response.append((*it).second);
    }
    _response.append("\r\n\r\n");
    _response.append(_body);
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

int Response::prepareForProcessing(void)
{
    if (_status_code != 200)
        return (processing::Type::error);
    if (_location->getType() == location::Type::redirection)
        return (processing::Type::redirection);
    if (isRequestedADirectory())
    {
        std::vector<std::string>    index_files = _location->getIndexFiles();
        std::string                 temp_uri;
        Location *                  temp_loc;
        std::string                 temp_file;
        while (!index_files.empty())
        {
            temp_uri = _resulting_uri + index_files.front();
            temp_loc = _request->_virt_serv->chooseLocation(temp_uri);
            temp_file = temp_loc->getFile(temp_uri);
            if (isFileExist(temp_file))
            {
                _location = temp_loc;
                _resulting_uri = temp_uri;
                _file = temp_file;
                break;
            }
            index_files.erase(index_files.begin());
        }
        if (index_files.empty() && _location->getAutoindex())
            return (processing::Type::autoindex);
        else
        {
            _status_code = 404;
            return (processing::Type::error);
        }
    }
    if (_location->getType() == location::Type::cgi)
        return (processing::Type::cgi);
    else
        return (processing::Type::file);
}

void Response::processRequest()
{
    _status_code = _request->_status_code;
    _resulting_uri = _request->_uri;
    _location = _request->_location;

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

    case processing::Type::file:  // временная затычка
    {
        _body = "<html>\n<head>\n<title>Test upload</title>\n</head>\n<body>\n<h2>Select files to upload</h2>\
        \n<form name=\"upload\" method=\"POST\" enctype=\"multipart/form-data\" action=\"/upload\">\n<input type=\"file\" name=\"file1\"><br>\
        <input type=\"file\" name=\"file2\"><br>\n<input type=\"submit\" name=\"submit\" value=\"Upload\">\n<input type=\"hidden\" name=\"test\" value=\"value\">\
        </form>\n</body>\n</html>";
        _headers["Content-Type"] = "text/html; charset=utf-8";
        break;
    }

    case processing::Type::error:
        break;
    }
    handleError();
    assembleResponse();
    _client->_state = client::State::writing;
}

int Response::sendResponse(void)
{
    int ret = send(_client->_fd, &_response[_sent], _response.size(), 0);

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
    o << resp._response << std::endl;
    return (o);
}