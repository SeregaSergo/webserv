#include "Request.hpp"

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <map>
#include "constants.hpp"
#include "InCGI.hpp"
#include "OutCGI.hpp"
#include "string.h"
#include <algorithm>

class Client;
class Request;
class Location;

class Response {

private:
    Request *                           _request;
    int *                               _client_state;

    // Data members for sending
    std::string                         _response;
    unsigned long                       _sent;

    // Data for constructing a response
    int                                 _status_code;
    std::map<std::string,std::string>   _headers;
    std::stringstream                   _body;

    // Data for file processing
    VirtServer *                        _virt_serv;
    Location *                          _location;
    std::string                         _resulting_uri;
    std::string                         _file;

    // Data for CGI processing
    InCGI *                             _in_CGI;
    OutCGI *                            _out_CGI;
    int                                 _pid;

    // Methods implementation
    int processMethod(void);

    // Redirections
    void processRedirection(void);

    // Assembling response
    void handleError(void);
    void assembleResponse(void);

    // CGI methods
    bool runCGI(void);
    void finishCGI(void);
    int processResponseCGI(void);
    bool parseHeaderLine(std::string & line);
    void removeInCGI(void);
    void removeOutCGI(void);
    void callbackFuncInCGI(int ret);
    void callbackFuncOutCGI(int ret);
    char * const * getArgv(std::vector<char*> & argv);
    char * const * getEnvp(std::vector<char*> & envp);
	void put_env_into_vec(std::vector<char *> &envp, std::string new_env);
    
    Response(void) {}

public:
    Response(Request * req, int * cl_state);
    Response(Response const & src);
    ~Response(void);

    void processRequest(void);
    int sendResponse(int fd);
    void clear(void);

    int getStatusCode(void);
    unsigned long getBytesSent(void);
    
    friend class Get;
    friend class Post;
    friend class Delete;
    friend std::ostream & operator<<(std::ostream & o, Response const & resp);
};

#endif