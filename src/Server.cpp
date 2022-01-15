#include "../inc/Server.hpp"

Server::Server(std::string & ip, const int port, Webserv * master, int fd, std::map<std::string, VirtServer *> & host_map, \
                std::vector<VirtServer *> virt_servers)
		: AFdHandler(fd)
        , _ip(ip)
        , _port(port)
        , _master(master)
        , _virt_servers(virt_servers)
        , _host_map(host_map)
{
    gettimeofday(&_last_sessions_cleaning, NULL);
}

Server::~Server(void) {
    removeAllClients();
    for (std::vector<VirtServer *>::iterator it = _virt_servers.begin(); it != _virt_servers.end(); ++it)
        delete *it;
}

Server * Server::create(std::string & ip, const int port, Webserv * master, \
                        std::map<std::string, VirtServer *> & host_map, \
                        std::vector<VirtServer *> virt_servers)
{
    struct sockaddr_in  addr;
    std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
    if (port < 0)
        throw std::runtime_error("Invalid port: -1");
    if (ip.empty())
	    addr.sin_addr.s_addr = INADDR_ANY;
	else 
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
        throw std::runtime_error("Could not create socket.");
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	    throw std::runtime_error("Could not bind port: " + numToStr(port));
	if (listen(fd, 1024) == -1)
	    throw std::runtime_error("Port could not listen.");
    Server * serv = new Server(ip, port, master, fd, host_map, virt_servers);
    master->addHandler(serv);
    return (serv);
}

void Server::handle()
{
    int                 connfd;
    struct sockaddr_in  addr;
    socklen_t addrlen = sizeof(addr);

    DISPLAY(std::cout << "[fd "<< _fd << "] Server accepting" << std::endl);
	connfd = accept(_fd, (struct sockaddr *)&addr, &addrlen);
	if (connfd == -1)
    {
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

void Server::removeHandler(AFdHandler * h)
{
    _master->removeHandler(h);
}

void Server::addHandler(AFdHandler * h)
{
    _master->addHandler(h);
}

void Server::sendErrMsg(std::string const & msg) {
    _master->sendErrMsg(msg);
}

void Server::removeAllClients(void)
{
    while(!_clients.empty())
        removeClient(_clients.begin()->second);
}

VirtServer * Server::getVirtualServ(std::string const & serv_name)
{
    size_t  pos = serv_name.find(':');
    std::map<std::string, VirtServer *>::iterator it = _host_map.find(serv_name.substr(0, pos));
    if (it == _host_map.end())
        it =_host_map.find("");
    return ((*it).second);
}

std::string const & Server::getIP(void) const {
    return (_ip);
}

int Server::getPort(void) const {
    return (_port);
}

bool Server::checkTimeout(struct timeval & cur_time)
{
    if (cur_time.tv_sec - _last_sessions_cleaning.tv_sec > constants::timeout_sid_cleaning)
    {
        for (std::vector<VirtServer *>::iterator it = _virt_servers.begin(); it != _virt_servers.end(); ++it)
            (*it)->cleanSessions(cur_time.tv_sec);
        _last_sessions_cleaning = cur_time;
    }
    return false;
}