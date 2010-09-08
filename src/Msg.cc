#include "Msg.h"

Msg::Msg(const std::string& rPrefix, const std::string& rCommand, const std::vector<std::string>& rParams)
   : mPrefix(rPrefix), mCommand(rCommand), mParams(rParams)
{}
