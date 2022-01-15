#include "Response.hpp"

#ifndef AMETHOD_HPP
#define AMETHOD_HPP

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

class Response;

// Absract parent class for all method classes:
//      Get class
//      Post class
//      Delete class

class AMethod {

protected:
    AMethod() {}
    bool isRequestedADirectory(std::string const & resp);
    bool isFileExist(std::string const & file);
    int errorCode(int * resp, int code);
    
public:
    virtual ~AMethod() {}
    virtual int process(Response & resp) = 0;
};

#endif