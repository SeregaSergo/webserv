#ifndef REDIRECT_HPP
#define REDIRECT_HPP

#include <vector>
#include <iostream>

class Redirect {

private:
    int             _code;
    std::string     _url;
    bool            _uri;
    Redirect();

public:
    Redirect(int code, std::string url, bool uri) : _code(code), _url(url), _uri(uri) {}
    ~Redirect();
    Redirect(Redirect const & src);
	Redirect & operator=(Redirect const & src);

    // std::string rewrite_url();
    // int get_code();
};

#endif