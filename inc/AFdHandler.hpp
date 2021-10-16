#ifndef AFDHANDLER_HPP
#define AFDHANDLER_HPP

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

// Absract parent class for Server, Client and Logger classes
class AFdHandler {

private:
    int     _fd;

public:
    AFdHandler(int fd) : _fd(fd) {fcntl(fd, F_SETFL, O_NONBLOCK);}
    virtual ~AFdHandler() { close(_fd); }
    virtual void handle(bool r, bool w) = 0;
    int getFd() const { return _fd; }
    virtual bool wantRead() const { return true; }
    virtual bool wantWrite() const { return false; }
    virtual bool checkTimeout(struct timeval & deadlineReq, struct timeval & deadlineKA, time_t & sleep_time) {return false;};
};

#endif