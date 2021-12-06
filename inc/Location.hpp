#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <vector>
#include <set>
#include <iostream>
#include "Redirect.hpp"
#include "constants.hpp"

// Location types defines
// namespace constants
// {
//     const char loc_equal_pathType(3);
//     const char loc_ext_pathType(2);
//     const char loc_partly_pathType(1);

// 	const std::string methods [] = {"GET", "POST", "DELETE"};
// }

class Location {

private:
	int							_pathType;	// see constants namespace
	std::vector<std::string>	_loc_path;
	std::vector<std::string>	_index;
	std::set<std::string>		_methods;
	int							_max_body_size;
	std::string					_root;

	Redirect *					_redir;

	// std::string					_cgi_pass;
	// int							_cgi_timeout;

	bool						_autoindex;

	// Location();

public:
	Location(int type, std::vector<std::string> & path, std::string & root, bool ai, int max_body_size);
	Location(Location const & src);
	~Location() {}
	Location & operator=(Location const & src);
	
	void			addIndex(std::string const & index);
	void			clearMethodSet();
	void			addMethod(std::string const & method);
	void			setRedir(Redirect * redir);
	void			setAutoindex(bool ai);
	void			setRoot(std::string const & root);
	void			setMaxBodySize(int body_size);

	char			checkLocation(std::string const & uri, unsigned int * max_symbols);
	bool			checkMethod(std::string & method);
	bool			checkBodySize(int size);
	Redirect *		getRedir(void);
	void			delRedir(void);
	int				getType(void);
	bool			getAutoindex(void);
	std::string		getResoursePath(std::string const & uri);
	std::vector<std::string> & getIndexFiles(void);

	// bool	check_access();   // methods, authorization, existence
	friend std::ostream & operator<<(std::ostream & o, Location const & src);
};

std::ostream & operator<<(std::ostream & o, Location const & src);

#endif
