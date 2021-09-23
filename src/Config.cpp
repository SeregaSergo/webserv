#include "../inc/Config.hpp"

Config::Config(void)
    : _err_log_file(MUTE_FILE),
    _acc_log_file(MUTE_FILE),
    _timeout(10000)
{
    _mime_types.insert(std::make_pair(std::string("html"), std::string("text/html")));
}

Config::Config(Config const & src) {*this = src;}

Config::~Config(void) {close_logs();}

Config & Config::operator=(Config const & src)
{
    if (&src == this)
        return (*this);
    _err_log_file = src._err_log_file;
    _acc_log_file = src._acc_log_file;
    _timeout = src._timeout;
    _mime_types = src._mime_types;
    _servers = src._servers;
    return (*this);
}

void Config::set_timeout(int sec) {
    _timeout = sec;
}

// fd 1 -> acces_log
// fd 2 -> error_log
int Config::open_logs(void)
{
    if (freopen(_acc_log_file, "w", stdout) == NULL)
         return (EXIT_FAILURE);
    if (freopen(_err_log_file, "w", stderr) == NULL)
         return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

int Config::close_logs(void)
{
    if (freopen(MUTE_FILE, "w", stdout) == NULL)
         return (EXIT_FAILURE);
    if (freopen(MUTE_FILE, "w", stderr) == NULL)
         return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

time_t    Config::cur_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

bool Config::check_timeout(time_t time)
{
    if ((cur_time() - time) > _timeout)
        return (true);
    else
        return (false);
}

void Config::add_type(std::string & ext, std::string & description) {
    _mime_types.insert(std::make_pair(ext, description));
}

std::string & Config::get_type(std::string const & type) {
    std::string & ret = _mime_types[type];
    if (ret.empty())
        ret = _mime_types[std::string("html")];
    return (ret);
}

void Config::add_server(void) {
    _servers.push_back(Server());
}

std::vector<Server> & Config::get_servers(void) {
    return (_servers);
}