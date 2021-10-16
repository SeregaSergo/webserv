#include "../inc/Logger.hpp"

Logger::Logger(int fd)
    : AFdHandler(fd), _write(false)
{
    gettimeofday(&_next_recording, NULL);
    _next_recording.tv_sec += constants::loggerTimeout;
}

void Logger::writeMsg(std::string & msg)
{
    _buffer += msg;
    if (_buffer.size() >= constants::loggerBuffer)
        _write = true;
}

void Logger::handle(bool r, bool w)
{
    if (w)
    {
        int fd = getFd();

    } 
}

void Logger::flaushBuf(void) {
    _write = true;
}