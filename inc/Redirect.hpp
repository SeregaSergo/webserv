#ifndef REDIRECT_HPP
#define REDIRECT_HPP

#include <vector>
#include <iostream>

class Redirect {

private:
    int             _code;
    std::string     _url;
    Redirect();

public:
    Redirect(int code, std::string url);
    ~Redirect();
    Redirect(Redirect const & src);
	Redirect & operator=(Redirect const & src);

    // std::string rewrite_url();
    // int get_code();
};

#endif