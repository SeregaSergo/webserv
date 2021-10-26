#include "../inc/Webserv.hpp"

// You need to be very carefull with exceptions in construtor.
// Don't let the memory to leak!
Webserv::Webserv(const char * config_path)
    : _max_fd(0)
    , _quit(false)
    , _err_log(NULL)
{
    Config conf;
    
    int file = open(config_path, O_RDONLY);
    if (yyparse(&conf))
        throw std::runtime_error("Сonfig file is not valid");
    close(file);
    setupParameters(conf);
    while (conf.servers.size() != 0)
        makeServ(conf.servers);
    if (_servers.empty())
        throw std::runtime_error("Servers are not defined");
    if (conf.daemon)
        demonize();
}

void Webserv::setupParameters(Config & conf)
{
    int err_log_fd = open(&conf.err_log[0], O_WRONLY|O_CREAT|O_TRUNC);
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
    std::map<std::string, VirtServer *> virt_map;
    for (std::vector<ConfigServ>::iterator it = ++conf.begin(); it != conf.end(); ++it)
    {
        if ((*it).port == port)
        {
            if ((*it).ip == ip)
            {
                serv_create.push_back(*it);
                serv_create.erase(it);
                --it;
            }
            else if ((*it).ip.empty() || ip.empty())
                throw std::runtime_error("Ip/host mismatch");
        }
    }
    for (std::vector<ConfigServ>::iterator it = serv_create.begin(); it != serv_create.end(); ++it)
    {
        int acc_log_fd = open(&(*it).acc_log[0], O_WRONLY|O_CREAT|O_TRUNC);
        if (acc_log_fd < 0) {
            std::cerr << "WARNING: can't open access file: " << &(*it).acc_log[0] << std::endl;
            acc_log_fd = open(constants::default_file, O_WRONLY);
        }
        Logger * acc_logger = new Logger(acc_log_fd);
        addHandler(acc_logger);
        _virt_servers.push_back(VirtServer(acc_logger, (*it).client_max_body_size, \
                        (*it).err_pages, (*it).locations));
        for (std::vector<std::string>::iterator i = (*it).server_names.begin(); i != (*it).server_names.end(); ++i)
            virt_map.insert(std::pair<std::string, VirtServer *>(*i, &_virt_servers.back()));
    }
    virt_map.insert(std::pair<std::string, VirtServer *>("", &_virt_servers[pos_new]));
    _servers.push_back(Server::create(ip, port, this, virt_map));
}

Webserv::~Webserv(void)
{
    if (_err_log)
        delete _err_log;
    for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); ++it)
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
    if (fd > _max_fd)
    {
        _fds.insert(_fds.end(), _max_fd - fd, 0);
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
    struct timeval  timeout;
    struct timeval  cur_time;

    timeout.tv_sec = constants::timeout_idle;
    timeout.tv_usec = 0;

    while (!_quit)
    {
        fd_set  rs, ws;
        fillSets(&rs, &ws);
        res = select(_max_fd + 1, &rs, &ws, 0, &timeout);
        if (res < 0) {
            if (errno == EINTR)
                continue;
            throw std::runtime_error("select error: " + std::string(strerror(errno)));
        }
        gettimeofday(&cur_time, NULL);
        if (res == 0) {
            for (int i = 0; i <= _max_fd; ++i) {
                if (!_fds[i])
                    continue;
                _fds[i]->checkTimeout(cur_time);
            }
        }
        else {
            for (int i = 0; i <= _max_fd; ++i) {
                if (!_fds[i] || _fds[i]->checkTimeout(cur_time))
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
