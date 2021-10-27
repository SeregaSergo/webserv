#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <sys/time.h>

namespace constants
{
    extern time_t          timeout_idle;
    extern time_t          timeout_ka;
    extern int             ka_time;
    extern int             ka_probes;
    extern int             ka_interval;
    extern const char *    default_file;
}
 
#endif