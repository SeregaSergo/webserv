#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <sys/ioctl.h>
#include <map>
#include "Location.hpp"
#include "Client.hpp"
#include "Webserv.hpp"
#include "VirtServer.hpp"

// Server creates new clients, process client's requests of
// choosing locations, delete clients at their requests.
// Virtual servers are stored at map structure, where:
// key = server_name,
// value = pointer to virtual server;
// There is also server_name = "" for default virtual server.
// WATCH OUT WITH DISTRUCTOR!!!

class Server : public AFdHandler {

private:
	Webserv *										_master;
	std::map<std::string, VirtServer *>				_virt_servers;
	std::map<int, Client>							_clients;

	Server(Webserv * master, int fd, std::map<std::string, VirtServer *> & virt_servers);

public:
	~Server(void) {}
	static Server * create(std::string & host, const int port, Webserv * master,\
							std::map<std::string, VirtServer *> & virt_servers);
	void removeClient(Client * c);
	void sendErrMsg(std::string & msg);
	const VirtServer & getVirtualServ(std::string const & serv_name);
	const std::string & getMimeType(std::string & ext);
	
private:
	virtual bool wantRead() const { return true; }
    virtual bool wantWrite() const { return false; }
	virtual void handle(bool r, bool w);
	virtual bool checkTimeout(struct timeval & cur_time) {return true;}
};

#endif
