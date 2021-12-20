#include "../inc/OutCGI.hpp"

bool OutCGI::checkTimeout(struct timeval & cur_time)
{
    if ((cur_time.tv_sec - _time.tv_sec) > _timeout)
    {
        (_resp.*_callback_func)(504);
        return (true);
    }
    return (false);
}

void OutCGI::handle(void)
{
    std::cout << "[fd "<< _fd << "] OutCGI reading" << std::endl;
    int ret = read(_fd, _buffer, constants::CGI_bufsize);
    std::cout << ret << std::endl;
    if (ret < 0)
    {
        (_resp.*_callback_func)(502);
        return;
    }
    if (ret == 0)
    {
        (_resp.*_callback_func)(0);
        return;
    }
    std::ostream & r = _stream.write(_buffer, ret);
}