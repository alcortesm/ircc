#include "ComQuit.h"
#include <ostream>
#include "ircc.h"

extern std::ostream* gpDebug;

ComQuit::ComQuit()
{
   *gpDebug << FROM_DEBUG << "ComQuit::ComQuit()" << std::endl;
}

/* static */
const std::string ComQuit::STR = std::string("/quit");

/* just do nothing */
void
ComQuit::Run() {
   *gpDebug << FROM_DEBUG << "ComQuit::Run(): do nothing" << std::endl ;
   return ;
}

bool
ComQuit::MustQuit() const
{
   return true;
}
