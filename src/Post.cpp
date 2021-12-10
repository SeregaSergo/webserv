#include "../inc/Post.hpp"

std::string Post::getBoundary(std::string const & cont_type)
{
    size_t pos = cont_type.find("boundary=", 0);
    if (pos == std::string::npos)
    {
        // _status_code = 400;
        return ("");
    }
    else
        return (cont_type.substr(pos + 9, std::string::npos));
}

size_t Post::split(const std::string &txt, std::vector<std::string> &strs, char c)
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

int Post::process(Response & resp)
{
    (void) resp;
    return (0);
}