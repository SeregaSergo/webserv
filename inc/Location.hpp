#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <vector>
#include <iostream>
#include "Redirect.hpp"

// Location types defines
#define L_EQUAL     0
#define L_EXTENTION 1
#define L_PARTLY    2

class Location {

private:
	int							_type;	// = ^ _
	std::string					_loc_path;
	std::vector<std::string>	_index;
	char						_methods;
	Redirect *					_redir;
	bool						_autoindex;
	std::string					_root;

	Location();

public:
	Location(int type, std::string & path)
		: _type(type), _loc_path(path), _redir(NULL), _methods(0), _autoindex(0) {};
	Location(Location const & src);
	~Location();
	Location & operator=(Location const & src);
	
	bool	check_location(std::string const & uri);
	int		get_type();
	bool	check_redir();
	bool	check_access();   // methods, authorization, existence
};

#endif
