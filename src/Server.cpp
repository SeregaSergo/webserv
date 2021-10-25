#include "../inc/Server.hpp"

Server::Server(Webserv * master, int fd, std::map<std::string, VirtServer *> & virt_servers)
		: AFdHandler(fd)
        , _master(master) {}

Server * Server::create(std::string & host, const int port, Webserv * master, std::map<std::string, VirtServer *> & virt_servers)
{
    struct sockaddr_in  addr;
    std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
    if (host.empty())
	    addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	else
        addr.sin_addr.s_addr = inet_addr(host.c_str());
	addr.sin_port = htons(port);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
        throw std::runtime_error("Could not create socket.");
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	    throw std::runtime_error("Could not bind port.");
	if (listen(fd, 1024) == -1)
	    throw std::runtime_error("Port could not listen.");
	
    Server * serv = new Server(master, fd, virt_servers);
    master->addHandler(serv);
    return (serv);
}

void Server::handle(bool r, bool w)
{
    int                 connfd;
    struct sockaddr_in  addr;

	connfd = accept(_fd, (struct sockaddr *)&addr, NULL);
	if (connfd == -1)
    {
        std::string er("Error of accepting connection: ");
        er += strerror(errno);
		_master->sendErrMsg(er);
    }
	else
	{
		_clients.insert(std::make_pair(connfd, Client(connfd, addr)));
        _master->addHandler(&_clients[connfd]);
	}
}

void Server::removeClient(Client * c)
{
    int fd = c->getFd();
    _master->removeHandler(&_clients[fd]);
    _clients.erase(fd);
}

void Server::sendErrMsg(std::string & msg) {
    _master->sendErrMsg(msg);
}

const VirtServer & Server::getVirtualServ(std::string const & serv_name)
{
    std::map<std::string, VirtServer *>::iterator it = _virt_servers.find(serv_name);
    if (it == _virt_servers.end())
        it =_virt_servers.find("");
    return (*(*it).second);
}

const std::string & Server::getMimeType(std::string & ext) {
    return (_master->getMimeType(ext));
}