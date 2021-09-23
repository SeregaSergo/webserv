#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include "Server.hpp"
#include <sys/time.h>

#define MUTE_FILE   "/dev/null"

typedef unsigned long long int  time_t;

class Config {

private:
    char *                              _err_log_file;
    char *                              _acc_log_file;
    int                                 _timeout;
    std::map<std::string, std::string>  _mime_types;
    std::vector<Server>                 _servers;

    time_t    cur_time(void);

public:
    Config();
    Config(Config const & src);
    ~Config();
	Config & operator=(Config const & src);

    void set_timeout(int sec);
    int open_logs(void);
    int close_logs(void);
    bool check_timeout(time_t time);  // true if time is up
    void add_type(std::string & ext, std::string & description);
    std::string & get_type(std::string const & type);
    void add_server();
    std::vector<Server> & get_servers();
};

#endif