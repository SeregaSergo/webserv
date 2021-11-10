#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/resource.h>

#ifdef LINUX_COMPILATION
#define TCP_KEEPALIVE TCP_KEEPIDLE
#endif

// #ifdef MACOS_COMPILATION
//     #include "/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/usr/include/netinet/tcp.h"
// #endif

#include "Request.hpp"
#include "AFdHandler.hpp"
#include "Webserv.hpp"

class Server;

namespace clientState
{
    const int     reading = 1;
    const int     writing = 2;
    const int     waitingForReq = 3;
}

class Client : public AFdHandler {

private:
	Server *        _master_serv;
	std::string	    _addr;
	int			    _port;
    int             _state;
    struct timeval  _timer;     // this is for timout_idle
    char	        _buffer[1024];
    Client(int fd, struct sockaddr_in const & addr, Server * serv);

public:
    static Client * create(int fd, struct sockaddr_in const & addr, Server * serv);
    Client(Client const & src);
    ~Client();

protected:
    virtual bool wantRead() const;
    virtual bool wantWrite() const;
    virtual void handle(bool r, bool w);
    virtual bool checkTimeout(struct timeval & cur_time);
};

#endif
