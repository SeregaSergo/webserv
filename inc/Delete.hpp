#ifndef DELETE_HPP
#define DELETE_HPP

#include "AMethod.hpp"
#include <cstdio>

class Delete : public AMethod {

private:
    
public:
    Delete() {}
    virtual ~Delete() {}
    virtual int process(Response & resp);
};

#endif