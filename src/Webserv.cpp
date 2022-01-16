#include "../inc/Webserv.hpp"

ConfigServ & ConfigServ::operator=(ConfigServ const & src)
{
    server_names = src.server_names;
    ip = src.ip;
    port = src.port;
    acc_log = src.acc_log;
    client_max_body_size = src.client_max_body_size;
    root = src.root;
    autoindex = src.autoindex;
    sessions_enabled = src.sessions_enabled;
    err_num_temp = src.err_num_temp;
    err_pages = src.err_pages;
    locations = src.locations;
    location_lvl = src.location_lvl;
    return (*this);
}

std::vector<Location> & ConfigServ::getLocations(void)
{
    std::vector<Location> * loc_vector = &locations;
    for (int i = 0; i < location_lvl; ++i) 
        loc_vector = &loc_vector->back().getLocations();
    return (*loc_vector);
}

bool    ConfigServ::incrementLvl(int pathType)
{
    ++location_lvl;
	if (lvl_inc_ban || location_lvl > 1)
		return false;
    if (pathType != location::pathType::partial)
        lvl_inc_ban = true;
    return true;
}

bool    ConfigServ::decrementLvl()
{
    --location_lvl;
    lvl_inc_ban = false;
    return true;
}

bool Webserv::_quit = false;

void Webserv::quitSignalHandler(int signum)
{
    std::cout << "[sig " << signum << "] Webserver was stopped!" << std::endl;
    Webserv::_quit = true;
}

// You need to be very carefull with exceptions in construtor.
// Don't let the memory to leak!
Webserv::Webserv(const char * config_path)
    : _max_fd(0)
    , _err_log(NULL)
{
    Config conf;
    int file = open(config_path, O_RDONLY);
    if (file < 0)
        throw std::runtime_error("Can't open file - " + std::string(config_path));
    dup2(file, 0);
    if (yyparse(&conf))
        throw std::runtime_error("Сonfig file is not valid");
    close(file);
    DEBUG(std::cout << conf << std::endl);
    setupParameters(conf);
    while (conf.servers.size() != 0)
        makeServ(conf.servers);
    if (_servers.empty())
        throw std::runtime_error("Servers are not defined");
    std::cout << "Starting webserv!" << std::endl;
    if (conf.daemon)
        demonize();
    sendErrMsg(std::string("Webserv has pid ") + numToStr(getpid()));
}

void init_codes_description(void)
{
    constants::codes_description[200] = "OK";
    constants::codes_description[201] = "Created";
    constants::codes_description[202] = "Accepted";
    constants::codes_description[203] = "Non-Authoritative Information";
    constants::codes_description[204] = "No Content";
    constants::codes_description[205] = "Reset Content";
    constants::codes_description[206] = "Partial Content";
    constants::codes_description[300] = "Multiple Choice";
    constants::codes_description[301] = "Moved Permanently";
    constants::codes_description[302] = "Found";
    constants::codes_description[303] = "See Other";
    constants::codes_description[304] = "Not Modified";
    constants::codes_description[305] = "Use Proxy";
    constants::codes_description[307] = "Temporary Redirect";
    constants::codes_description[308] = "Permanent Redirect";
    constants::codes_description[400] = "Bad Request";
    constants::codes_description[401] = "Unauthorized";
    constants::codes_description[403] = "Forbidden";
    constants::codes_description[404] = "Not Found";
    constants::codes_description[405] = "Method Not Allowed";
    constants::codes_description[406] = "Not Acceptable";
    constants::codes_description[408] = "Request Timeout";
    constants::codes_description[409] = "Conflict";
    constants::codes_description[411] = "Length Required";
    constants::codes_description[413] = "Payload Too Large";
    constants::codes_description[414] = "URI Too Long";
    constants::codes_description[415] = "Unsupported Media Type";
    constants::codes_description[431] = "Request Header Fields Too Large";
    constants::codes_description[500] = "Internal Server Error";
    constants::codes_description[501] = "Not Implemented";
    constants::codes_description[502] = "Bad Gateway";
    constants::codes_description[503] = "Service Unavailable";
    constants::codes_description[504] = "Gateway Timeout";
    constants::codes_description[505] = "HTTP Version Not Supported";
}

