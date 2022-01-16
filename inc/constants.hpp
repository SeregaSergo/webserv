#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <sys/time.h>
#include <iostream>
#include <map>

class AMethod;

namespace constants
{
    extern time_t                               timeout_idle;
    extern time_t                               timeout_ka;
    extern time_t                               timeout_session;
    extern time_t                               timeout_sid_cleaning;
    extern int                                  ka_time;
    extern int                                  ka_probes;
    extern int                                  ka_interval;
    extern const char *                         default_file;
    extern const int                            cgi_timeout;

	extern const std::string                    methods[3];
    extern const std::string                    versions[2];
    extern std::map<int, std::string>           codes_description;
    extern std::map<std::string, AMethod *>     method;
    extern std::map<std::string, std::string>   mime_types;

    extern unsigned long                        limit_uri_length;
    extern unsigned long                        limit_headers_length;
    extern const int                            client_max_body_size;
    extern int                                  incoming_buffer;
    extern const std::string                    default_http_version;
}

namespace location
{
    namespace pathType
    {
        enum pathType
        {
            none,
            partial,
            extention,
            equal
        };
    }

    namespace Type
    {
        enum Type
        {
            redirection,
            cgi,
            file
        };
    }
}

namespace request
{
    namespace ReturnCode
    {
        enum ReturnCode
        {
            error,
            disconnected,
            unfinished,
            completed
        };
    }

    namespace State
    {
        enum State
        {
            getting_headers,
            endOfHeaders,
            method,
            uri,
            version,
            headerName,
            headerValue,
            body,
            chunk_size,
            chunk_endl,
            chunk,
            done
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
            processing,
            writing,
            waitingForReq,
            shutdown
        };
    }
}

namespace processing
{
    namespace Type
    {
        enum Type
        {
            redirection,
            cgi,
            autoindex,
            file,
            error,
            done
        };
    }
}

namespace response
{
    namespace ReturnCode
    {
        enum ReturnCode
        {
            error,
            unfinished,
            completed
        };
    }

    namespace CGI
    {
        enum CGI
        {
            server_redir,
            document
        };
    }
}

#endif