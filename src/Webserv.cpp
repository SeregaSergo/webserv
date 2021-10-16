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

void Webserv::getDeadline(struct timeval * deadlineReq, struct timeval * deadlineKA)
{
    gettimeofday(deadlineReq, NULL);
    deadlineReq->tv_sec -= _timeout_idle;
    if (!_timeout_ka)
        deadlineKA->tv_sec = 1;
    else
        deadlineKA->tv_sec -= _timeout_ka;
}

void Webserv::flushLoggerBuffers(void)
{
    _err_log.flushBuf();
    for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); ++it)
        (*it)->flushLoggerBuffers();
}

void Webserv::start(void)
{
    int             res;
    struct timeval  timeout;        // for select timeout
    struct timeval  deadlineReq;    // for deleteing clients by idle time
    struct timeval  deadlineKA;     // for deleteing clients with keep-alive
    time_t          sleep_time = _timeout_idle;

    timeout.tv_usec = 0;            // sleeping with second precision
    
    while (!_quit)
    {
        fd_set  rs, ws;
        fillSets(&rs, &ws);
        timeout.tv_sec = sleep_time;
        res = select(_max_fd + 1, &rs, &ws, 0, &timeout);
        if (res < 0) {
            if (errno == EINTR)
                continue;
            throw std::runtime_error("Select error: " + std::string(strerror(errno)));
        }
        getDeadline(&deadlineReq, &deadlineKA);
        sleep_time = _timeout_idle;
        if (res == 0) {
            flushLoggerBuffers();
            for (int i = 0; i <= _max_fd; ++i) {
                if (!_fds[i])
                    continue;
                _fds[i]->checkTimeout(deadlineReq, deadlineKA, sleep_time);
            }
        }
        else {
            for (int i = 0; i <= _max_fd; ++i) {
                if (!_fds[i] || _fds[i]->checkTimeout(deadlineReq, deadlineKA, sleep_time))
                    continue;
                bool r = FD_ISSET(i, &rs);
                bool w = FD_ISSET(i, &ws);
                if (r | w)
                    _fds[i]->handle(r, w);
            }
        }
    }
}