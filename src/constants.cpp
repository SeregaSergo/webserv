#include "../inc/constants.hpp"

namespace constants
{
    // Default config parameters
    time_t          timeout_idle = 10;
    time_t          timeout_ka = 86400;         // 24 h
    time_t          timeout_session = 3600;     // 1h
    time_t          timeout_sid_cleaning = 60;
    int             ka_time = 600;		        // every 10 min
    int             ka_probes = 5;
    int             ka_interval = 30;
    const char *    default_file = "/dev/null";
    unsigned long   limit_uri_length = 1024;
    unsigned long   limit_headers_length = 4096;
    const int       client_max_body_size = 104857600;   // 100 Mb
    const int       cgi_timeout = 5;
    
    int             incoming_buffer = 8192;
    // Implemented methods
	const std::string methods [] = {"GET", "POST", "DELETE"};
    const std::string versions [] = {"HTTP/1.1", "HTTP/1.0"};
    const std::string default_http_version("HTTP/0.9");
    
    std::map<int, std::string>          codes_description;
    std::map<std::string, AMethod *>    method;
    std::map<std::string, std::string>  mime_types;
}