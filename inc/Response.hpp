#include "Request.hpp"
#include "Client.hpp"

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "constants.hpp"

class Client;

class Response {

private:
    const Request *         _request;
    Client *                _client;
    std::string             _response;
    std::string             _resulting_uri;

    unsigned long           _sent;

    void handleError(void);
    void processRedirection(Request const & request);

public:
    Response(Client * client) : _request(NULL), _client(client), _sent(0) {}
    void processRequest(const Request * request);
    int sendResponse(void);
    void clear(void);
};

#endif