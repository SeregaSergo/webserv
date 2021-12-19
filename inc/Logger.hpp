#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sys/time.h>
#include "AFdHandler.hpp"

namespace constants
{
    const int loggerLimit = 2048;
    const time_t loggerTimeout = 5;
}

// Absract parent class for Server, Client and Logger classes
class Logger : public AFdHandler {

private:
    std::string         _buffer;
    bool                _write;

    const char * getDateTime(void);
    
public:
    Logger(int fd) : AFdHandler(fd), _write(false) {}
    virtual ~Logger(void) {}
    virtual bool wantRead(void) const { return false; }
    virtual bool wantWrite(void) const { return _write; }
    void sendMsg(std::string const & msg);

protected:
    virtual void handle(void);
    virtual bool checkTimeout(struct timeval & cur_time);
};

#endif