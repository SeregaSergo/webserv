#include "Server.hpp"

Server::Server(){}

Server & Server::operator=(Server const & src){

	this->server_names = src.server_names;
	this->locations = src.locations;
	return *this;
}


Server::Server(Server const & src){

	*this = src;
}

void	Server::addServerName(std::string server_name){
	this->server_names.push_back(server_name);
}

Server::~Server(){}

std::vector<std::string>	Server::getServerNames(){
	return this->server_names;
}


std::vector<Location>	Server::getLocations(){
	
	return this->locations;
}

