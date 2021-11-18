#include "../inc/Request.hpp"

Request::Request(void)
    : _state(request::State::method)
    , _status_code(200)
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

inline bool Request::isEndOfHeaders(void)
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

inline bool Request::isMethodImplemented(void)
{
    for (int i = sizeof(constants::methods) / sizeof(std::string) - 1; i != -1; --i)
        if (_method == constants::methods[i])
            return (true);
    return (false);
}

int    Request::getRequest(int socket)
{
    int ret;
    ret = recv(socket, _ptr, freeSpace(), 0);
    _ptr += ret;
    if (ret < 0)
        return (request::ReturnCode::error);
    if (ret == 0)
        return (request::ReturnCode::disconnected);
    while (!(ret = parseData()))
    {
        // switch (_state)
        // {
        // case request::State::method:
        //     std::cout << "method" << std::endl;
        //     break;
        // case request::State::uri:
        //     std::cout << "uri" << std::endl;
        //     break;
        // case request::State::version:
        //     std::cout << "version" << std::endl;
        //     break;
        // case request::State::headerName:
        //     std::cout << "header name" << std::endl;
        //     break;
        // case request::State::headerValue:
        //     std::cout << "header value" << std::endl;
        //     break;
        // case request::State::body:
        //     std::cout << "body" << std::endl;
        //     break;
        // }
    }
    return (ret);
}

inline int Request::errorCode(int code)
{
    _status_code = code;
    return (request::ReturnCode::completed);
}

// State machine for parsing.
// return:  request::ReturnCode::unfinished - Stop parsing, wait for new data,
//          request::ReturnCode::completed - Stop parsing, send response,
//          0 - Continue parsing;
int Request::parseData(void)
{
    char * headers_end;
    switch (_state)
    {
    case request::State::method:
        headers_end = strnstr(_buffer, "\r\n\r\n", (_ptr - _buffer) / sizeof(char));
        if (headers_end == NULL)
        {
            if ((_ptr - _buffer) / sizeof(char) > constants::limit_request_length)
                return (errorCode(413));     // Request Entity Too Large
            return (request::ReturnCode::unfinished);
        }
        if ((headers_end - _buffer) / sizeof(char) > constants::limit_request_length)
            return (errorCode(413));        // Request Entity Too Large
        if (findEndOfWord(' '))
        {
            _method = std::string(_head, _tail);
            if (!isMethodImplemented())
                return (errorCode(501)); // Not implemented
            _state = request::State::uri;
        }
        else
            return (request::ReturnCode::unfinished);
    case request::State::uri:
        if (passSymbols(' ') && findEndOfWord(' '))
        {
            if ((_tail - _head) / sizeof(char) > constants::limit_uri_length)
                return (errorCode(414));    // Request-URI Too Long
            if (getUri())
                return (errorCode(400));     // Bad Request
            _state = request::State::version;
        }
        else
            return (request::ReturnCode::unfinished);
    case request::State::version:
        if (passSymbols(' ') && findEndOfWord('\r'))
        {
            _http_version = std::string(_head, _tail);
            if (findEndOfLine())
                _state = request::State::headerName;
            if (!isVersionSupported())
                return (errorCode(505));     // HTTP Version Not Supported
        }
        else
            return (request::ReturnCode::unfinished);
    case request::State::headerName:
        if (isEndOfHeaders())
            break;
        if (findEndOfWord(':'))
        {
            _header_name = std::string(_head, _tail);
            _state = request::State::headerValue;
        }
        else
            return (request::ReturnCode::unfinished);
    case request::State::headerValue:
        if (passSymbols(':') && findEndOfWord('\r'))
        {
            _headers.insert(std::make_pair(_header_name, std::string(_head, _tail)));
            if (findEndOfLine())
                _state = request::State::headerName;
        }
        else
            return (request::ReturnCode::unfinished);
        break;
    case request::State::body:
        std::cout << "in parseData body" << std::endl;
        return (request::ReturnCode::completed);
    }
    return (0);
}

int Request::getStatusCode(void)
{
    return (_status_code);
}

std::ostream & operator<<(std::ostream & o, Request const & req)
{
    o << "Method: " << req._method << std::endl;
    o << "URI: " << req._uri << std::endl;
    o << "HTTP version: " << req._http_version << std::endl;
    o << "State: ";
    switch (req._state)
        {
        case request::State::method:
            o << "method" << std::endl;
            break;
        case request::State::uri:
            o << "uri" << std::endl;
            break;
        case request::State::version:
            o << "version" << std::endl;
            break;
        case request::State::headerName:
            o << "header name" << std::endl;
            break;
        case request::State::headerValue:
            o << "header value" << std::endl;
            break;
        case request::State::body:
            o << "body" << std::endl;
            break;
        }
    o << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = req._headers.begin(); \
            it != req._headers.end(); ++it)
        o << "    " << it->first << ": " << it->second <<std::endl;
    o << "\nBuffer:\n" << std::string(req._buffer, req._ptr) << std::endl;
    return (o);
}