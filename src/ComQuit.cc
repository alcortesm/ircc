#include "ComQuit.h"
#include <ostream>

extern std::ostream* gpDebug;

ComQuit::ComQuit() {}

/* static */
const std::string ComQuit::STR = std::string("/quit");

/* just do nothing */
void
ComQuit::Run(void) {
   *gpDebug << "ComQuit::Run(): do nothing" << std::endl ;
   return ;
}

bool
ComQuit::MustQuit() const
{
   return true;
}
