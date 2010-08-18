#include "ComQuit.h"
#include <ostream>

extern std::ostream* gpDebug;

ComQuit::ComQuit() {}

/* static */
const std::string ComQuit::STR = std::string("/quit");

/* just do nothing */
void
ComQuit::run(void) {
   *gpDebug << "ComQuit.run(): do nothing" << std::endl ;
   return ;
}

bool
ComQuit::MustQuit() const
{
   return true;
}
