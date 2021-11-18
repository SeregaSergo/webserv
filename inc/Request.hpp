#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "constants.hpp"
#include "Location.hpp"

class Request {

private:
    std::string                         _method;
    std::string                         _uri;
    std::string                         _http_version;
    std::map<std::string, std::string>  _headers;

    std::string                         _header_name;

    int                                 _state;
    int                                 _status_code;

    char *                              _buffer;
    char *                              _ptr;
    char *                              _end;
    char *                              _head;  // during parsing: left border
    char *                              _tail;  // during parsing: right border

    int freeSpace(void);
    int filledSpace(void);
    bool passSymbols(char c);
    bool findEndOfWord(char c);
    bool findEndOfLine(void);
    bool isVersionSupported(void);
    bool isEndOfHeaders(void);
    bool getUri(void);
    bool isMethodImplemented(void);
    int errorCode(int code);

public:
    Request();
    ~Request(void);
    int getRequest(int socket);
    int parseData(void);
    int getStatusCode(void);

    friend std::ostream & operator<<(std::ostream & o, Request const & req);
};

#endif