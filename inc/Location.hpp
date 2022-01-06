#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <set>
#include "Response.hpp"
#include "Redirect.hpp"
#include "constants.hpp"


class Location {

private:
	int							_pathType;	// see constants namespace
	std::vector<std::string>	_loc_path;
	std::vector<std::string>	_index;
	std::set<std::string>		_methods;
	int							_max_body_size;
	std::string					_root;
	Redirect *					_redir;
	std::string					_cgi_interpreter;
	int							_cgi_timeout;
	bool						_autoindex;

public:
	Location(int type, std::vector<std::string> & path, std::string & root, bool ai, int max_body_size);
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
	void						setCgiInterpreter(const char * interpreter);
	void						setCgiTimeout(int timout);

	char						checkLocation(std::string const & uri, unsigned int * max_symbols);
	bool						checkMethod(std::string & method);
	bool						checkBodySize(int size);
	Redirect *					getRedir(void);
	void						delRedir(void);
	int							getType(void);
	bool						getAutoindex(void);
	std::string					getResoursePath(std::string const & uri);
	const char *				getCgiInterpreter(void);
	int							getCgiTimout(void);
	std::vector<std::string> &	getIndexFiles(void);

	friend std::ostream &		operator<<(std::ostream & o, Location const & src);
	friend class Response;
};

#endif
