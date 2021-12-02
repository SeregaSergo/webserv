#include "../inc/Response.hpp"
#include <sstream>

void Response::handleError(void)
{
    std::string file = _request->_virt_serv->getPage(_request->_status_code);
    if (!file.empty())
        _resulting_uri = file;
}

void Response::processRedirection(Request const & request)
{
    Redirect * redir = request._location->getRedir();
    int code = redir->get_code();

    _response.append(request._http_version);
    _response.push_back(' ');
    _response.append(std::to_string(code));
    _response.push_back(' ');
    _response.append(constants::codes_description.find(code)->second);
    _response.append("\r\nLocation: ");
    _response.append(redir->rewrite_url(request._uri));
    _response.append("\r\n\r\n");
}

void Response::processRequest(const Request * request)
{
    _request = request;
    if (request->_status_code == 200)
    {
        switch (request->_location->getType())
        {
        case location::Type::redirection:
            processRedirection(*request);
            _client->_state = client::State::writing;
            break;
        case location::Type::cgi:
        {

        }
        case location::Type::file:  // временная затычка
        {
            std::stringstream resp;
            std::string body("<html>\n<head>\n<title>Test upload</title>\n</head>\n<body>\n<h2>Select files to upload</h2>\
            \n<form name=\"upload\" method=\"POST\" enctype=\"multipart/form-data\" action=\"/upload\">\n<input type=\"file\" name=\"file1\"><br>\
            <input type=\"file\" name=\"file2\"><br>\n<input type=\"submit\" name=\"submit\" value=\"Upload\">\n<input type=\"hidden\" name=\"test\" value=\"value\">\
            </form>\n</body>\n</html>");
            resp << "HTTP/1.1 " << _request->getStatusCode() << " " << constants::codes_description[_request->getStatusCode()] << "\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Content-Type: text/html; charset=utf-8\r\n"
                << "Content-Length: " << body.length()
                << "\r\n\r\n"
                << body;
            _response = resp.str();
            _client->_state = client::State::writing;
            break;
        }
        }

    }
    handleError();
}

int Response::sendResponse(void)
{
    int ret = send(_client->_fd, &_response[_sent], _response.size(), 0);

    if (ret < 0)
        return (response::ReturnCode::error);
    if (ret == 0)
        return (response::ReturnCode::disconnected);
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
    _sent = 0;
}