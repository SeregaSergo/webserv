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
    err_num_temp = src.err_num_temp;
    err_pages = src.err_pages;
    locations = src.locations;
    return (*this);
}

// You need to be very carefull with exceptions in construtor.
// Don't let the memory to leak!
Webserv::Webserv(const char * config_path)
    : _max_fd(0)
    , _quit(false)
    , _err_log(NULL)
{
    Config conf;
    int file = open(config_path, O_RDONLY);
    if (file < 0)
        throw std::runtime_error("Can't open file: " + std::string(config_path));
    dup2(file, 0);
    close(file);
    if (yyparse(&conf))
        throw std::runtime_error("Сonfig file is not valid");
    std::cout << conf << std::endl;     // debug print
    setupParameters(conf);
    while (conf.servers.size() != 0)
        makeServ(conf.servers);
    if (_servers.empty())
        throw std::runtime_error("Servers are not defined");
    std::cout << "Starting webserv!" << std::endl;
    if (conf.daemon)
        demonize();
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
    constants::ka_time = conf.keepalive_time;
    constants::ka_probes = conf.num_probes;
    constants::ka_interval = conf.keepalive_intvl;
    _mime_types.swap(conf.mime_types);
}

void Webserv::makeServ(std::vector<ConfigServ> & conf)
{
    int pos_new = _virt_servers.size();
    int port = conf[0].port;
    std::string ip = conf[0].ip;
    std::vector<ConfigServ> serv_create;
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
        _virt_servers.push_back(VirtServer::create(*it, *this));
        for (std::vector<std::string>::iterator i = (*it).server_names.begin(); i != (*it).server_names.end(); ++i)
            virt_map.insert(std::pair<std::string, VirtServer *>(*i, _virt_servers.back()));
    }
    virt_map.insert(std::pair<std::string, VirtServer *>("", _virt_servers[pos_new]));
    
    // Creating real server
    _servers.push_back(Server::create(ip, port, this, virt_map));
}

Webserv::~Webserv(void)
{
    if (_err_log)
        delete _err_log;
    for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); ++it)
        delete *it;
    for (std::vector<VirtServer *>::iterator it = _virt_servers.begin(); it != _virt_servers.end(); ++it)
        delete *it;
}

void Webserv::sendErrMsg(std::string const & msg) {
    _err_log->sendMsg(msg);
}

const std::string & Webserv::getMimeType(std::string & ext)
{
    std::map<std::string, std::string>::iterator it = _mime_types.find(ext);
    if (it == _mime_types.end())
        it = _mime_types.find("");
    return ((*it).second);
}

void Webserv::addHandler(AFdHandler * h)
{
    int fd = h->getFd();
    std::cout << fd << "|max_fd=" << _max_fd << std::endl;
    if (fd > _max_fd)
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
    _fds[fd] = 0;
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
                bool r = FD_ISSET(i, &rs);
                bool w = FD_ISSET(i, &ws);
                if (r | w)
                    _fds[i]->handle(r, w);
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

    // freopen(MUTE_FILE, "r", stdin);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
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
    {
        o << "******************" << std::endl;
        o << "Server names: " << std::endl;
        for (std::vector<std::string>::const_iterator i = (*it).server_names.begin(); i != (*it).server_names.end(); ++i)
            o << "    " << *i << std::endl;
        o << "IP: " << (*it).ip << std::endl;
        o << "Port: " << (*it).port << std::endl;
        o << "Acc log: " << (*it).acc_log << std::endl;
        o << "max body: " << (*it).client_max_body_size << std::endl;
        o << "Root: " << (*it).root << std::endl;
        o << "Autoindex: " << (*it).autoindex << std::endl;
        o << "Error pages: " << std::endl;
        for (std::map<int, std::string>::const_iterator i = (*it).err_pages.begin(); i != (*it).err_pages.end(); ++i)
            o << "    " << (*i).first << ": " << (*i).second << std::endl;
        o << "Locations: " << std::endl;
        for (std::vector<Location>::const_iterator i = (*it).locations.begin(); i != (*it).locations.end(); ++i)
        {
            o << "*****************" << std::endl;
            o << *i << std::endl;
        }
    }
    return (o);
}