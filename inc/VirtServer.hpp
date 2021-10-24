#ifndef VIRTSERVER_HPP
#define VIRTSERVER_HPP
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Location.hpp"
#include "Client.hpp"
#include "Webserv.hpp"


// Access message format :
// client address | request | status | body_bytes_sent | user_agent

class VirtServer {

private:
	Logger							_acc_log;
	long long int					_max_body_size;
	std::map<int, std::string>		_err_pages;
	std::vector<Location>			_locations;

public:
	VirtServer(int fdLog, long long int body_size, std::map<int, \
			std::string> & err_pages, std::vector<Location> & locations);
	~VirtServer();
	void sendAccMsg(std::string & msg);
	const Location * chooseLocation(std::string const & uri);
	bool IsBodyOversize(long long int body_size);
	// File & getPage(int num_page);
};

#endif
