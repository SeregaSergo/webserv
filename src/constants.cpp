#include "../inc/constants.hpp"

namespace constants
{
    time_t          timeout_idle = 10;
    time_t          timeout_ka = 86400; // 24 h
    int             ka_time = 600;		// every 10 min
    int             ka_probes = 5;
    int             ka_interval = 30;
    const char *    default_file = "/dev/null";
}
