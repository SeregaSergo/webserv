#include "Server.hpp"
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "constants.hpp"
#include <string.h>
#include <stdlib.h>

class Server;
class VirtServer;
class Location;

class Request {

private:
    std::string                         _method;
    std::string                         _uri;
    std::string                         _query;
    std::string                         _http_version;
    std::map<std::string, std::string>  _headers;
    std::string                         _body;
    Server *                            _server;
    VirtServer *                        _virt_serv;
    Location *                          _location;

    // Temporary variables for parsing
    int                                 _state;
    int                                 _status_code;
    int                                 _body_size;

    char *                              _buffer;
    char *                              _ptr;
    char *                              _end;
    char *                              _head;  // during parsing: left border
    char *                              _tail;  // during parsing: right border

    int freeSpace(void) const;
    int filledSpace(void) const;
    bool passSymbols(char c);
    bool findEndOfWord(char c);
    bool findEndOfLine(void);
    bool isVersionSupported(void);
    bool parseUri(void);
    bool isMethodImplemented(void);
    int errorCode(int code);
    void addToBody(void);

public:
    Request(Server * server);
    ~Request(void);
    int getRequest(int socket);
    int parseData(void);
    bool isLastRequest(void);
    void clear(void);

    std::string & getMethod(void);
    std::string & getURI(void);
    std::string & getQuery(void);
    std::string const & getHttpVersion(void);
    bool hasVirtServer(void);

    std::map<std::string, std::string> & getHeaders(void);
    std::string const & getBody(void);
    VirtServer * getVirtualServer(void);

    friend std::ostream & operator<<(std::ostream & o, Request const & req);
    friend class Response;
};

#endif