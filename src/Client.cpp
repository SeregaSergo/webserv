#include "../inc/Client.hpp"

Client::Client(int fd, struct sockaddr_in & addr)
    : AFdHandler(fd)
    , _addr(inet_ntoa(addr.sin_addr)
    , ntohs(addr.sin_port))
    , _state(clientState::reading)
{
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, &constants::ka_time, sizeof(constants::ka_time));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &constants::ka_probes, sizeof(constants::ka_probes));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &constants::ka_interval, sizeof(constants::ka_interval));
    gettimeofday(&_timer, NULL);
}

Client::Client(Client const & src)
    :  AFdHandler(src._fd)
    , _master_serv(src._master_serv)
    , _addr(src._addr)
    , _port(src._port)
    , _state(src._state)
{}

bool Client::wantRead() const
{
    if (_state == clientState::reading || _state == clientState::waitingForReq)
        return true;
    return false;
}

bool Client::wantWrite() const
{
    if (_state == clientState::writing)
        return true;
    return false;
}

void Client::handle(bool r, bool w)
{
    (void)r;
    (void)w;
}

// Delete client if the client takes no action during <timeout_idle> time
// or keepalive session expired and there is no request.
bool Client::checkTimeout(struct timeval & cur_time)
{
    if (((_state == clientState::reading || _state == clientState::writing) \
            && (cur_time.tv_sec - _timer.tv_sec) > constants::timeout_idle) \
            || (_state == clientState::waitingForReq \
            && (cur_time.tv_sec - _timer.tv_sec) > constants::timeout_ka))
    {
        _master_serv->removeClient(this);
        return (true);
    }
    return (false);
}