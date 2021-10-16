#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
// #include <sys/time.h>
#include "AFdHandler.hpp"

namespace constants
{
    int loggerBuffer = 2048;
    time_t loggerTimeout = 5;
}

// Absract parent class for Server, Client and Logger classes
class Logger : public AFdHandler {

private:
    std::string         _buffer;
    bool                _write;
    struct timeval      _next_recording;

public:
    Logger(int fd);
    virtual ~Logger() {}
    virtual bool wantRead() const { return false; }
    virtual bool wantWrite() const { return _write; }
    void writeMsg(std::string & msg);
    void flushBuf(void);

protected:
    virtual void handle(bool r, bool w);
};

#endif