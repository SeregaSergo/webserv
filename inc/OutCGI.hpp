#include "Response.hpp"

#ifndef OUTCGI_HPP
#define OUTCGI_HPP

#include "AFdHandler.hpp"

class Response;

namespace constants
{
    const size_t  CGI_bufsize = 4096;
}

// This class gets data from CGI process. It reads CGI response 
// from one side of the pipe that emulates stdout of CGI process.
// When it's done it calls callback function of Response class
// for it's deletion and post processing of CGI response.

class OutCGI : public AFdHandler {

private:
    typedef  void (Response::*Callback)(int);

    std::stringstream & _stream;
    char                _buffer[constants::CGI_bufsize];
    int                 _timeout;
    Response &          _resp;
    Callback            _callback_func;

public:
    OutCGI(int fd, int timeout, std::stringstream & stream, Response & resp, void (Response::*f)(int))
    : AFdHandler(fd)
    , _stream(stream)
    , _timeout(timeout)
    , _resp(resp)
    , _callback_func(f)
    {}

    virtual ~OutCGI(void) { DISPLAY(std::cout << "[fd " << _fd << "] OutCGI deleted" << std::endl); }

    virtual bool wantRead(void) const { return true; }
    virtual bool wantWrite(void) const { return false; }

protected:
    virtual void handle(void);
    virtual bool checkTimeout(struct timeval & cur_time);
};

#endif