#ifndef POST_HPP
#define POST_HPP

#include "AMethod.hpp"

namespace post
{
    namespace State
    {
        enum State
        {
            boundary,
            end_line,
            header_name,
            header_val,
            body,
        };
    }
}

class Post : public AMethod {

private:

    struct File_post
    {
        std::map<std::string, std::string>  headers;
        const char *                        start_pos;
        size_t                              num;

        File_post(void) : start_pos(NULL), num(0) {}
    };
    
    size_t split(const std::string &txt, std::vector<std::string> &strs, char c);
    std::string decodeURLencoded(std::string str);
    bool getBoundary(std::string & boundary, std::string const & cont_type);
    int processUrlEncoded(Response & resp);
    int processForm(Response & resp, const char * boundary);
    int parseForm(const char * body, const char * end_of_body, const char * boundary, \
                    std::vector<File_post> & files);
    bool getFilename(std::string & path, std::string & filename, std::map<std::string, std::string> & headers);
    void checkFilename(std::string & filename);

public:
    Post() {}
    virtual ~Post() {}
    virtual int process(Response & resp);
};

#endif