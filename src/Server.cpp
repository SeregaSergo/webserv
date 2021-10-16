#include "../inc/Server.hpp"

Server & Server::operator=(Server const & src){

}

Server::~Server(){}

void Server::flushLoggerBuffers(void)
{
    for (std::map<std::string, VirtServer *>::iterator it = _virt_servers.begin(); it != _virt_servers.end(); ++it)
        (*it).second->flushLoggerBuffers();
}