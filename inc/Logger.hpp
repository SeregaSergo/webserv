#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <vector>
#include "AFdHandler.hpp"

// Absract parent class for Server, Client and Logger classes
class Logger : public AFdHandler {

private:
    std::vector<std::string *>  _msg_stack;
    bool                        _write;

public:
    Logger(int fd) : AFdHandler(fd), _write(false) {}
    virtual ~Logger() {}
    virtual bool wantRead() const { return false; }
    virtual bool wantWrite() const { return _write; }
    void writeMsg(std::string * msg);

protected:
    virtual void handle(bool r, bool w);
};

#endif