#include "ComNop.h"
#include <ostream>

extern std::ostream* gpDebug;

ComNop::ComNop() {}

/* static */
const std::string ComNop::STR = std::string("/nop");

/* just do nothing */
void
ComNop::Run() {
   *gpDebug << "ComNop::Run(): do nothing" << std::endl ;
   return ;
}
