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

#define DEFAULT_TIMEOUT_IDLE    10
#define DEFAULT_TIMEOUT_KA      NULL

#define YYSTYPE char *

int yyparse(Config *config);

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
    time_t                              _timeout_idle;
    time_t                              _timeout_ka;
    Logger *                            _err_log;
    std::vector<Server *>               _servers;
    std::map<std::string, std::string>  _mime_types;    // (ext, MIME type)
    
    void fillSets(fd_set * rs, fd_set * ws);
    void getDeadline(struct timeval * deadlineReq, struct timeval * deadlineKA);

public:
    Webserv(const char * config_path);
    ~Webserv();
    void sendErrMsg(std::string * msg);
    void addHandler(AFdHandler * h);
    bool removeHandler(AFdHandler * h);
    void flushLoggerBuffers(void);
    void start();
    void stop() { _quit = true; }  
};

#endif