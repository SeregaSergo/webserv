#ifndef VIRTSERVER_HPP
#define VIRTSERVER_HPP

#include "constants.hpp"
#include "Location.hpp"
#include "Client.hpp"
#include "Webserv.hpp"
#include "Logger.hpp"

class Webserv;
struct ConfigServ;

// This class is represents virtual server that described in
// a config file. The main purpose of this class is choosing
// location.

class VirtServer {

private:
	Logger *						_acc_log;
	long long int					_max_body_size;
	std::map<int, std::string>		_err_pages;
	std::vector<Location>			_locations;

	VirtServer(Logger * logger, const long long int body_size, const std::map<int, \
			std::string> & err_pages, const std::vector<Location> & locations);

public:
	static VirtServer * create(ConfigServ const & conf, Webserv & wbsrv);
	~VirtServer();
	VirtServer(VirtServer const & src);
	void sendAccMsg(std::string & msg);
	Location * chooseLocation(std::string const & uri);
	bool IsBodyOversize(long long int body_size);
	std::string getPage(int num_page);
};

#endif
