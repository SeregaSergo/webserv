#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "constants.hpp"

class Request {

private:
    std::string                         _method;
    std::string                         _uri;
    char                                _uri_type;
    std::string                         _http_version;
    std::map<std::string, std::string>  _headers;

    int                                 _state;
    int                                 _status_code;

    char *                              _buffer;
    char *                              _ptr;
    char *                              _end;
    char *                              _head;  // during parsing: left border
    char *                              _tail;  // during parsing: right border

    int freeSpace(void);
    int filledSpace(void);
    int passWhitespaces(void);
    int findNextWhitespace(void);
    int findEndOfLine(void);
    bool isVersionSupported(void);
    
public:
    Request();
    ~Request(void);
    int getRequest(int socket);
    int parseData(void);
};

#endif