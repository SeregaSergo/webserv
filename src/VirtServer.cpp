#include "../inc/VirtServer.hpp"

VirtServer::VirtServer(Logger * logger, const long long int body_size, const std::map<int, \
			std::string> & err_pages, const std::vector<Location> & locations)
		: _acc_log(logger)
		, _max_body_size(body_size)
		, _err_pages(err_pages)
		, _locations(locations)
{}

VirtServer * VirtServer::create(ConfigServ const & conf, Webserv & wbsrv)
{
	int acc_log_fd = open(&conf.acc_log[0], O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if (acc_log_fd < 0) {
		std::cerr << "WARNING: can't open access file: " << &conf.acc_log[0] << std::endl;
		acc_log_fd = open(constants::default_file, O_WRONLY);
	}
	Logger * acc_logger = new Logger(acc_log_fd);
	VirtServer * virt_serv = new VirtServer(acc_logger, conf.client_max_body_size, \
											conf.err_pages, conf.locations);
	wbsrv.addHandler(acc_logger);
	return (virt_serv);
}

VirtServer::~VirtServer(void) {
	delete _acc_log;
	for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); ++it)
		(*it).delRedir();
}

VirtServer::VirtServer(VirtServer const & src)
	: _acc_log(src._acc_log)
	, _max_body_size(src._max_body_size)
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