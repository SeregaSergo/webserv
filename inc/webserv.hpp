#ifndef WEBSERV_HPP
#define WEBSERV_HPP

// #define FD_SETSIZE 2048
// #define _DARWIN_UNLIMITED_SELECT

#include <iostream>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <fcntl.h>
#include <sys/select.h>
#include <stdexcept>
#include <string.h>
#include "Logger.hpp"
#include "Server.hpp"
#include "Location.hpp"
//#define YYSTYPE char *

namespace constants
{
    time_t          timeout_idle = 10;
    time_t          timeout_ka = 86400; // 24 h
    int             ka_time = 600;		// every 10 min
    int             ka_probes = 5;
    int             ka_interval = 30;
    const char *    default_file = "/dev/null";
}

struct ConfigServ {
    std::vector<std::string>    server_names;
    std::string                 ip;
    int                         port;
    std::string                 acc_log;
    int                         client_max_body_size;
    std::string                 root;
    bool                        autoindex;
    std::vector<int>            err_num_temp;
    std::map<int, std::string>	err_pages;
    std::vector<Location>       locations;
    
    ConfigServ(void)
    : port(-1)
    , client_max_body_size(-1)
    , autoindex(false) {}
};

struct Config {
    std::string                         err_log;
    bool                                daemon;
    time_t                              timeout_idle;
    time_t                              timeout_ka;
    time_t                              keepalive_time;
    int                                 num_probes;
    time_t                              keepalive_intvl;
    std::map<std::string, std::string>  mime_types;
    std::string                         mime_temp;
    std::vector<ConfigServ>             servers;

    Config(void)
    : daemon(false)
    , timeout_idle(constants::timeout_idle)
    , timeout_ka(constants::timeout_ka)
    , keepalive_time(constants::ka_time)
    , num_probes(constants::ka_probes)
    , keepalive_intvl(constants::ka_interval) {}
};

int yyparse(Config *config);
class VirtServer;
class Server;
class Location;

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
    void demonize(void);
    void setupParameters(Config & conf);
    void makeServ(std::vector<ConfigServ> & conf_servers);

public:
    Webserv(const char * config_path);
    ~Webserv(void);
    void sendErrMsg(std::string const & msg);
    const std::string & getMimeType(std::string & ext);
    void addHandler(AFdHandler * h);
    bool removeHandler(AFdHandler * h);
    void start();
    void stop() { _quit = true; }  
    void reset();
};

#endif