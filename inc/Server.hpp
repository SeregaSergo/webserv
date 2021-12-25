#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"
#include "AFdHandler.hpp"

class Webserv;
class VirtServer;
class AFdHandler;
class Client;

// This class represent server's listening socket. It creates new clients,
// process client's requests of choosing virtual servers, delete clients at their requests.
// Virtual servers are stored at map structure, where:
// key = server_name,
// value = pointer to virtual server;
// There is also server_name = "" for default virtual server.
// BE CAREFUL WITH THE DISTRUCTOR!!!

class Server : public AFdHandler {

private:
	Webserv *										_master;
	std::map<std::string, VirtServer *>				_virt_servers;
	std::map<int, Client *>							_clients;

	Server(Webserv * master, int fd, std::map<std::string, VirtServer *> & virt_servers);

public:
	~Server(void);
	static Server * create(std::string & host, const int port, Webserv * master,\
							std::map<std::string, VirtServer *> & virt_servers);
	void removeClient(Client * c);
	void sendErrMsg(std::string const & msg);
	VirtServer * getVirtualServ(std::string const & serv_name);
	void removeHandler(AFdHandler * h);
	void addHandler(AFdHandler * h);
	void removeAllClients(void);

private:
	virtual bool wantRead(void) const { return true; }
    virtual bool wantWrite(void) const { return false; }
	virtual void handle(void);
	virtual bool checkTimeout(struct timeval & cur_time)
	{
		(void)cur_time;
		return false;
	}
};

#endif
