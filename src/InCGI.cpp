#include "../inc/InCGI.hpp"

bool InCGI::checkTimeout(struct timeval & cur_time)
{
    if ((cur_time.tv_sec - _time.tv_sec) > _timeout)
    {
        (_resp.*_callback_func)(504);
        return (true);
    }
    return (false);
}

void InCGI::handle(void)
{
    std::cout << "[fd "<< _fd << "] InCGI writting" << std::endl;
    int ret = write(_fd, &_body[_sent], _body.size() - _sent);
    std::cout << ret << std::endl;
    if (ret < 0)
    {
        (_resp.*_callback_func)(502);
        return;
    }
    _sent += ret;
    if (_sent == _body.size())
        (_resp.*_callback_func)(0);
}