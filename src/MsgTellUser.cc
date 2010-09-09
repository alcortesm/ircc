#include "MsgTellUser.h"
#include <iostream>
#include "NullStream.h"
#include "ircc.h"

extern std::ostream* gpDebug;
using std::string;

MsgTellUser::MsgTellUser(
                         const std::string& rPrefix,
                         const std::string& rCommand,
                         const std::vector<std::string>& rParams)
   : Msg(rPrefix, rCommand, rParams)
{}

void
MsgTellUser::Run()
{
   std::cout << FROM_SERVER << "[" << mCommand << "] " ;
   for(std::vector<std::string>::iterator it = mParams.begin();
       it != mParams.end();
       ++it) {
      if (it != mParams.begin())
         std::cout << " " ;

      std::cout << *it ;
   }
   std::cout << std::endl;
}
