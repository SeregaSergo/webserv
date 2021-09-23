#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <fcntl.h>
#include "Config.hpp"

#define YYSTYPE char *

int yyparse(Config *config);

// HTTP methods defines
#define M_GET       0b00000001
#define M_POST      0b00000010
#define M_DELETE    0b00000100

// Location types defines
#define L_EQUAL     0
#define L_EXTENTION 1
#define L_PARTLY    2

#endif