void Webserv::init_methods(void)
{
    constants::method["GET"] = new Get();
    constants::method["POST"] = new Post();
    constants::method["DELETE"] = new Delete();
}

void Webserv::setupParameters(Config & conf)
{
    int err_log_fd = open(&conf.err_log[0], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (err_log_fd < 0) {
        std::cerr << "WARNING: can't open error file." << std::endl;
        err_log_fd = open(constants::default_file, O_WRONLY);
    }
    _err_log = new Logger(err_log_fd);
    addHandler(_err_log);
    constants::timeout_idle = conf.timeout_idle;
    constants::timeout_ka = conf.timeout_ka;
    constants::timeout_session = conf.timeout_session < 120 ? 120 : conf.timeout_session;
    constants::ka_time = conf.keepalive_time;
    constants::ka_probes = conf.num_probes;
    constants::ka_interval = conf.keepalive_intvl;
    constants::limit_uri_length = static_cast<unsigned long>(conf.limit_uri_length);
    constants::limit_headers_length = static_cast<unsigned long>(conf.limit_headers_length);
    constants::incoming_buffer = conf.incoming_buffer;
    constants::mime_types.swap(conf.mime_types);
    constants::mime_types[""] = "application/octet-stream";
    constants::mime_types["default"] = "text/html";
    init_codes_description();
    init_methods();
    signal (SIGPIPE, SIG_IGN);
    signal (SIGTERM, Webserv::quitSignalHandler);
    signal (SIGINT, Webserv::quitSignalHandler);
}

void Webserv::makeServ(std::vector<ConfigServ> & conf)
{
    int port = conf[0].port;
    std::string ip = conf[0].ip;
    std::vector<ConfigServ> serv_create;
    std::vector<VirtServer *> virt_servers;
    serv_create.push_back(conf[0]);
    conf.erase(conf.begin());
    std::map<std::string, VirtServer *> virt_map;

    // Serching all virt_servers corresponding to first
    for (std::vector<ConfigServ>::iterator it = conf.begin(); it != conf.end(); ++it)
    {
        if ((*it).port == port)
        {
            if ((*it).ip == ip)
            {
                serv_create.push_back(*it);
                conf.erase(it);
                --it;
            }
            else if ((*it).ip.empty() || ip.empty())
                throw std::runtime_error("Ip/host mismatch");
        }
    }

    // Creating virtual servers for one real server
    for (std::vector<ConfigServ>::iterator it = serv_create.begin(); it != serv_create.end(); ++it)
    {
        virt_servers.push_back(VirtServer::create(*it, *this));
        for (std::vector<std::string>::iterator i = (*it).server_names.begin(); i != (*it).server_names.end(); ++i)
            virt_map.insert(std::pair<std::string, VirtServer *>(*i, virt_servers.back()));
    }
    virt_map.insert(std::pair<std::string, VirtServer *>("", virt_servers[0]));
    
    // Creating real server
    _servers.push_back(Server::create(ip, port, this, virt_map, virt_servers));
    std::cout << "[fd " << _servers.back()->getFd() << "] Server created " << ip << ":" << port << std::endl;
}

Webserv::~Webserv(void)
{
    if (_err_log)
        delete _err_log;
    for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); ++it)
        delete *it;
    for (std::map<std::string, AMethod *>::iterator it = constants::method.begin(); it != constants::method.end(); ++it)
        delete it->second;
}

void Webserv::sendErrMsg(std::string const & msg) {
    _err_log->sendMsg(msg);
}

void Webserv::addHandler(AFdHandler * h)
{
    int fd = h->getFd();
    if (fd >= _max_fd)
    {
        _fds.insert(_fds.end(), fd - _max_fd + 1, NULL);
        _max_fd = fd;
    }
    _fds[fd] = h;
}

