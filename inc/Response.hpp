#include "Request.hpp"
#include "Client.hpp"

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "constants.hpp"

class Client;

class Response {

private:
    const Request *                     _request;
    Client *                            _client;

    std::string                         _response;
    std::map<std::string,std::string>   _headers;
    std::string                         _body;

    int                                 _status_code;
    Location *                          _location;
    std::string                         _resulting_uri;
    std::string                         _file;
    unsigned long                       _sent;

    static std::map<std::string, int (Response::*)(void)>	_prepare_func;
	static std::map<std::string, int (Response::*)(void)>	initPrepareFunc(void); 

    void handleError(void);
    void processRedirection(void);
    bool isConnectionAlive(void);
    void assembleResponse(void);
    bool isRequestedADirectory(void);
    bool isFileExist(std::string const & temp_file);
    int errorCode(int code);

public:
    Response(Client * client, Request * req) : _request(req), _client(client), _status_code(0), _sent(0) {}
    void processRequest(void);
    int prepareForProcessing(void);
    int prepareGET(void);
    int preparePOST(void);
    int prepareDELETE(void);
    int sendResponse(void);
    void clear(void);

    friend std::ostream & operator<<(std::ostream & o, Response const & resp);
};

#endif