#ifndef AFDHANDLER_HPP
#define AFDHANDLER_HPP

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

// Macros for debug printing
#ifdef DEBUG_FLAG
    #define DEBUG(x) x
#else 
    #define DEBUG(x)
#endif

// Macros for printing information to show how the program works 
#ifdef DISPLAY_FLAG
    #define DISPLAY(x) x
#else 
    #define DISPLAY(x)
#endif

// This is an absract parent class for all classes that represent FD:
//      Server class (listening socket)
//      Client class (recv/send socket)
//      Logger class (log file)
//      InCGI  class (input pipe of CGI process)
//      OutCGI class (output pipe of CGI process)

class AFdHandler {

protected:
    int             _fd;
    struct timeval  _time;

    AFdHandler(int fd) : _fd(fd) {
        fcntl(fd, F_SETFL, O_NONBLOCK);
        gettimeofday(&_time, NULL);
    }
    
public:
    virtual ~AFdHandler() { close(_fd); }
    int getFd() const { return _fd; }
    virtual bool wantRead() const = 0;
    virtual bool wantWrite() const = 0;
    virtual void handle(void) = 0;
    virtual bool checkTimeout(struct timeval & cur_time) = 0;
};

#endif