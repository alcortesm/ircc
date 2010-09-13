#include "ComUnknown.h"
#include <ostream>
#include <iostream>
#include "ircc.h"

extern std::ostream* gpDebug;

/* print the ofending command */
void
ComUnknown::Run() {
   std::cout << FROM_PROGRAM << "Unknown command: " << mOriginal << std::endl ;
   return ;
}

ComUnknown::ComUnknown(const std::string& original)
   : mOriginal(original)
{
   *gpDebug << FROM_DEBUG << "ComUnknown::ComUnknown(\""
            << mOriginal << "\")" << std::endl ;
}
