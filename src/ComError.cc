#include "ComError.h"
#include <ostream>
#include <iostream>
#include "ircc.h"

extern std::ostream* gpDebug;

/* print the message */
void
ComError::Run() {
   std::cout << FROM_PROGRAM << mMsg << std::endl ;
   return ;
}
