#include "../inc/Client.hpp"
#include <iostream>     // std::cout
#include <sstream>

Client::Client(int fd, struct sockaddr_in const & addr, Server * serv)
    : AFdHandler(fd)
    , _master_serv(serv)
    , _addr(inet_ntoa(addr.sin_addr))
    , _port(ntohs(addr.sin_port))
    , _state(client::State::reading)
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
        ret = _req.getRequest(_fd);
        if (ret <= 0)
	    {
            _master_serv->removeClient(this);
            if (ret == -1)
                _master_serv->sendErrMsg("Client " + getAddress() + " - recieve error");
            return;
        }
        gettimeofday(&_timer, NULL);
	    if (ret == request::ReturnCode::completed)
            _state = client::State::writing;
    }
    else if (w)
    {
        // std::cout << _fd << ") Client handling writting" << std::endl;
        // std::stringstream response;
        // std::string str("Request-URI Too Long");

        // response << "HTTP/1.1 414 Request-URI Too Long\r\n"
        //     << "Version: HTTP/1.1\r\n"
        //     << "Content-Type: text/html; charset=utf-8\r\n"
        //     << "Content-Length: " << str.length()
        //     << "\r\n\r\n"
        //     << str;

        // // Отправляем ответ клиенту с помощью функции send
        // if (_state == clientState::writing)
        //     send(_fd, response.str().c_str(),
        //         response.str().length(), 0);
        
        // _state = clientState::waitingForReq;
        // // shutdown(_fd, SHUT_RDWR);
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