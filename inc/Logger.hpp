#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include "AFdHandler.hpp"

// Absract parent class for Server, Client and Logger classes
class Logger : public AFdHandler {

private:
    bool    _write;

public:
    Logger(int fd) : AFdHandler(fd), _write(false) {}
    virtual ~Logger() {}
    virtual void handle(bool r, bool w) = 0;
    virtual bool wantRead() const { return false; }
    virtual bool wantWrite() const { return _write; }
};

#endif