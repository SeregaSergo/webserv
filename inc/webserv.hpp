#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <fcntl.h>
#include <sys/select.h>
#include "AFdHandler.hpp"
#include <stdexcept>
#include <string.h>
#include "Logger.hpp"
#include "Server.hpp"

#define YYSTYPE char *

int yyparse(Config *config);

namespace constants
{
    time_t      timeout_idle = 10;  // it is constant after parsing of the config
    time_t      timeout_ka = 86400; // it is constant after parsing of the config
}

struct Config {
    std::string                         err_log;
    bool                                daemon;
    time_t                              timeout_idle;
    time_t                              timeout_ka;
    time_t                              keepalive_time;
    int                                 num_probes;
    time_t                              keepalive_intvl;
    std::map<std::string, std::string>  mime_types;
};

class Webserv {

private:
    std::vector<AFdHandler*>            _fds;
    int                                 _max_fd;
    bool                                _quit;
    Logger *                            _err_log;
    std::vector<Server *>               _servers;
    std::vector<VirtServer>             _virt_servers;
    std::map<std::string, std::string>  _mime_types;    // (ext, MIME type)
    
    void fillSets(fd_set * rs, fd_set * ws);

public:
    Webserv(const char * config_path);
    ~Webserv(void);
    void sendErrMsg(std::string & msg);
    const std::string & getMimeType(std::string & ext);
    void addHandler(AFdHandler * h);
    bool removeHandler(AFdHandler * h);
    void start();
    void stop() { _quit = true; }  
};

#endif