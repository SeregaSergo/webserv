#include "../inc/Webserv.hpp"

// You need to be very carefull with exceptions in construtor.
// Don't let the memory to leak!
Webserv::Webserv(const char * config_path)
{
    Config conf;
    int file = open(config_path, O_RDONLY);
    dup2(file, 0);
    if (yyparse(&conf))
        throw std::runtime_error("Сonfig file is not valid");
    // prepare proccess for starting (keepalive, demonize, change select-limit);
};