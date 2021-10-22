#include "../inc/Webserv.hpp"

// You need to be very carefull with exceptions in construtor.
// Don't let the memory to leak!
Webserv::Webserv(const char * config_path)
{
    Config conf;
    
    close(0);
    int file = open(config_path, O_RDONLY);
    if (yyparse(&conf))
        throw std::runtime_error("Сonfig file is not valid");
    // prepare proccess for starting (keepalive, demonize, change select-limit);
}

Webserv::~Webserv(void) {
    delete _err_log;
    for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); ++it)
        delete *it;
}

void Webserv::sendErrMsg(std::string & msg) {
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
            throw std::runtime_error("Select error: " + std::string(strerror(errno)));
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