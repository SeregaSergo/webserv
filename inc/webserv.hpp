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
#include "Logger.hpp"
#include "Server.hpp"
#include "EventSelector.hpp"

#define YYSTYPE char *

int yyparse(Config *config);

struct Config {
    std::string     err_log;
    time_t          timeout;
    time_t          keepalive_time;
    int             num_probes;
    time_t          keepalive_intvl;
};

class Webserv {

private:
    Logger                              _err_log;
    EventSelector                       _selector;
    std::vector<Server *>               _servers;
    std::map<std::string, std::string>  _mime_types;

public:
    Webserv(const char * config_path);
    ~Webserv();
    void sendErrMsg(std::string * msg);
    const std::string & getMimeType(std::string & ext);
    void addHandler(AFdHandler * h);
    bool removeHandler(AFdHandler * h);
};

#endif