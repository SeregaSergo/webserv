#ifndef VIRTSERVER_HPP
#define VIRTSERVER_HPP

#include <ctime>
#include <ftw.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
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
	std::string						_document_root;

	bool							_sessions_enabled;
	std::map<std::string, time_t>	_sessions;
	pthread_mutex_t					_mutex;
	pthread_mutex_t					_sync_mutex;
	pthread_t						_threadID;
	std::vector<std::string>		_dead_sessions;

	std::map<int, std::string>		_err_pages;
	std::vector<Location>			_locations;

	VirtServer(Logger * logger, const long long int body_size, std::string root, \
				bool sessions, const std::map<int, std::string> & err_pages, \
				const std::vector<Location> & locations);
	
	void				giveID(std::string	& cookies, std::vector<std::string> & set_cookies);
	static void * 		cleaningProcess(void * args);

public:
	static VirtServer * create(ConfigServ const & conf, Webserv & wbsrv);
	VirtServer(VirtServer const & src);
	~VirtServer();

	void 				sendAccMsg(std::string & msg);
	Location * 			chooseLocation(std::string const & uri);
	bool 				IsBodyOversize(long long int body_size);
	std::string 		getPage(int num_page);
	void 				init_session(std::map<std::string, std::string> & headers, \
										std::vector<std::string> & _set_cookies);
	void 				cleanSessions(time_t cur_time);
	std::string const & getDocRoot(void) const;

	friend std::ostream & operator<<(std::ostream & o, VirtServer const & serv);
};

#endif
