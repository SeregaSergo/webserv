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

    const std::string & getDateTime(void);
    
public:
    Logger(int fd) : AFdHandler(fd), _write(false) {}
    virtual ~Logger() {}
    virtual bool wantRead() const { return false; }
    virtual bool wantWrite() const { return _write; }
    void sendMsg(std::string & msg);

protected:
    virtual void handle(bool r, bool w);
    virtual bool checkTimeout(struct timeval & cur_time);
};

#endif