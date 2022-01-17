#include "../inc/Location.hpp"

Location::Location(int type, std::vector<std::string> const & path, std::string const & root, \
					bool ai, int max_body_size, int cut = 0)
		: _pathType(type)
		, _loc_path(path)
		, _max_body_size(max_body_size)
		, _root(root)
		, _redir(NULL)
		, _cgi_enabled(false)
		, _cgi_timeout(constants::cgi_timeout)
		, _autoindex(ai)
		, _symbols_cut(cut)
{
	for (int i = sizeof(constants::methods) / sizeof(std::string) - 1; i != -1; --i)
		_methods.insert(constants::methods[i]);
}

Location::Location(Location const & src)
	: _pathType(src._pathType)
	, _loc_path(src._loc_path)
	, _index(src._index)
	, _locations(src._locations)
	, _methods(src._methods)
	, _max_body_size(src._max_body_size)
	, _root(src._root)
	, _redir(src._redir)
	, _cgi_enabled(src._cgi_enabled)
	, _cgi_timeout(src._cgi_timeout)
	, _autoindex(src._autoindex)
	, _symbols_cut(src._symbols_cut)
{}

Location & Location::operator=(Location const & src)
{
	if (this == &src)
		return (*this);
	_pathType = src._pathType;
	_loc_path = src._loc_path;
	_index = src._index;
	_locations = src._locations;
	_methods = src._methods;
	_redir = src._redir;
	_autoindex = src._autoindex;
	_root = src._root;
	_symbols_cut = src._symbols_cut;
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
	// if (*root.rbegin() != '/')
	// 	_root = root + "/";
	// else
		_root = root;
}

void	Location::setMaxBodySize(int body_size) {
	_max_body_size = body_size;
}

void	Location::switchOnCgi(void) {
	_cgi_enabled = true;
}

void	Location::setCgiTimeout(int timout)
{
	if (timout < 0)
		_cgi_timeout = constants::cgi_timeout;
	else
		_cgi_timeout = timout;
}

// if the location is suitable to uri then return
// apropriate priority, else return 0
Location *	Location::checkLocation(std::string const & uri)
{
	Location * ret = &location::NoneLocation;
	
	if (_pathType == location::pathType::equal) {
		if (uri == _loc_path[0])
			ret = this;
	}
	else if (_pathType == location::pathType::extention) {
		std::size_t found = uri.find_last_of(".");
		if (found != std::string::npos) {
			std::string ext_uri = uri.substr(found + 1);
			for (std::vector<std::string>::iterator it = _loc_path.begin(); it != _loc_path.end(); ++it) {
				if (*it == ext_uri)
					ret = this;
			}
		}
	}
	else if (uri.compare(_symbols_cut - _loc_path[0].size(), _loc_path[0].size(), _loc_path[0]) == 0)
	{
		ret = this;
		if (!_locations.empty())
		{
			Location *	_ret;
			Location *	winner = &location::NoneLocation;

			for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); ++it)
			{
				_ret = (*it).checkLocation(uri);
				if (*_ret > *winner)
					winner = _ret;
			}
			if (winner != &location::NoneLocation)
				ret = winner;
		}
	}
	return (ret);
}

std::string  Location::getResoursePath(std::string const & uri)
{
	return (_root + &uri[_symbols_cut]);
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
	else if (_cgi_enabled)
		return (location::Type::cgi);
	else
		return (location::Type::file);
}

bool Location::getAutoindex(void) {
	return (_autoindex);
}

int	Location::getCgiTimout(void) {
	return (_cgi_timeout);
}

std::vector<Location> &	Location::getLocations(void) {
	return (_locations);
}

int	Location::getSymbolsToCut(void) {
	return (_symbols_cut);
}

Location	location::NoneLocation(location::pathType::none, std::vector<std::string>(), std::string(), false, 0, 0);

bool	operator>(const Location & l1, const Location & l2) {
	return (l1._pathType > l2._pathType);
}

bool	operator>=(const Location & l1, const Location & l2) {
	return (l1._pathType >= l2._pathType);
}

bool	operator==(const Location & l1, const Location & l2) {
	return (l1._pathType == l2._pathType);
}

bool	operator!=(const Location & l1, const Location & l2) {
	return (l1._pathType != l2._pathType);
}

bool	operator<(const Location & l1, const Location & l2) {
	return (l1._pathType < l2._pathType);
}

bool	operator<=(const Location & l1, const Location & l2) {
	return (l1._pathType <= l2._pathType);
}

std::ostream & operator<<(std::ostream & o, Location const & src) {
    o << "Type: " << (int)src._pathType << std::endl;
	o << "Location path: ";
	for (std::vector<std::string>::const_iterator it = src._loc_path.begin(); it != src._loc_path.end(); ++it)
		o << *it << "  ";
	o << "\nIndex: ";
	for (std::vector<std::string>::const_iterator it = src._index.begin(); it != src._index.end(); ++it)
		o << *it << "  ";
	o << "\nLocations inner:\n";
	for (std::vector<Location>::const_iterator it = src._locations.begin(); it != src._locations.end(); ++it)
	{
		o << "***************************" << std::endl;
		o << *it << std::endl;
		o << "***************************" << std::endl;
	}
	o << "\nSymbols to cut: " << src._symbols_cut << std::endl;
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
	o << "CGI enabled: " << src._cgi_enabled << std::endl;
	return (o);
}