#include "../inc/Request.hpp"

#include <string.h>

#ifdef LINUX_COMPILATION
char * strnstr(const char *s, const char *find, size_t slen)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') {
		len = strlen(find);
		do {
			do {
				if (slen-- < 1 || (sc = *s++) == '\0')
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}
#endif

Request::Request(Server * server)
    : _server(server)
    , _virt_serv(NULL)
    , _location(NULL)
    , _state(request::State::method)
    , _status_code(200)
    , _body_size(-2)
{
    _buffer = new char[constants::incoming_buffer];
    _ptr = _buffer;
    _end = _buffer + constants::incoming_buffer;
    _head = _buffer;
    _tail = _buffer;
}

Request::~Request(void) {
    delete[] _buffer;
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
    if (findEndOfLine())
    {
        // prepare buffer for reading
        memmove(_buffer, _tail, (_ptr - _tail) / sizeof(char));
        _ptr = _buffer;
        // checking length
        std::map<std::string, std::string>::iterator    ptr;
        if ((ptr = _headers.find("Content-Length")) != _headers.end())
            _body_size = atoll(&ptr->second[0]);
        // else if ((ptr = _headers.find("Transfer-Encoding")) != _headers.end())
        // {

        // }
        // else

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
    std::string header_name;
    switch (_state)
    {
    case request::State::getting_headers:
    {
        char * headers_end = strnstr(_buffer, "\r\n\r\n", (_ptr - _buffer) / sizeof(char));
        if (headers_end == NULL)
        {
            if ((_ptr - _buffer) / sizeof(char) > constants::limit_request_length)
                return (errorCode(413));     // Request Entity Too Large
            return (request::ReturnCode::unfinished);
        }
        else
            _state = request::State::method;
    }

    case request::State::method:
    {
        if (findEndOfWord(' '))
        {
            _method = std::string(_head, _tail);
            if (!isMethodImplemented())
                return (errorCode(501)); // Not implemented
            _state = request::State::uri;
        }
        else
            return (errorCode(400)); // Bad request
    }

    case request::State::uri:
    {
        if (passSymbols(' ') && findEndOfWord(' '))
        {
            if ((_tail - _head) / sizeof(char) > constants::limit_uri_length)
                return (errorCode(414));    // Request-URI Too Long
            if (getUri())
                return (errorCode(400));     // Bad Request
            _state = request::State::version;
        }
        else
            return (errorCode(400)); // Bad request
    }

    case request::State::version:
    {
        if (passSymbols(' ') && findEndOfWord('\r'))
        {
            _http_version = std::string(_head, _tail);
            if (findEndOfLine())
                _state = request::State::endOfHeaders;
            if (!isVersionSupported())
                return (errorCode(505));     // HTTP Version Not Supported
        }
        else
            return (errorCode(400)); // Bad request
    }

    case request::State::endOfHeaders:
    {
        if (findEndOfLine())
        {
            std::map<std::string, std::string>::iterator    ptr;
            ptr = _headers.find("Host");
            if (ptr == _headers.end())
                _virt_serv = _server->getVirtualServ("");
            else
                _virt_serv = _server->getVirtualServ(ptr->second);
            if ((_location = _virt_serv->chooseLocation(_uri)) == NULL)
                return (errorCode(404));     // Not found
            if ((ptr = _headers.find("Transfer-Encoding")) != _headers.end())
            {
                if (ptr->second != "chunked")
                    return (errorCode(501));     // Not Implemented
                else
                    _state = request::State::chunk_size;
            }
            else if ((ptr = _headers.find("Content-Length")) != _headers.end())
            {
                _body_size = atoll(&ptr->second[0]);
                _state = request::State::body;
            }
            else
                return (errorCode(411));     // Length Required
            if (!_location->checkMethod(_method))
                return (errorCode(405));     // Method Not Allowed

            memmove(_buffer, _tail, (_ptr - _tail) / sizeof(char));
            _ptr = _buffer;
        }
        else
            _state = request::State::headerName;
        break;
    }

    case request::State::headerName:
    {
        if (findEndOfWord(':'))
        {
            header_name = std::string(_head, _tail);
            _state = request::State::headerValue;
        }
        else
            return (errorCode(400)); // Bad request
    }

    case request::State::headerValue:
    {
        if (passSymbols(':') && passSymbols(' ') && findEndOfWord('\r'))
        {
            _headers.insert(std::make_pair(header_name, std::string(_head, _tail)));
            if (findEndOfLine())
                _state = request::State::endOfHeaders;
        }
        else
            return (errorCode(400)); // Bad request
        break;
    }

    case request::State::body:
    {
        return (request::ReturnCode::completed);
    }

    case request::State::chunk_size:
    {
        return (request::ReturnCode::completed);
    }

    case request::State::chunk:
    {
        return (request::ReturnCode::completed);
    }

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
    o << "Status code: " << req._status_code << std::endl;
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