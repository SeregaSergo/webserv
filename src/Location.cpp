#include "../inc/Location.hpp"

Location::Location(char type, std::vector<std::string> & path, std::string & root)
		: _type(type)
		, _loc_path(path)
		, _redir(NULL)
		, _autoindex(false)
		, _root(root)
{
	for (int i = sizeof(constants::methods) / sizeof(std::string); i != 0; --i)
		_methods.insert(constants::methods[i]);
}

Location::~Location(void)
{
	if (_redir)
		delete _redir;
}

void	Location::addIndex(std::string & index) {
	_index.push_back(index);
}

void	Location::clearMethodSet() {
	_methods.clear();
}

void	Location::addMethod(std::string & method) {
	_methods.insert(method);
}

void	Location::setRedir(Redirect * redir) {
	_redir = redir;
}

void	Location::setAutoindex(bool ai) {
	_autoindex = ai;
}

void	Location::setRoot(std::string & root) {
	_root = root;
}

// if the location is suitable to uri then return
// apropriate priority, else return 0
char	Location::checkLocation(std::string const & uri)
{
	if (_type == constants::loc_equal_type) {
		if (uri == _loc_path[0])
			return (constants::loc_equal_type);
	}
	else if (_type == constants::loc_ext_type) {
		std::size_t found = uri.find_last_of(".");
		if (found != std::string::npos) {
			std::string ext_uri = uri.substr(found + 1);
			for (std::vector<std::string>::iterator it = _loc_path.begin(); it != _loc_path.end(); ++it) {
				if (*it == ext_uri)
					return (constants::loc_ext_type);
			}
		}
	}
	else if (uri.compare(0, _loc_path[0].size(), _loc_path[0]) == 0)
		return (constants::loc_partly_type);
	return (0);
}

Redirect *	Location::getRedir(void) {
	return (_redir);
}
