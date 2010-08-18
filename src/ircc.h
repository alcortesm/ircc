#ifndef IRCC_H
#define IRCC_H

#include <string>

// mix constants
const bool   DEF_DEBUG = false;
const int    STDIN_FD = 0;
const time_t MAIN_LOOP_TIMEOUT_SECS  = 2;
const long   MAIN_LOOP_TIMEOUT_USECS = 0;
const size_t MAX_LINE_LEN = 1023;
const std::string USAGE("usage:\n\tircc [OPTIONS]\n\n\tOPTIONS:\n\t\t-t\n\t\t-d");
#endif
