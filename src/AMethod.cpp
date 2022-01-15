#include "../inc/AMethod.hpp"

int AMethod::errorCode(int * status_code, int code)
{
    *status_code = code;
    return (processing::Type::error);
}

bool AMethod::isRequestedADirectory(std::string const & uri)
{
    return (uri[uri.size() - 1] == '/');
}

bool AMethod::isFileExist(std::string const & file)
{
    struct stat buffer;   
    return (stat(&file[0], &buffer) == 0 && !(S_IFDIR & buffer.st_mode));
}