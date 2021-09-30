#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Location.hpp"
#include "Client.hpp"
#include "Webserv.hpp"

// Server creates new clients, process client's requests of
// choosing locations, delete clients at their requests.
// Locations are stored at map structure, where:
// key = server_name,
// value = pointer to vector of locations associated with server_name;
// There is also server_name = "" for default server.

class Server : public AFdHandler {

private:
	Logger *                            			_err_log;
	Logger *                            			_acc_log;
	EventSelector *									_selector;
	std::map<std::string, std::vector<Location> * >	_locations;
	std::list<Client*>								_clients;
	std::map<std::string, std::string> *			_mime_types;

	Server(Webserv * master, int fd);

public:
	~Server();
	static Server * create(std::string & host, std::string & port, Webserv * master);
	bool addLocation(std::vector<std::string> & serv_names, std::vector<Location> * location);
	void removeClient(Client * c);
	void sendErrMsg(std::string * msg);
	void sendAccMsg(std::string * msg);
	const Location & chooseLocation(Request & req);

private:
	virtual void handle(bool r, bool w);

};

#endif
