#include "ComError.h"
#include <ostream>
#include <iostream>

extern std::ostream* gpDebug;

/* print the message */
void
ComError::Run() {
   std::cout << "*** " << mMsg << std::endl ;
   return ;
}
