#include "../inc/Client.hpp"

Client::Client(int fd, struct sockaddr_in & addr)
    : AFdHandler(fd)
    , _addr(inet_ntoa(addr.sin_addr)
    , ntohs(addr.sin_port))
{
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, &constants::ka_time, sizeof(constants::ka_time));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &constants::ka_probes, sizeof(constants::ka_probes));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &constants::ka_interval, sizeof(constants::ka_interval));
}