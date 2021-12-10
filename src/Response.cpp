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


// int Response::preparePOST(void)
// {
//     bool isDirectory = isRequestedADirectory();
//     size_t pos = 0;

//     if (_location->getType() == location::Type::cgi && !isDirectory)
//         return (processing::Type::cgi);
    
//     std::map<std::string,std::string>::const_iterator it = _request->_headers.find("Content-Type");
//     if (it == _request->_headers.end())
//         return (errorCode(415));

    
//     if ((pos = it->second.find(';')) != std::string::npos)
//         _content_type = it->second.substr(0, pos);
//     else
//         _content_type = it->second;
//     if (_content_type == "application/x-www-form-urlencoded")
//     {
//         if (isDirectory)
//             return (errorCode(400));
//         if (isFileExist(_file))
//             return (errorCode(409));    // Conflict ?
//         return (processing::Type::file);
//     }
//     else if (_content_type == "multipart/form-data")
//     {
//         if (isDirectory)
//             return (processing::Type::file);
//         return (errorCode(400));
//     }
//     else
//         return (errorCode(415));    // Unsupported Media Type
// }

// int Response::prepareDELETE(void)
// {
//     if (isRequestedADirectory())
//         return (errorCode(400));
//     if (!isFileExist(_file))
//         return (errorCode(404));
//     if (_location->getType() == location::Type::cgi)
//         return (processing::Type::cgi);
//     else
//         return (processing::Type::file);
// }


// void Response::processPOST(void)
// {
//     if (_content_type == "application/x-www-form-urlencoded")
//     {
//         std::ofstream file(_file.c_str(), std::ios::trunc);
//         if (file.is_open())
//         {
//             std::vector<std::string>    tuples;
//             split(_request->_body, tuples, '&');
//             std::cout << tuples.size() << std::endl;
//             while (!tuples.empty())
//             {
//                 file << decodeURLencoded(tuples[0]) << "\n";
//                 tuples.erase(tuples.begin());
//             }
//             file.close();
//         }
//         else
//             _status_code = 403; // Forbidden
//     }
//     else if (_content_type == "multipart/form-data")
//     {
//         std::string boundary = "--" + getBoundary(_request->_headers.find("Content-Type")->second);
        
//     }
// }

// void Response::processDELETE(void)
// {

// }

int Response::processMethod(void)
{
    _virt_serv = _request->_virt_serv;
    _status_code = _request->_status_code;
    _resulting_uri = _request->_uri;
    _location = _request->_location;
    _file = _location->getResoursePath(_resulting_uri);

    if (_status_code != 200)
        return (processing::Type::error);
    if (_location->getType() == location::Type::redirection)
        return (processing::Type::redirection);
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

        break;
    
    case processing::Type::autoindex:
        break;

    case processing::Type::error:
        handleError();  // пока ничего нет
        break;
    
    case processing::Type::done:
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