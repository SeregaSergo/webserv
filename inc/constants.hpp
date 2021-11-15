#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <sys/time.h>
#include <iostream>

namespace constants
{
    extern time_t           timeout_idle;
    extern time_t           timeout_ka;
    extern int              ka_time;
    extern int              ka_probes;
    extern int              ka_interval;
    extern const char *     default_file;

	extern const std::string    methods[4];
    extern const std::string    versions[2];

    extern unsigned long    limit_uri_length;
    extern unsigned long    limit_request_length;
    extern int              incoming_buffer;
}

namespace location
{
    namespace Type
    {
        enum Type
        {
            equal,
            extention,
            partial
        };
    }
}

namespace request
{
    namespace ReturnCode
    {
        enum ReturnCode
        {
            proceed,
            completed
        };
    }

    namespace State
    {
        enum State
        {
            method,
            URI,
            version,
            headerName,
            headerValue,
            body
        };
    }

    namespace TypeURI
    {
        enum TypeURI
        {
            absPath,    // http://www.w3.org/pub/WWW/TheProject.html
            asterisk,   // OPTIONS * HTTP/1.1 (request apply to the server itself)
            auth,       // only used by the CONNECT method
            absURI,     // /pub/WWW/TheProject.html
            none        // if not defined
        };
    }
}

namespace client
{
    namespace State
    {
        enum State
        {
            reading,
            writing,
            waitingForReq,
            shutdown
        };
    }
}

#endif