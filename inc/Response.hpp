#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "Webserv.hpp"

class Response {

private:
    VirtServer *            _virt_serv;
    Location *              _location;
    std::string             _result_uri;

public:
    Response(Request const & request);
};

#endif