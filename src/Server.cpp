#include "../inc/Server.hpp"

Server::Server(Webserv * master, int fd, std::map<std::string, VirtServer *> & virt_servers)
		: AFdHandler(fd)
        , _master(master)
        , _virt_servers(virt_servers) {}

Server::~Server(void)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete (*it).second;
}

Server * Server::create(std::string & host, const int port, Webserv * master, std::map<std::string, VirtServer *> & virt_servers)
{
    struct sockaddr_in  addr;
    std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
    if (host.empty())
	    addr.sin_addr.s_addr = INADDR_ANY;
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
    socklen_t addrlen = sizeof(addr);

    (void)r;
    (void)w;
    std::cout << _fd << ") Server handling" << std::endl;
	connfd = accept(_fd, (struct sockaddr *)&addr, &addrlen);
	if (connfd == -1)
    {
        std::cout << "Error: " << strerror(errno) << std::endl;
        std::string er("Error of accepting connection: ");
        er += strerror(errno);
		_master->sendErrMsg(er);
    }
	else
	{
        Client * cl_ptr = Client::create(connfd, addr, this);
        _master->addHandler(cl_ptr);
		_clients.insert(std::make_pair(connfd, cl_ptr));
	}
}

void Server::removeClient(Client * c)
{
    int fd = c->getFd();
    _master->removeHandler(c);
    _clients.erase(fd);
    delete c;
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