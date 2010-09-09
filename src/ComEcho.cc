#include "ComEcho.h"
#include <ostream>
#include <iostream>

extern std::ostream* gpDebug;

const std::string ComEcho::STR("/echo");

/* print the message */
void
ComEcho::Run() {
   std::cout << mMsg << std::endl ;
   return ;
}
