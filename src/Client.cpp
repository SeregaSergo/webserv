#include "../inc/Client.hpp"
#include <iostream>     // std::cout
#include <sstream>

Client::Client(int fd, struct sockaddr_in const & addr, Server * serv)
    : AFdHandler(fd)
    , _master_serv(serv)
    , _addr(inet_ntoa(addr.sin_addr))
    , _port(ntohs(addr.sin_port))
    , _state(client::State::reading)
    , _request(serv)
{
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, &constants::ka_time, sizeof(constants::ka_time));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &constants::ka_probes, sizeof(constants::ka_probes));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &constants::ka_interval, sizeof(constants::ka_interval));
    
    gettimeofday(&_timer, NULL);

    std::cout << _fd << ") Client connected(" << _addr << ":" << _port << ") time: " << _time.tv_sec << std::endl;
}

Client * Client::create(int fd, struct sockaddr_in const & addr, Server * serv)
{
    return (new Client(fd, addr, serv));
}

Client::Client(Client const & src)
    :  AFdHandler(src._fd)
    , _master_serv(src._master_serv)
    , _addr(src._addr)
    , _port(src._port)
    , _state(src._state)
    , _request(src._request)
{}

Client::~Client(void)
{
    struct timeval  tm;
    gettimeofday(&tm, NULL);
    std::cout << _fd << ") Client deleted(" << _addr << ":" << _port << ") time: " << tm.tv_sec << std::endl;
}

bool Client::wantRead() const
{
    if (_state == client::State::reading || _state == client::State::waitingForReq)
        return true;
    return false;
}

bool Client::wantWrite() const
{
    if (_state == client::State::writing)
        return true;
    return false;
}

void Client::handle(bool r, bool w)
{
    if (r)
    {
        std::cout << _fd << ") Client handling reading" << std::endl;
	    int		ret;
        ret = _request.getRequest(_fd);
        std::cout << _request << std::endl;
        switch (ret)
        {
        case request::ReturnCode::error:
            std::cout << "return error" << std::endl; 
            _master_serv->sendErrMsg("Client " + getAddress() + " - recieve error");
            _master_serv->removeClient(this);
            return;

        case request::ReturnCode::disconnected:
            std::cout << "disconnected" << std::endl; 
            _master_serv->removeClient(this);
            return;
        
        case request::ReturnCode::unfinished:
            std::cout << "unfinished" << std::endl;
            break;

        case request::ReturnCode::completed:
            std::cout << "complited" << std::endl;
            // _response(_request);
            _state = client::State::writing;
            break;
        }
        gettimeofday(&_timer, NULL);
    }
    else if (w)
    {
        std::cout << _fd << ") Client handling writting" << std::endl;
        std::stringstream response;
        std::string body("<html>\n<head>\n<title>Test upload</title>\n</head>\n<body>\n<h2>Select files to upload</h2>\
        \n<form name=\"upload\" method=\"POST\" enctype=\"multipart/form-data\" action=\"/upload\">\n<input type=\"file\" name=\"file1\"><br>\
        <input type=\"file\" name=\"file2\"><br>\n<input type=\"submit\" name=\"submit\" value=\"Upload\">\n<input type=\"hidden\" name=\"test\" value=\"value\">\
        </form>\n</body>\n</html>");
        response << "HTTP/1.1 " << _request.getStatusCode() << " OK\r\n"
            << "Version: HTTP/1.1\r\n"
            << "Content-Type: text/html; charset=utf-8\r\n"
            << "Content-Length: " << body.length()
            << "\r\n\r\n"
            << body;

        // Отправляем ответ клиенту с помощью функции send
        if (_state == client::State::writing)
        {
            send(_fd, response.str().c_str(), response.str().length(), 0);
            std::cout << response.str() << std::endl;
        }
        _state = client::State::waitingForReq;
        _request.clear();
        // shutdown(_fd, SHUT_RDWR);
    }
}

std::string Client::getAddress(void)
{
    std::stringstream stream;

    stream << _addr << ":" << _port;
    return (stream.str());
}

// Delete client if the client takes no action during <timeout_idle> time
// or keepalive session expired and there is no request.
bool Client::checkTimeout(struct timeval & cur_time)
{
    if (((_state == client::State::reading || _state == client::State::writing) \
            && (cur_time.tv_sec - _timer.tv_sec) >= constants::timeout_idle) \
            || (_state == client::State::waitingForReq \
            && (cur_time.tv_sec - _time.tv_sec) >= constants::timeout_ka)
            || _state == client::State::shutdown)
    {
        _master_serv->removeClient(this);
        return (true);
    }
    return (false);
}