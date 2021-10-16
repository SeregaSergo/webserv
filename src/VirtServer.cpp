#include "../inc/VirtServer.hpp"

void VirtServer::sendAccMsg(std::string & msg) {
	_acc_log->writeMsg(msg);
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

void VirtServer::flushLoggerBuffers(void) {
	_acc_log->flushBuf();
}