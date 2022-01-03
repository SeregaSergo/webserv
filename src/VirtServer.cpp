#include "../inc/VirtServer.hpp"

VirtServer::VirtServer(Logger * logger, const long long int body_size, std::string root, \
						bool sessions, const std::map<int, std::string> & err_pages, \
						const std::vector<Location> & locations)
		: _acc_log(logger)
		, _max_body_size(body_size)
		, _document_root(root)
		, _sessions_enabled(sessions)
		, _err_pages(err_pages)
		, _locations(locations)
{
	if (_sessions_enabled)
	{
		pthread_mutex_init(&_mutex, NULL);
	}
}

VirtServer * VirtServer::create(ConfigServ const & conf, Webserv & wbsrv)
{
	int acc_log_fd = open(&conf.acc_log[0], O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if (acc_log_fd < 0) {
		std::cerr << "WARNING: can't open access file: " << &conf.acc_log[0] << std::endl;
		acc_log_fd = open(constants::default_file, O_WRONLY);
	}
	Logger * acc_logger = new Logger(acc_log_fd);
	VirtServer * virt_serv = new VirtServer(acc_logger, conf.client_max_body_size, \
											conf.root, conf.sessions_enabled, \
											conf.err_pages, conf.locations);
	wbsrv.addHandler(acc_logger);

	if (conf.sessions_enabled)
	{
		std::string dir_name_sid = conf.root + ".sessions";
		if (mkdir(dir_name_sid.c_str(), 0777))
			std::cerr << "WARNING: can't create session directory: " << dir_name_sid << std::endl;
	}
	return (virt_serv);
}

static int rmFiles(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    remove(pathname);
	(void) sbuf;
	(void) type;
	(void) ftwb;
    return 0;
}

VirtServer::~VirtServer(void)
{
	pthread_mutex_destroy(&_mutex);
	delete _acc_log;
	for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); ++it)
		(*it).delRedir();

	std::string dir_name_sid = _document_root + ".sessions";
	if (nftw(dir_name_sid.c_str(), rmFiles, 10, FTW_DEPTH|FTW_MOUNT|FTW_PHYS) < 0)
		DISPLAY(std::cout << "Error while deleting session directory." << std::endl);
}

VirtServer::VirtServer(VirtServer const & src)
	: _acc_log(src._acc_log)
	, _max_body_size(src._max_body_size)
	, _document_root(src._document_root)
	, _sessions_enabled(src._sessions_enabled)
	, _sessions(src._sessions)
	, _mutex(src._mutex)
	, _dead_sessions(src._dead_sessions)
	, _err_pages(src._err_pages)
	, _locations(src._locations)
{}

void VirtServer::sendAccMsg(std::string & msg) {
	_acc_log->sendMsg(msg);
}

Location * VirtServer::chooseLocation(std::string const & uri)
{
	std::vector<Location>::iterator winner_pos;
	int ret;
	unsigned int max_symbols = 0;
	int win_prior = location::pathType::none;
	for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); ++it)
	{
		ret = (*it).checkLocation(uri, &max_symbols);
		if (ret >= win_prior)
		{
			winner_pos = it;
			win_prior = ret;
		}
	}
	if (win_prior == location::pathType::none)
		return NULL;
	return (winner_pos.base());	
}

bool VirtServer::IsBodyOversize(long long int body_size) {
	return (body_size > _max_body_size);
}

std::string VirtServer::getPage(int num_page)
{
	std::map<int, std::string>::iterator ptr = _err_pages.find(num_page);
	if (ptr == _err_pages.end())
		return ("");
	else
		return (ptr->second);
}

void VirtServer::giveID(std::string	& cookies, std::string & client_sid)
{
	time_t creation_time = std::time(NULL);
	client_sid = decToHexStr(creation_time) + decToHexStr(std::rand());
	std::string filename = _document_root + ".sessions/" + client_sid;
	if (!fopen(filename.c_str() , "rw"))
	{
		client_sid.clear();
		return;
	}
	if (cookies.empty())
		cookies += "SID=" + client_sid;
	else
		cookies += "; SID=" + client_sid;
	_sessions[client_sid] = creation_time;
}

void VirtServer::init_session(std::map<std::string, std::string> & headers, \
								std::string & client_sid)
{
	if (!_sessions_enabled)
		return;
	std::string	& cookies = headers["Cookie"];
	size_t pos = cookies.find("SID=");
	std::string sid;
	if (pos != std::string::npos)
	{
		pos += 4;
		size_t end = cookies.find(';', pos);
		if (end == std::string::npos)
			sid = cookies.substr(pos);
		else
			sid = cookies.substr(pos, end - pos);
		if (_sessions.find(sid) == _sessions.end())
		{
			pos -= 4;
			if (end == std::string::npos)
				cookies.erase(pos);
			else
				cookies.erase(pos, end - pos + 1);
			giveID(cookies, client_sid);
		}
		else
			client_sid = sid;
	}
	else
		giveID(cookies, client_sid);
}

void VirtServer::cleanSessions(time_t cur_time)
{
	if (!_sessions_enabled)
		return;
	pthread_mutex_lock(&_mutex);
	for (std::map<std::string, time_t>::iterator it = _sessions.begin(); it != _sessions.end(); ++it)
	{
		if (cur_time - it->second > constants::timeout_session)
			_dead_sessions.push_back(it->first);
	}
	DISPLAY( std::cout << "Virt_server deleted these SIDs:" << std::endl; )
	for (std::vector<std::string>::iterator it = _dead_sessions.begin(); it != _dead_sessions.end(); ++it)
	{
		DISPLAY( std::cout << *it << std::endl; )
		_sessions.erase(*it);
	}
	pthread_mutex_unlock(&_mutex);
}

std::ostream & operator<<(std::ostream & o, VirtServer const & serv)
{
	o << "\n***** Virtual server *****\n" << std::endl;
    o << "Max body size:\n" << serv._max_body_size << std::endl;
    o << "Document_root: " << serv._document_root << std::endl;
    o << "Sessions enabled: " << serv._sessions_enabled << std::endl;
    return (o);
}