#include "MsgTellUser.h"
#include <iostream>

MsgTellUser::MsgTellUser(const std::string& rPrefix, const std::string& rCommand, const std::vector<std::string>& rParams)
   : Msg(rPrefix, rCommand, rParams)
{}

void
MsgTellUser::Run()
{
   std::cout << "< " << mCommand << std::endl;
}
