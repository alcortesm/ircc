#include "CommandFactory.h"
#include "ComNop.h"
#include "ComUnknown.h"
#include <ostream>

extern std::ostream* gpDebug;

using std::endl;

Command*
CommandFactory::Build(const std::string& rLine)
   throw (std::runtime_error)
{
   /* if the user pressed only enter: nop command */
   if (rLine.empty()) {
      *gpDebug << "CommandFactory::Build(): nop presumed" << endl ;
      return new ComNop();
   }


   /* nop */
   if (rLine == ComNop::STR) {
      *gpDebug << "CommandFactory::Build(): nop detected" << endl ;
      return new ComNop();
   }
   
   /* nop + ' ' + none or more chars : nop command */
   size_t str_len = ComNop::STR.length();
   if (rLine.length() > str_len) {
      if (rLine.compare(0, str_len, ComNop::STR) == 0 && rLine.at(str_len) == ' ') {
         *gpDebug << "CommandFactory::Build(): nop + some chars detected" << endl ;
         return new ComNop();
      }
   }
          
   *gpDebug << "CommandFactory::Build(): unknown command" << endl ;
   return new ComUnknown(rLine);
}
