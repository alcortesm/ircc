#include "ComNop.h"
#include "ircc.h"
#include <ostream>

extern std::ostream* gpDebug;

ComNop::ComNop() {}

/* static */
const std::string ComNop::STR = std::string("/nop");

/* just do nothing */
void
ComNop::Run() {
   *gpDebug << FROM_DEBUG << "ComNop::Run(): do nothing" << std::endl ;
   return ;
}
