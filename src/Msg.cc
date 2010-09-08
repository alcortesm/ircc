#include "Msg.h"

Msg::Msg(std::string& rPrefix, std::string& rCommand, std::vector<std::string>& rParams)
   : mPrefix(rPrefix), mCommand(rCommand), mParams(rParams)
{}
