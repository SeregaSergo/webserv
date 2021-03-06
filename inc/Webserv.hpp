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
#include <stdexcept>
#include <string.h>
#include <sstream>
#include "constants.hpp"
#include "Server.hpp"
#include "VirtServer.hpp"
#include "Location.hpp"
#include "Logger.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"

class VirtServer;
class Server;
class Location;

////////////////////////////////////////
// Config structure of virtual server //
////////////////////////////////////////

struct ConfigServ {
    std::vector<std::string>    server_names;
    std::string                 ip;
    int                         port;
    std::string                 acc_log;
    int                         client_max_body_size;
    std::string                 root;
    bool                        autoindex;
    bool                        sessions_enabled;
    std::vector<int>            err_num_temp;
    std::map<int, std::string>	err_pages;
    std::vector<Location>       locations;
    int                         location_lvl;
    bool                        lvl_inc_ban;
    
    ConfigServ(void)
    : port(-1)
    , client_max_body_size(constants::client_max_body_size)
    , autoindex(false)
    , sessions_enabled(false)
    , location_lvl(-1)
    , lvl_inc_ban(false)
    {}
    
    ~ConfigServ();

    std::vector<Location> & getLocations();
    bool                    incrementLvl(int pathType);
    bool                    decrementLvl();
    ConfigServ &            operator=(ConfigServ const & src);
    friend std::ostream &   operator<<(std::ostream & o, ConfigServ const & conf);
};


////////////////////////////////////////
// Config structure of whole webserv  //
////////////////////////////////////////

struct Config {
    std::string                         err_log;
    bool                                daemon;
    time_t                              timeout_idle;
    time_t                              timeout_ka;
    time_t                              timeout_session;
    time_t                              keepalive_time;
    int                                 num_probes;
    time_t                              keepalive_intvl;
    int                                 limit_uri_length;
    int                                 limit_headers_length;
    int                                 incoming_buffer;
    std::map<std::string, std::string>  mime_types;
    std::string                         mime_temp;
    std::vector<ConfigServ>             servers;

    Config(void)
    : daemon(false)
    , timeout_idle(constants::timeout_idle)
    , timeout_ka(constants::timeout_ka)
    , timeout_session(constants::timeout_session)
    , keepalive_time(constants::ka_time)
    , num_probes(constants::ka_probes)
    , keepalive_intvl(constants::ka_interval)
    , limit_uri_length(constants::limit_uri_length)
    , limit_headers_length(constants::limit_headers_length)
    , incoming_buffer(constants::incoming_buffer) {}

    friend std::ostream & operator<<(std::ostream & o, Config const & conf);
};

template <typename T>
std::string numToStr(T num)
{
    std::ostringstream ss;
    ss << num;
    return ss.str();
}

template <typename T>
std::string decToHexStr(T num)
{
    std::stringstream ss;
    ss << std::hex << num;
    return ss.str();
}

int yyparse(Config *config);


////////////////////////////////////////
//            Webserv class           //
////////////////////////////////////////
//
// It contains select loop that works with
// classes inherited from AFdHandler abstract
// parent class.
//
class Webserv {

private:
    std::vector<AFdHandler *>           _fds;
    int                                 _max_fd;
    static bool                         _quit;
    Logger *                            _err_log;
    std::vector<Server *>               _servers;
    
    void fillSets(fd_set * rs, fd_set * ws);
    void demonize(void);
    void setupParameters(Config & conf);
    void init_code_descriptions(void);
    void init_methods(void);
    bool makeServ(std::vector<ConfigServ> & conf_servers, std::string & result);
    static void quitSignalHandler(int signum);

public:
    Webserv(const char * config_path, std::string & result);
    ~Webserv(void);
    void sendErrMsg(std::string const & msg);
    void addHandler(AFdHandler * h);
    bool removeHandler(AFdHandler * h);
    void removeAllClients(void);

    void start();
    void stop() { _quit = true; }  
    void reset();
};

#endif