#ifndef AFDHANDLER_HPP
#define AFDHANDLER_HPP

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

// Absract parent class for
// Server class
// Client class
// Logger class
// File class
// CGI class
class AFdHandler {

protected:
    int             _fd;
    struct timeval  _time;

public:
    AFdHandler(int fd) : _fd(fd) {
        fcntl(fd, F_SETFL, O_NONBLOCK);
        gettimeofday(&_time, NULL);
    }
    virtual ~AFdHandler() { close(_fd); }
    int getFd() const { return _fd; }
    virtual bool wantRead() const = 0;
    virtual bool wantWrite() const = 0;
    virtual void handle(bool r, bool w) = 0;
    virtual bool checkTimeout(struct timeval & cur_time) = 0;
};

#endif