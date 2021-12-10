#include "../inc/AMethod.hpp"

int AMethod::errorCode(Response & resp, int code)
{
    resp._status_code = code;
    return (processing::Type::error);
}

bool AMethod::isRequestedADirectory(Response & resp)
{
    return (resp._resulting_uri[resp._resulting_uri.size() - 1] == '/');
}

bool AMethod::isFileExist(std::string const & temp_file)
{
    struct stat buffer;   
    return (stat(&temp_file[0], &buffer) == 0);
}