#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <set>
#include "Response.hpp"
#include "Redirect.hpp"
#include "constants.hpp"

namespace location {
    extern Location	NoneLocation;
}

class Location {

private:
	int							_pathType;	// see constants namespace
	std::vector<std::string>	_loc_path;
	std::vector<std::string>	_index;
	std::vector<Location>		_locations;
	std::set<std::string>		_methods;
	int							_max_body_size;
	std::string					_root;
	Redirect *					_redir;
	bool						_cgi_enabled;
	int							_cgi_timeout;
	bool						_autoindex;
	int							_symbols_cut;
	
public:
	Location(int type, std::vector<std::string> const & path, std::string const & root, \
				bool ai, int max_body_size, int cut);
	Location(Location const & src);
	~Location() {}
	Location & operator=(Location const & src);
	
	void						addIndex(std::string const & index);
	void						clearMethodSet();
	void						addMethod(std::string const & method);
	void						setRedir(Redirect * redir);
	void						setAutoindex(bool ai);
	void						setRoot(std::string const & root);
	void						setMaxBodySize(int body_size);
	void						switchOnCgi(void);
	void						setCgiTimeout(int timout);

	Location *					checkLocation(std::string const & uri);
	bool						checkMethod(std::string & method);
	bool						checkBodySize(int size);
	Redirect *					getRedir(void);
	void						delRedir(void);
	int							getType(void);
	bool						getAutoindex(void);
	std::string					getResoursePath(std::string const & uri);
	int							getCgiTimout(void);
	std::vector<std::string> &	getIndexFiles(void);
	std::vector<Location> &		getLocations(void);
	int							getSymbolsToCut(void);

	friend bool 				operator>(const Location & l1, const Location & l2);
	friend bool					operator>=(const Location & l1, const Location & l2);
	friend bool					operator==(const Location & l1, const Location & l2);
	friend bool					operator!=(const Location & l1, const Location & l2);
	friend bool					operator<(const Location & l1, const Location & l2);
	friend bool					operator<=(const Location & l1, const Location & l2);
	
	friend std::ostream &		operator<<(std::ostream & o, Location const & src);
};

#endif
