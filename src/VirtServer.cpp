#include "../inc/VirtServer.hpp"

VirtServer::VirtServer(Logger * logger, long long int body_size, std::map<int, \
			std::string> & err_pages, std::vector<Location> & locations)
		: _acc_log(logger)
		, _max_body_size(body_size)
		, _err_pages(err_pages)
		, _locations(locations)
{

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

const Location * VirtServer::chooseLocation(std::string const & uri)
{
	std::vector<Location>::iterator winner_pos;
	char ret;
	char win_prior = 0;
	for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); ++it)
	{
		ret = (*it).checkLocation(uri);
		if (ret > win_prior)
			winner_pos = it;
	}
	if (win_prior == 0)
		return NULL;
	return (winner_pos.base());	
}

bool VirtServer::IsBodyOversize(long long int body_size) {
	return (body_size > _max_body_size);
}
