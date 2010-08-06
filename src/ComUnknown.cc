#include "ComUnknown.h"
#include <ostream>
#include <iostream>

extern std::ostream* gpDebug;

/* print the ofending command */
void
ComUnknown::run(void) {
   *gpDebug << "ComUnknown.run() invoked" << std::endl ;
   std::cout << "*** Unknown command: " << mOriginal << std::endl ;
   return ;
}
