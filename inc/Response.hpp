#include "Request.hpp"
// #include "Client.hpp"
// #include "VirtServer.hpp"

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "constants.hpp"

class Client;
class Request;
class Location;

class Response {

private:
    Request *                           _request;
    int *                               _client_state;

    // Data members for sending
    std::string                         _response;
    unsigned long                       _sent;

    // Data for contructing a response
    int                                 _status_code;
    std::map<std::string,std::string>   _headers;
    std::stringstream                   _body;

    // Data for file processing
    VirtServer *                        _virt_serv;
    Location *                          _location;
    std::string                         _resulting_uri;
    std::string                         _file;

    int processMethod(void);
    void handleError(void);
    void processRedirection(void);
    bool isConnectionAlive(void);
    void assembleResponse(void);

public:
    Response(Request * req, int * cl_state)
        : _request(req)
        , _client_state(cl_state)
        , _sent(0)
        , _status_code(0)
    {}

    Response(Response const & src);

    void processRequest(void);
    int sendResponse(int fd);
    void clear(void);

    friend class Get;
    friend class Post;
    friend class Delete;
    friend std::ostream & operator<<(std::ostream & o, Response const & resp);
};

#endif