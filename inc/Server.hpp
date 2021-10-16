#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <list>
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
	std::list<Client>								_clients;

	Server(Webserv * master, int fd);

public:
	~Server();
	static Server * create(std::string & host, std::string & port, Webserv * master);
	bool addLocation(std::vector<std::string> & serv_names, std::vector<Location> * location);
	void removeClient(Client * c);
	void sendErrMsg(std::string * msg);
	const std::string & getMimeType(std::string & ext);
	const Location & chooseLocation(Request & req);
	void flushLoggerBuffers(void);
	
private:
	virtual void handle(bool r, bool w);

};

#endif
