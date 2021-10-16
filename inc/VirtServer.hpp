#ifndef VIRTSERVER_HPP
#define VIRTSERVER_HPP
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Location.hpp"
#include "Client.hpp"
#include "Webserv.hpp"

class VirtServer {

private:
	Logger *						_acc_log;
	long long int					_max_body_size;
	std::string						_root;
	std::map<int, std::string>		_err_pages;
	std::vector<Location>			_locations;

public:
	~VirtServer();
	void sendAccMsg(std::string & msg);
	const Location * chooseLocation(std::string const & uri);
	bool IsBodyOversize(long long int body_size);
	void flushLoggerBuffers(void);
	// File & getPage(int num_page);
};

#endif
