#include "MsgTellUser.h"
#include <iostream>

MsgTellUser::MsgTellUser(const std::string& rPrefix, const std::string& rCommand, const std::vector<std::string>& rParams)
   : Msg(rPrefix, rCommand, rParams)
{}

void
MsgTellUser::Run()
{
   for(std::vector<std::string>::iterator it = mParams.begin();
       it != mParams.end();
       ++it)
      std::cout << "< " << *it << std::endl ;
}
