#include "../inc/Logger.hpp"

void Logger::sendMsg(std::string const & msg)
{
    _buffer += getDateTime();
    _buffer += msg;
    _buffer.push_back('\n');
    if (_buffer.size() >= constants::loggerLimit)
        _write = true;
}

void Logger::handle(bool r, bool w)
{
    int ret = 0;
    int buf_size = _buffer.size();
    (void)r;
    (void)w;
    
    ret = write(_fd, _buffer.c_str(), buf_size);
    if (ret == buf_size || ret == -1)
    {
        _write = false;
        _buffer.clear();
        gettimeofday(&_time, NULL);
    }
    else
        _buffer.erase(0, ret);
}

bool Logger::checkTimeout(struct timeval & cur_time) {
    if (_time.tv_sec + constants::loggerTimeout < cur_time.tv_sec)
        _write = true;
    return (false);
}

const char * Logger::getDateTime(void)
{
    time_t      now = time(0);
    struct tm   tstruct;
    static char buf[64];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "[%Y/%m/%d-%X] ", &tstruct);
    return buf;
}