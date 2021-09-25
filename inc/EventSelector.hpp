#ifndef EVENTSELECTOR_HPP
#define EVENTSELECTOR_HPP

#include <iostream>
#include <vector>
#include <sys/select.h>
#include "AFdHandler.hpp"
#include <stdexcept>
#include <string.h>

class EventSelector {

private:
    std::vector<AFdHandler*>    _fds;
    int                         _max_fd;
    bool                        _quit;
    time_t                      _timeout;

    void fillSets(fd_set * rs, fd_set *ws);
    void getDeadline(struct timeval * deadline);
    
public:
    EventSelector(time_t timeout)
        : _max_fd(-1), _quit(false), _timeout(timeout) {}
    ~EventSelector() {}
    void addHandler(AFdHandler * h);
    bool removeHandler(AFdHandler * h);
    void start();
    void stop() { _quit = true; }   
};

#endif