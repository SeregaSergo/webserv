#include "../inc/Get.hpp"

int Get::findFile(Response & resp)
{
    std::vector<std::string>    index_files = resp._location->getIndexFiles();
    std::string                 temp_uri;
    Location *                  temp_loc;
    std::string                 temp_file;
    while (!index_files.empty())
    {
        temp_uri = resp._resulting_uri + index_files.front();
        temp_loc = resp._virt_serv->chooseLocation(temp_uri);
        temp_file = temp_loc->getResoursePath(temp_uri);
        if (isFileExist(temp_file))
        {
            resp._location = temp_loc;
            resp._resulting_uri = temp_uri;
            resp._file = temp_file;
            return (processing::Type::file);
        }
        index_files.erase(index_files.begin());
    }
    if (index_files.empty() && resp._location->getAutoindex())
        return (processing::Type::autoindex);
    else
        return (errorCode(&resp._status_code, 404));
}

const std::string & Get::getMimeType(std::string const & uri)
{
    std::size_t found = uri.find_last_of(".");
    std::map<std::string, std::string>::iterator it;
    if (found == std::string::npos)
        it = constants::mime_types.find("");
    else if ((it = constants::mime_types.find(uri.substr(found + 1))) == constants::mime_types.end())
        it = constants::mime_types.find("default");
    return (it->second);
}

int Get::process(Response & resp)
{
    if (isRequestedADirectory(resp._resulting_uri))
    {
        int ret = findFile(resp);
        if (ret != processing::Type::file)
            return (ret);
    }

    std::ifstream file(resp._file.c_str());
    if (file.is_open())
    {
        resp._body << file.rdbuf();
        if (file.fail())
        {
            file.close();
            return (errorCode(&resp._status_code, 500));
        }
        std::cout << getMimeType(resp._resulting_uri) << std::endl;
        resp._headers["Content-Type"] = getMimeType(resp._resulting_uri);
        file.close();
    }
    else
        return (errorCode(&resp._status_code, 404));
    return (processing::Type::done);
}