bool Webserv::removeHandler(AFdHandler * h)
{
    int fd = h->getFd();
    if (fd > _max_fd || _fds[fd] != h)
        return (false);
    _fds[fd] = NULL;
    if (fd == _max_fd)
        while (_max_fd >= 0 && !_fds[_max_fd])
            --_max_fd;
    return (true);
}

void Webserv::fillSets(fd_set * rs, fd_set * ws)
{
    FD_ZERO(rs);
    FD_ZERO(ws);
    for (int i = 0; i <= _max_fd; ++i)
    {
        if (_fds[i])
        {
            if (_fds[i]->wantRead())
                FD_SET(i, rs);
            if (_fds[i]->wantWrite())
                FD_SET(i, ws);
        }
    }
}

void Webserv::start(void)
{
    int             res;
    fd_set          rs, ws;
    struct timeval  time;

    while (!_quit)
    {
        fillSets(&rs, &ws);
        time.tv_sec = constants::timeout_idle;
        time.tv_usec = 0;
        res = select(_max_fd + 1, &rs, &ws, 0, &time);
        if (res < 0) {
            if (errno == EINTR)
                continue;
            throw std::runtime_error("select error: " + std::string(strerror(errno)));
        }
        gettimeofday(&time, NULL);
        if (res == 0) {
            DISPLAY(std::cout << "waiting..." << std::endl);
            for (int i = 0; i <= _max_fd; ++i) {
                if (!_fds[i])
                    continue;
                _fds[i]->checkTimeout(time);
            }
        }
        else {
            for (int i = 0; i <= _max_fd; ++i) {
                if (!_fds[i] || _fds[i]->checkTimeout(time))
                    continue;
                if (FD_ISSET(i, &rs) || FD_ISSET(i, &ws))
                    _fds[i]->handle();
            }
        }
    }
}

void Webserv::demonize()
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    chdir("/");
    
    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
    
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
}

void Webserv::removeAllClients(void)
{
    for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); ++it)
        (*it)->removeAllClients();
}

std::ostream & operator<<(std::ostream & o, Config const & conf)
{
    o << "Err_log: " << conf.err_log << std::endl;
    o << "Daemon: " << conf.daemon << std::endl;
    o << "T_idle: " << conf.timeout_idle << std::endl;
    o << "T_ka: " << conf.timeout_ka << std::endl;
    o << "KA_time: " << conf.keepalive_time << std::endl;
    o << "Num probes: " << conf.num_probes << std::endl;
    o << "KA_intvl: " << conf.keepalive_intvl << std::endl;
    o << "Mime types: " << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = conf.mime_types.begin(); it != conf.mime_types.end(); ++it)
        o << "    [" << (*it).first << ", " << (*it).second << "]\n";
    o << "Servers: " << std::endl;
    for (std::vector<ConfigServ>::const_iterator it = conf.servers.begin(); it != conf.servers.end(); ++it)
        o << *it << std::endl;
    return (o);
}

std::ostream & operator<<(std::ostream & o, ConfigServ const & conf)
{
    o << "*****Config VirtServ******" << std::endl;
    o << "Server names: " << std::endl;
    for (std::vector<std::string>::const_iterator i = conf.server_names.begin(); i != conf.server_names.end(); ++i)
        o << "    " << *i << std::endl;
    o << "IP: " << conf.ip << std::endl;
    o << "Port: " << conf.port << std::endl;
    o << "Acc log: " << conf.acc_log << std::endl;
    o << "max body: " << conf.client_max_body_size << std::endl;
    o << "Root: " << conf.root << std::endl;
    o << "Autoindex: " << conf.autoindex << std::endl;
    o << "Sessions enabled: " << conf.sessions_enabled << std::endl;
    o << "Error pages: " << std::endl;
    for (std::map<int, std::string>::const_iterator i = conf.err_pages.begin(); i != conf.err_pages.end(); ++i)
        o << "    " << (*i).first << ": " << (*i).second << std::endl;
    o << "Locations: " << std::endl;
    for (std::vector<Location>::const_iterator i = conf.locations.begin(); i != conf.locations.end(); ++i)
    {
        o << "******* First level Location **********" << std::endl;
        o << *i << std::endl;
    }
    return (o);
}