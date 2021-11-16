#include "../inc/Request.hpp"

Request::Request(void)
    : _state(request::State::method)
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

// std::string Request::getParams( std::vector< std::pair< std::string, std::string> > requestData )
// {
//     std::string parm = "";

//     std::vector< std::pair< std::string, std::string> >::iterator itr = requestData.begin();

//     for( ; itr != requestData.end(); ++itr )
//     {
//         if( parm.size() < 1 )
//             parm += "";
//         else
//             parm += "&";
//         parm += itr->first + "=" + itr->second;
//     }
//     return parm;
// }

bool Request::getUri(void)
{
    int size = (_tail - _head) / sizeof(char);
    if (*_head == '/')
        _uri = std::string(_head, _tail);
    else if (size < 9 && strcmp(_head, "http://"))
        return (1);
    else
    {
        _head += 7;
        size -= 7;
        int pos = 0;
        while (true)
        {
            if (pos == size)
            {
                _uri = std::string("/");
                break;
            }
            if (_head[pos] == '/')
            {
                _uri = std::string(&_head[pos], _tail);
                break;
            }
            ++pos;
        }
        _headers.insert(std::make_pair("Host", std::string(_head, &_head[pos])));
    }
    return (0);
}

inline bool Request::passSymbols(char c)
{
    while (_tail != _ptr)
    {
        if (*_tail == c)
            ++_tail;
        else
        {
            _head = _tail;
            return (true);
        }
    }
    return (false);
}

inline bool Request::findEndOfWord(char c)
{
    while (_tail != _ptr)
    {
        if (*_tail != c)
            ++_tail;
        else
            return (true);
    }
    return (false);
}

inline bool Request::findEndOfLine(void)
{
    if (*_tail == '\r' && (_tail + 1) != _ptr && _tail[1] == '\n')
    {
        _tail += 2;
        _head = _tail;
        return (true);
    }
    else
        return (false);
}

inline bool Request::isEndOfRequest(void)
{
    if (_tail == _ptr)
        return (true);
    else if (findEndOfLine())
    {
        _state = request::State::body;
        return (true);
    }
    return (false);
}

inline bool Request::isVersionSupported(void)
{
    const std::string * end = constants::versions + sizeof(constants::versions);
    for (const std::string * ptr = constants::versions; ptr != end; ++ptr)
        if (_http_version == (static_cast<const std::string>(*ptr)))
            return (true);
    return (false);
}

int    Request::getRequest(int socket)
{
    int ret;
    ret = recv(socket, _ptr, freeSpace(), 0);
    _ptr += ret;
    if (ret <= 0)
        return (ret);
    while (parseData() == 1);
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
        if (findEndOfWord(' '))
        {
            _method = std::string(_head, _tail);
            _state = request::State::uri;
        }
        else
            return (0);
    case request::State::uri:
        if (passSymbols(' ') && findEndOfWord(' '))
        {
            if ((_tail - _head) / sizeof(char) > constants::limit_request_length)
            {
                _status_code = 414;
                return (-1);
            }
            if (getUri())
            {
                _status_code = 400;
                return (-1);
            }
            _state = request::State::version;
        }
        else
            return (0);
    case request::State::version:
        if (passSymbols(' ') && findEndOfWord('\r'))
        {
            _http_version = std::string(_head, _tail);
            if (findEndOfLine())
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
        if (!isEndOfRequest() && findEndOfWord(':'))
        {
            _header_name = std::string(_head, _tail);
            _state = request::State::headerValue;
        }
        else
            return (0);
    case request::State::headerValue:
        if (passSymbols(':') && findEndOfWord('\r'))
        {
            _headers.insert(std::make_pair(_header_name, std::string(_head, _tail)));
            if (findEndOfLine())
                _state = request::State::headerValue;
        }
        else
            return (0);
    case request::State::body:
        return (0);
    }
    return (1);
}

std::ostream & operator<<(std::ostream & o, Request const & req)
{
    o << "Method: " << req._method << std::endl;
    o << "URI: " << req._uri << std::endl;
    o << "HTTP version: " << req._http_version << std::endl;
    o << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = req._headers.begin(); \
            it != req._headers.end(); ++it)
        o << "    " << it->first << ": " << it->second <<std::endl;
    o << "\nBuffer:\n" << std::string(req._buffer, req._ptr) << std::endl;
    return (o);
}