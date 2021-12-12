#include "../inc/Delete.hpp"

int Delete::process(Response & resp)
{
    if (isRequestedADirectory(resp._resulting_uri))
        return (errorCode(&resp._status_code, 400));
    if (!isFileExist(resp._file))
        return (errorCode(&resp._status_code, 404));
    if (std::remove(resp._file.c_str()))
        return (errorCode(&resp._status_code, 500));
    resp._status_code = 204;
    return (processing::Type::done);
}