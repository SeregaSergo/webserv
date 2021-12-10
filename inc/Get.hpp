#ifndef GET_HPP
#define GET_HPP

#include "AMethod.hpp"

class Get : public AMethod {

private:
    int findFile(Response & resp);
    const std::string & getMimeType(std::string const & uri);

public:
    Get() {}
    virtual ~Get() {}
    virtual int process(Response & resp);
};

#endif