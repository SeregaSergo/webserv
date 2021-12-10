#ifndef POST_HPP
#define POST_HPP

#include "AMethod.hpp"

class Post : public AMethod {

private:
    size_t split(const std::string &txt, std::vector<std::string> &strs, char c);
    std::string decodeURLencoded(std::string str);
    std::string getBoundary(std::string const & cont_type);
    
public:
    Post() {}
    virtual ~Post() {}
    virtual int process(Response & resp);
};

#endif