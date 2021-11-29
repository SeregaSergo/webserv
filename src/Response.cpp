#include "../inc/Response.hpp"

Response::Response(Request const & request)
{
    std::map<std::string, std::string>::iterator    ptr;
    ptr = _headers.find("Host");
    if (ptr == _headers.end())
        _virt_serv = _server->getVirtualServ("");
    else
        _virt_serv = _server->getVirtualServ(ptr->second);
    if ((_location = _virt_serv->chooseLocation(_uri)) == NULL)
        return (errorCode(404));     // Not found
}