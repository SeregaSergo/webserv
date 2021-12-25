#include "Response.hpp"

#ifndef INCGI_HPP
#define INCGI_HPP

#include "AFdHandler.hpp"

class Response;

// This class puts data into CGI process.
// It takes data from request body and writes into
// one side of the pipe that emulates stdin of CGI process.
// When it's done it calls callback function of Response class
// for it's deletion.

class InCGI : public AFdHandler {

private:
    typedef  void (Response::*Callback)(int);

    const std::string & _body;
    size_t              _sent;
    int                 _timeout;
    Response &          _resp;
    Callback            _callback_func;

public:
    InCGI(int fd, int timeout, const std::string & body, Response & resp, void (Response::*f)(int))
    : AFdHandler(fd)
    , _body(body)
    , _sent(0)
    , _timeout(timeout)
    , _resp(resp)
    , _callback_func(f)
    {}

    virtual ~InCGI(void) {DISPLAY(std::cout << "[fd " << _fd << "] InCGI deleted" << std::endl); }

    virtual bool wantRead(void) const { return false; }
    virtual bool wantWrite(void) const { return true; }

protected:
    virtual void handle(void);
    virtual bool checkTimeout(struct timeval & cur_time);
};

#endif