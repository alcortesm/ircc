#include "ComUnknown.h"
#include <ostream>
#include <iostream>

extern std::ostream* gpDebug;

/* print the ofending command */
void
ComUnknown::Run(void) {
   std::cout << "*** Unknown command: " << mOriginal << std::endl ;
   return ;
}
