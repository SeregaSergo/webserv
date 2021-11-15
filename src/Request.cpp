#include "../inc/Request.hpp"

Request::Request(void)
    : _uri_type(request::TypeURI::none)
    , _state(request::State::method)
{
    _buffer = new char[constants::incoming_buffer];
    _ptr = _buffer;
    _end = _buffer + constants::incoming_buffer;
    _head = _buffer;
    _tail = _buffer;
}

Request::~Request(void) {
    delete _buffer;
}


inline int Request::freeSpace(void)
{
    return ((_end - _ptr) / sizeof(char));
}

inline int Request::filledSpace(void)
{
    return ((_ptr - _buffer) / sizeof(char));
}

inline int Request::passWhitespaces(void)
{
    while (_tail != _ptr)
    {
        if (*_tail == ' ')
            ++_tail;
        else
            return (1);
    }
    return (0);
}

inline int Request::findNextWhitespace(void)
{
    while (_tail != _ptr)
    {
        if (*_tail != ' ')
            ++_tail;
        else
        {
            _head = _tail;
            return (1);
        }
    }
    return (0);
}

inline int Request::findEndOfLine(void)
{
    while (_tail != _ptr)
    {
        if (*_tail != '\r')
            ++_tail;
        else
        {
            _head = _tail;
            return (1);
        }
    }
    return (0);
}

inline bool Request::isVersionSupported(void)
{
    const std::string * end = constants::versions + sizeof(constants::versions);
    for (const std::string * ptr = constants::versions; ptr != end; ++ptr)
        if (_http_version == *ptr)
            return (true);
    return (false);
}

int    Request::getRequest(int socket)
{
    int ret;
    ret = recv(socket, _ptr, freeSpace(), 0);
    if (_state == request::State::body)
    {

    }
    else
    {
        while (parseData());
    }
    _ptr += ret;
    _uri_type = request::TypeURI::none;
    return (ret);
}

// State machine for parsing.
// return:  -1 - Client or Server Error
//           0 - Stop parsing, wait for new data
//           1 - Continue parsing
int Request::parseData(void)
{
    switch (_state)
    {
    case request::State::method:
        if (findNextWhitespace())
        {
            _method = std::string(_head, _tail);
            _state = request::State::URI;
        }
        else
            return (0);
    case request::State::URI:
        if (passWhitespaces() && findNextWhitespace())
        {
            _uri = std::string(_head, _tail);
            _state = request::State::version;
            if ((_tail - _head) / sizeof(char) > constants::limit_request_length)
            {
                _status_code = 414;
                return (-1);
            }
        }
        else
            return (0);
    case request::State::version:
        if (passWhitespaces() && findEndOfLine())
        {
            _http_version = std::string(_head, _tail);
            _state = request::State::headerName;
            if (!isVersionSupported())
            {
                _status_code = 505;
                return (-1);
            }
        }
        else
            return (0);
    case request::State::headerName:
        break;
    }
    return (1);
}