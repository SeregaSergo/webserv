#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <map>
#include "Location.hpp"
#include "Socket.hpp"
#include "Client.hpp"

class Server {

private:
	std::vector<std::string>			_serv_names;
	char *								_host;
	unsigned short int					_port;
	std::map<unsigned int, std::string>	_error_pages;
	int									_limit_body_size;
	std::vector<Location>				_locations;
	Socket *							_socket;
	std::vector<Client*>				_clients;

public:
	Server();
	Server(Server const & src);
	~Server();
	Server & operator=(Server const & src);
	
};

#endif
