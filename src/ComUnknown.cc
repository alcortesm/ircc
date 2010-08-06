#include "ComUnknown.h"
#include <ostream>
#include <iostream>

extern std::ostream* gpDebug;

ComUnknown::ComUnknown() {}

/* just do nothing */
void
ComUnknown::run(void) {
   *gpDebug << "ComUnknown.run() invoked" << std::endl ;
   std::cout << "*** Unknown command" << std::endl ;
   return ;
}
