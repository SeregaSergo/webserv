#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <vector>
#include <set>
#include <iostream>
#include "Redirect.hpp"

// Location types defines
namespace constants
{
    const char loc_equal_type(3);
    const char loc_ext_type(2);
    const char loc_partly_type(1);

	const std::string methods [] = {"GET", "POST", "DELETE"};
}

class Location {

private:
	char						_type;	// = ^ _
	std::vector<std::string>	_loc_path;
	std::vector<std::string>	_index;
	std::set<std::string>		_methods;
	Redirect *					_redir;
	bool						_autoindex;
	std::string					_root;

	// Location();

public:
	Location(char type, std::vector<std::string> & path, std::string & root, bool ai);
	// Location(Location const & src);
	~Location();
	// Location & operator=(Location const & src);
	
	void		addIndex(std::string & index);
	void		clearMethodSet();
	void		addMethod(std::string &method);
	void		setRedir(Redirect * redir);
	void		setAutoindex(bool ai);
	void		setRoot(std::string & root);

	char		checkLocation(std::string const & uri);
	Redirect *	getRedir();

	// bool	check_access();   // methods, authorization, existence
};

#endif
