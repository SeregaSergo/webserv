#include "../inc/Response.hpp"
#include <sstream>

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

void Response::handleError(void)
{
    std::string file = _request->_virt_serv->getPage(_request->_status_code);
    if (!file.empty())
        _resulting_uri = file;
}

void Response::processRedirection(void)
{
    Redirect * redir = _location->getRedir();
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

void Response::processRequest()
{
    switch (processMethod()) // looking for final location and uri
    {
    case processing::Type::redirection:
        processRedirection();
        break;
    
    case processing::Type::cgi:
        std::cout << "CGI" << std::endl;
        break;
    
    case processing::Type::autoindex:
        break;

    case processing::Type::error:
        handleError();  // пока ничего нет
        break;
    }

    assembleResponse();
    *_client_state = client::State::writing;
}

int Response::sendResponse(int fd)
{
    int ret = send(fd, &_response[_sent], _response.size(), 0);

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
    _headers.clear();
    _body.str(std::string());
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