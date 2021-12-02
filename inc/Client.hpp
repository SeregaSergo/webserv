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

#include "Response.hpp"
#include "Request.hpp"
#include "AFdHandler.hpp"
#include "Webserv.hpp"
#include "constants.hpp"

class Server;
class Request;
class Response;

class Client : public AFdHandler {

private:
	Server *        _master_serv;
	std::string	    _addr;
	int			    _port;
    int             _state;
    struct timeval  _timer;     // this is for timout_idle

    Request         _request;
    Response        _response;

    Client(int fd, struct sockaddr_in const & addr, Server * serv);

public:
    static Client * create(int fd, struct sockaddr_in const & addr, Server * serv);
    Client(Client const & src);
    ~Client();

    friend class Response;

protected:
    virtual bool wantRead() const;
    virtual bool wantWrite() const;
    virtual void handle(bool r, bool w);
    virtual bool checkTimeout(struct timeval & cur_time);

private:
    int recieveData(void);
    int freeSpace(void);
    std::string getAddress(void);
};

#endif
