#include "../inc/Redirect.hpp"

std::ostream & operator<<(std::ostream & o, Redirect const & src) {
    o << "Code: " << src._code << std::endl;
    o << "URL: " << src._url << std::endl;
	o << "URI: " << src._uri << std::endl;
	return (o);
}