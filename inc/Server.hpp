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
	std::string &							_ip;
	const int 								_port;
	Webserv *								_master;
	std::vector<VirtServer *>           	_virt_servers;
	std::map<std::string, VirtServer *>		_host_map;
	std::map<int, Client *>					_clients;
	struct timeval  						_last_sessions_cleaning;

	Server(std::string & ip, const int port, Webserv * master, int fd, \
			std::map<std::string, VirtServer *> & host_map, \
			std::vector<VirtServer *> virt_servers);

public:
	~Server(void);
	static Server * create(std::string & ip, const int port, Webserv * master,\
							std::map<std::string, VirtServer *> & host_map, \
							std::vector<VirtServer *> virt_servers);
	void removeClient(Client * c);
	void sendErrMsg(std::string const & msg);
	VirtServer * getVirtualServ(std::string const & serv_name);
	void removeHandler(AFdHandler * h);
	void addHandler(AFdHandler * h);
	void removeAllClients(void);
	std::string const & getIP(void) const;
	int getPort(void) const;
	static void clean_after_error(std::vector<VirtServer *> vec, std::string error_msg);

private:
	virtual bool wantRead(void) const { return true; }
    virtual bool wantWrite(void) const { return false; }
	virtual void handle(void);
	virtual bool checkTimeout(struct timeval & cur_time);

};

#endif
