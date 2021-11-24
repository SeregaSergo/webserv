#include "../inc/Location.hpp"

Location::Location(int type, std::vector<std::string> & path, std::string & root, bool ai)
		: _type(type)
		, _loc_path(path)
		, _redir(NULL)
		, _autoindex(ai)
		, _root(root)
{
	for (int i = sizeof(constants::methods) / sizeof(std::string) - 1; i != -1; --i)
		_methods.insert(constants::methods[i]);
}

Location::Location(Location const & src)
	: _type(src._type)
	, _loc_path(src._loc_path)
	, _index(src._index)
	, _methods(src._methods)
	, _redir(src._redir)
	, _autoindex(src._autoindex)
	, _root(src._root)
{}

Location & Location::operator=(Location const & src)
{
	if (this == &src)
		return (*this);
	_type = src._type;
	_loc_path = src._loc_path;
	_index = src._index;
	_methods = src._methods;
	_redir = src._redir;
	_autoindex = src._autoindex;
	_root = src._root;
	return (*this);
}

void	Location::addIndex(std::string const & index) {
	_index.push_back(index);
}

void	Location::clearMethodSet() {
	_methods.clear();
}

void	Location::addMethod(std::string const & method) {
	_methods.insert(method);
}

void	Location::setRedir(Redirect * redir) {
	_redir = redir;
}

void	Location::setAutoindex(bool ai) {
	_autoindex = ai;
}

void	Location::setRoot(std::string const & root) {
	_root = root;
}

// if the location is suitable to uri then return
// apropriate priority, else return 0
char	Location::checkLocation(std::string const & uri)
{
	std::cout << "compare: " << uri << " and " << _loc_path[0] << std::endl; 
	std::cout << "Type: " << _type << std::endl;
	if (_type == location::Type::equal) {
		if (uri == _loc_path[0])
			return (location::Type::equal);
	}
	else if (_type == location::Type::extention) {
		std::size_t found = uri.find_last_of(".");
		if (found != std::string::npos) {
			std::string ext_uri = uri.substr(found + 1);
			for (std::vector<std::string>::iterator it = _loc_path.begin(); it != _loc_path.end(); ++it) {
				if (*it == ext_uri)
					return (location::Type::extention);
			}
		}
	}
	else if (uri.compare(0, _loc_path[0].size(), _loc_path[0]) == 0)
		return (location::Type::partial);
	return (0);
}

bool	Location::checkMethod(std::string & method)
{
	return (_methods.find(method) != _methods.end());
}

Redirect * Location::getRedir(void) {
	return (_redir);
}

void Location::delRedir(void) {
	if (_redir)
		delete _redir;
}

std::ostream & operator<<(std::ostream & o, Location const & src) {
    o << "Type: " << (int)src._type << std::endl;
	o << "Location path: ";
	for (std::vector<std::string>::const_iterator it = src._loc_path.begin(); it != src._loc_path.end(); ++it)
		o << *it << "  ";
	o << "\nIndex: ";
	for (std::vector<std::string>::const_iterator it = src._index.begin(); it != src._index.end(); ++it)
		o << *it << "  ";
	o << "\nAllowed methods: ";
	for (std::set<std::string>::const_iterator it = src._methods.begin(); it != src._methods.end(); ++it)
		o << *it << "  ";
	o << "\nRedirection: ";
	if (src._redir)
		o << "\n" << *src._redir << "\n";
	else
		o << "none" << std::endl;
	o << "Autoindex: " << src._autoindex << std::endl;
	o << "Root: " << src._root << std::endl;
	return (o);
}