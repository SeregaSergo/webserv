#include "../inc/Post.hpp"

bool Post::getBoundary(std::string & boundary, std::string const & cont_type)
{
    size_t pos = cont_type.find("boundary=", 0);
    if (pos == std::string::npos)
        return (false);
    else
        boundary = "--" + cont_type.substr(pos + 9);
    return (true);
}

size_t Post::split(const std::string & txt, std::vector<std::string> &strs, char c)
{
    size_t pos = txt.find(c);
    size_t initialPos = 0;

    while( pos != std::string::npos ) {
        strs.push_back(txt.substr( initialPos, pos - initialPos) );
        initialPos = pos + 1;

        pos = txt.find(c, initialPos);
    }
    strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

    return strs.size();
}

std::string Post::decodeURLencoded(std::string str)
{
    size_t pos = str.find('%');
    std::stringstream stream;
    int num_symbol;

    while( pos != std::string::npos ) {
        stream << str.substr(pos + 1, 2);
        stream >> std::hex >> num_symbol;
        str.erase(pos, 3);
        str.insert(pos, 1, (char)num_symbol);
        pos = str.find('%');
    }
    return (str);
}

int Post::processUrlEncoded(Response & resp)
{
    std::ofstream file(resp._file.c_str(), std::ios::trunc);
    if (file.is_open())
    {
        std::vector<std::string>    tuples;
        split(resp._request->getBody(), tuples, '&');
        std::vector<std::string>::iterator last = --tuples.end();
        for (std::vector<std::string>::iterator it = tuples.begin(); it != tuples.end(); ++it)
        {
            file << decodeURLencoded(*it) << (it != last ? "\n" : "");
            if (file.fail())
            {
                file.close();
                return (errorCode(&resp._status_code, 500));
            }
        }
        file.close();
        return (processing::Type::done);
    }
    else
        return (errorCode(&resp._status_code, 403)); // Forbidden
}

int Post::parseForm(const char * body, const char * end_of_body, const char * boundary, \
                    std::vector<File_post> & files)
{
    int                     state = post::State::boundary;
    bool                    parse = true;

    std::string             temp_header_name;
    const char *            pos;
    size_t                  boundary_size = strlen(boundary);

    if (strncmp(body, boundary, boundary_size))
        return (processing::Type::error);
    body += boundary_size;

    while (parse)
    {
        switch (state)
        {
        case post::State::boundary:
            if (!strncmp(body, "\r\n", 2))
            {
                files.push_back(File_post());
                state = post::State::end_line;
            }
            else if (!strncmp(body, "--", 2))
                parse = false;
            else
                return (processing::Type::error);
            body += 2;
            break;

        case post::State::end_line:
            if (!strncmp(body, "\r\n", 2))
            {
                body += 2;
                state = post::State::body;
            }
            else
                state = post::State::header_name;
            break;

        case post::State::header_name:
            pos = strchr(body, ':');
            if (pos == NULL)
                return (processing::Type::error);
            temp_header_name = std::string(body, pos);
            body = pos + 1;
            state = post::State::header_val;
            break;
        
        case post::State::header_val:
            pos = strstr(body, "\r\n");
            if (pos == NULL)
                return (processing::Type::error);
            files.back().headers[temp_header_name] = std::string(body, pos);
            body = pos + 2;
            state = post::State::end_line;
            break;

        case post::State::body:
            // Using another function because of byte stream
            pos = (const char *)memmem(body, end_of_body - body, \
                                        boundary, boundary_size);
            if (pos == NULL)
                return (processing::Type::error);
            files.back().start_pos = body;
            if ((pos - body) < 2)
                return (processing::Type::error);
            files.back().num = pos - body - 2;
            body = pos + boundary_size;
            state = post::State::boundary;
            break;
        }
    }
    return (processing::Type::file);
}

void Post::checkFilename(std::string & filename)
{
    size_t  number = 1;
    std::string name;
    std::string ext;

    std::size_t found = filename.find_last_of(".");
    if (found == std::string::npos)
        name = filename;
    else
    {
        name = filename.substr(0, found);
        ext = filename.substr(found);
    }

    while (isFileExist(filename))
    {
        filename = name + numToStr(number) + ext;
        ++number;
    }
}

bool Post::getFilename(std::string & path, std::string & filename, std::map<std::string, std::string> & headers)
{
    std::map<std::string, std::string>::iterator ptr_header = headers.find("Content-Disposition");
    if (ptr_header == headers.end())
        return (false);
    size_t  begin = ptr_header->second.find("filename=\"");
    if (begin == std::string::npos)
        return (false);
    begin += 10;
    size_t  end = ptr_header->second.find('"', begin);
    if (end == std::string::npos)
        return (false);

    filename = path + std::string(ptr_header->second, begin, end - begin);
    if (filename.empty() || filename[filename.size() - 1] == '/')
        return (false);
    checkFilename(filename);
    return (true);
}

int Post::processForm(Response & resp, const char * boundary)
{
    std::vector<File_post>  files;
    std::string             filename;
    const std::string &     body = resp._request->getBody();
    if (parseForm(body.c_str(), &body[body.size()], boundary, files) == processing::Type::error)
        return (errorCode(&resp._status_code, 403));

    for (std::vector<File_post>::iterator it = files.begin(); it != files.end(); ++it)
    {
        if (!getFilename(resp._file, filename, it->headers))
            continue;
        std::ofstream file(filename.c_str(), std::ios::trunc);
        if (file.is_open())
        {
            if (file.write(it->start_pos, it->num).fail())
            {
                file.close();
                std::remove(resp._file.c_str());
                return (errorCode(&resp._status_code, 500));
            }
            file.close();
        }
        else
            return (errorCode(&resp._status_code, 403)); // Forbidden
    }
    resp._status_code = 204;
    return (processing::Type::done);
}

int Post::process(Response & resp)
{
    size_t pos = 0;
    std::string content_type;
    std::map<std::string,std::string>::const_iterator it = resp._request->getHeaders().find("Content-Type");
    
    if (it == resp._request->getHeaders().end())
        return (errorCode(&resp._status_code, 415));    // Unsupported Media Type
    if ((pos = it->second.find(';')) != std::string::npos)
        content_type = it->second.substr(0, pos);
    else
        content_type = it->second;
    
    if (content_type == "application/x-www-form-urlencoded")
    {
        if (isRequestedADirectory(resp._resulting_uri))
            return (errorCode(&resp._status_code, 400));
        if (isFileExist(resp._file))
            return (errorCode(&resp._status_code, 409));    // Conflict ?
        return (processUrlEncoded(resp));
    }

    else if (content_type == "multipart/form-data")
    {
        if (!isRequestedADirectory(resp._resulting_uri))
            return (errorCode(&resp._status_code, 400));
        std::string boundary;
        if (!getBoundary(boundary, it->second))
            return (errorCode(&resp._status_code, 400));
        return (processForm(resp, boundary.c_str()));
    }

    else
        return (errorCode(&resp._status_code, 415));    // Unsupported Media Type
}