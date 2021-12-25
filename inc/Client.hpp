#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/resource.h>

#include "Response.hpp"
#include "Request.hpp"
#include "AFdHandler.hpp"
#include "Webserv.hpp"

#ifdef LINUX_COMPILATION
#define TCP_KEEPALIVE TCP_KEEPIDLE
#endif

// This class represent socket connected to client.
// It gets a request, processes and send back a response. 

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
    int recieveData(void);
    int freeSpace(void);
    std::string getAddress(void);
    void sendLogMessage(void);

public:
    static Client * create(int fd, struct sockaddr_in const & addr, Server * serv);
    Client(Client const & src);
    ~Client(void);

protected:
    virtual bool wantRead(void) const;
    virtual bool wantWrite(void) const;
    virtual void handle(void);
    virtual bool checkTimeout(struct timeval & cur_time);

};

#endif
