#include "../inc/Redirect.hpp"

Redirect::Redirect(Redirect const & src)
	: _code(src._code)
	, _url(src._url)
	, _uri(src._uri)
{}

int Redirect::get_code(void) {
    return (_code);
}

std::string Redirect::rewrite_url(std::string const & uri)
{
    if (_uri)
        return (_url + uri);
    else
        return (_url);
}

std::ostream & operator<<(std::ostream & o, Redirect const & src) {
    o << "Code: " << src._code << std::endl;
    o << "URL: " << src._url << std::endl;
	o << "URI: " << src._uri << std::endl;
	return (o);
}