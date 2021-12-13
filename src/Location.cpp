#include "../inc/Location.hpp"

Location::Location(int type, std::vector<std::string> & path, std::string & root, bool ai, int max_body_size)
		: _pathType(type)
		, _loc_path(path)
		, _max_body_size(max_body_size)
		, _root(root)
		, _redir(NULL)
		, _cgi_timeout(constants::cgi_timout)
		, _autoindex(ai)
{
	for (int i = sizeof(constants::methods) / sizeof(std::string) - 1; i != -1; --i)
		_methods.insert(constants::methods[i]);
}

Location::Location(Location const & src)
	: _pathType(src._pathType)
	, _loc_path(src._loc_path)
	, _index(src._index)
	, _methods(src._methods)
	, _max_body_size(src._max_body_size)
	, _root(src._root)
	, _redir(src._redir)
	, _cgi_interpreter(src._cgi_interpreter)
	, _cgi_timeout(src._cgi_timeout)
	, _autoindex(src._autoindex)
{}

Location & Location::operator=(Location const & src)
{
	if (this == &src)
		return (*this);
	_pathType = src._pathType;
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
	if (*root.rbegin() != '/')
		_root = root + "/";
	else
		_root = root;
}

void	Location::setMaxBodySize(int body_size) {
	_max_body_size = body_size;
}

void	Location::setCgiInterpreter(const char * interpreter) {
	_cgi_interpreter = interpreter;
}

void	Location::setCgiTimout(int timout)
{
	if (timout < 0)
		_cgi_timeout = constants::cgi_timout;
	else
		_cgi_timeout = timout;
}

// if the location is suitable to uri then return
// apropriate priority, else return 0
char	Location::checkLocation(std::string const & uri, unsigned int * max_symbols)
{
	std::cout << "Location: " << _loc_path[0] << std::endl;
	if (_pathType == location::pathType::equal) {
		if (uri == _loc_path[0])
			return (location::pathType::equal);
	}
	else if (_pathType == location::pathType::extention) {
		std::size_t found = uri.find_last_of(".");
		if (found != std::string::npos) {
			std::string ext_uri = uri.substr(found + 1);
			for (std::vector<std::string>::iterator it = _loc_path.begin(); it != _loc_path.end(); ++it) {
				if (*it == ext_uri)
					return (location::pathType::extention);
			}
		}
	}
	else if (uri.compare(0, _loc_path[0].size(), _loc_path[0]) == 0 && \
				_loc_path[0].size() > *max_symbols)
	{
		*max_symbols = _loc_path[0].size();
		return (location::pathType::partial);
	}
	return (0);
}

std::string  Location::getResoursePath(std::string const & uri)
{
	if (_pathType == location::pathType::partial)
		return (_root + &uri[_loc_path[0].size()]);
	else
		return (_root + &uri[1]);
}

bool	Location::checkMethod(std::string & method) {
	return (_methods.find(method) != _methods.end());
}

bool	Location::checkBodySize(int size) {
	return (size > _max_body_size);
}

std::vector<std::string> & Location::getIndexFiles(void) {
	return (_index);
}

Redirect * Location::getRedir(void) {
	return (_redir);
}

void Location::delRedir(void) {
	if (_redir)
		delete _redir;
}

int	Location::getType(void)
{
	if (_redir)
		return (location::Type::redirection);
	else if (!_cgi_interpreter.empty())
		return (location::Type::cgi);
	else
		return (location::Type::file);
}

bool Location::getAutoindex(void) {
	return (_autoindex);
}

std::ostream & operator<<(std::ostream & o, Location const & src) {
    o << "Type: " << (int)src._pathType << std::endl;
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