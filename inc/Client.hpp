#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include "/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/usr/include/netinet/tcp.h"
#include "Request.hpp"
#include "Server.hpp"
#include "AFdHandler.hpp"

class Server;

class Client : public AFdHandler {

private:
	Server *    _master_serv;
	std::string	_addr;
	int			_port;

public:
    Client(int fd, struct sockaddr_in & addr);
    Client(Client const & src);

protected:
    virtual bool wantRead() const;
    virtual bool wantWrite() const;
    virtual void handle(bool r, bool w);
    virtual bool checkTimeout(struct timeval & cur_time);
};

#endif
