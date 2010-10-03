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
const std::string TCP("tcp");
const size_t PROTOENT_BUF_SZ = 1024;
const char   SPACE = ' ';
const int    NO_FLAGS = 0;
const std::string FROM_SERVER("<<< ");
const std::string FROM_PROGRAM("*** ");
const std::string FROM_DEBUG("DDD ");
const std::string IRCC_HOST_PORT_SEPARATOR(":");
const int BACK_LOG = 10;
const std::string DCC_SERVER_PORT("8101"); // 8000 + group id
#endif // IRCC_H
