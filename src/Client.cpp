#include "../inc/Client.hpp"

Client::Client(int fd, struct sockaddr_in const & addr, Server * serv)
    : AFdHandler(fd)
    , _master_serv(serv)
    , _addr(inet_ntoa(addr.sin_addr))
    , _port(ntohs(addr.sin_port))
    , _state(client::State::reading)
    , _request(serv)
    , _response(&_request, this)
{
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, &constants::ka_time, sizeof(constants::ka_time));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &constants::ka_probes, sizeof(constants::ka_probes));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &constants::ka_interval, sizeof(constants::ka_interval));
    
    gettimeofday(&_timer, NULL);

    DISPLAY(std::cout << "[fd " << _fd << "] Client connected(" << _addr << ":" << _port << ") time: " << _time.tv_sec << std::endl);
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
    , _response(src._response)
{}

Client::~Client(void)
{
    struct timeval  tm;
    gettimeofday(&tm, NULL);
    DISPLAY(std::cout << "[fd " << _fd << "] Client deleted(" << _addr << ":" << _port << ") time: " << tm.tv_sec << std::endl);
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

void Client::handle(void)
{
    if (_state == client::State::writing)
    {
        DISPLAY(std::cout << "[fd " << _fd << "] Client writting" << std::endl);

        switch (_response.sendResponse(_fd))
        {
        case response::ReturnCode::error:
            _master_serv->sendErrMsg("Client " + getAddress() + " - send error/disconnected");
            _master_serv->removeClient(this);
            return;
        
        case response::ReturnCode::unfinished:
            gettimeofday(&_timer, NULL);
            break;

        case response::ReturnCode::completed:
            sendLogMessage();
            if (_request.isLastRequest())
            {
                shutdown(_fd, SHUT_RDWR);
                _state = client::State::shutdown;
            }
            else
            {
                _request.clear();
                _response.clear();
                _state = client::State::waitingForReq;
            }
            break;
        }
    }
    else
    {
        DISPLAY(std::cout << "[fd "<< _fd << "] Client reading" << std::endl);

        switch (_request.getRequest(_fd))
        {
        case request::ReturnCode::error:
            _master_serv->sendErrMsg("Client " + getAddress() + " - recieve error");
            _master_serv->removeClient(this);
            return;

        case request::ReturnCode::disconnected:
            std::cout << "client disconnected" << std::endl;
            _master_serv->removeClient(this);
            return;
        
        case request::ReturnCode::unfinished:
            gettimeofday(&_timer, NULL);
            break;

        case request::ReturnCode::completed:
            _state = client::State::processing;
            _response.processRequest();
            break;
        }
    }
}

void Client::sendLogMessage(void)
{
    std::string msg;
    msg += getAddress();
    msg += " \"" + _request.getMethod();
    msg += ' ' + _request.getURI();
    msg += _request.getQuery() + "\" ";
    msg += numToStr(_response.getStatusCode()) + " ";
    msg += numToStr(_response.getBytesSent()) + "B";
    _request.getVirtualServer()->sendAccMsg(msg);

    DISPLAY(std::cout << _request << std::endl);
    DISPLAY(std::cout << _response << std::endl);
}

std::string Client::getAddress(void)
{
    std::stringstream stream;

    stream << _addr << ":" << _port;
    return (stream.str());
}

void Client::setState(int state) {
    _state = state;
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