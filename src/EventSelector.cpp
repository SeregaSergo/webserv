#include "../inc/EventSelector.hpp"

void EventSelector::addHandler(AFdHandler * h)
{
    int fd = h->getFd();
    if (fd > _max_fd)
    {
        _fds.insert(_fds.end(), _max_fd - fd, 0);
        _max_fd = fd;
    }
    _fds[fd] = h;
}

bool EventSelector::removeHandler(AFdHandler * h)
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

void EventSelector::fillSets(fd_set * rs, fd_set * ws)
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

void EventSelector::getDeadline(struct timeval * deadline)
{
    gettimeofday(deadline, NULL);
    deadline->tv_sec -= _timeout;
}

void EventSelector::start(void)
{
    int             res;
    struct timeval  timeout;    // for select timeout
    struct timeval  deadline;   // for deleteing clients
    time_t          sleep_time = _timeout;

    timeout.tv_usec = 0;        // sleeping with second precision
    
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
        getDeadline(&deadline);
        if (res == 0) {
            for (int i = 0; i <= _max_fd; ++i) {
                if (!_fds[i])
                    continue;
                _fds[i]->checkTimeout(&deadline, sleep_time);
            }
        }
        else {
            for (int i = 0; i <= _max_fd; ++i) {
                if (!_fds[i] || _fds[i]->checkTimeout(&deadline, sleep_time))
                    continue;
                bool r = FD_ISSET(i, &rs);
                bool w = FD_ISSET(i, &ws);
                if (r | w)
                    _fds[i]->handle(r, w);
            }
        }
    }
}