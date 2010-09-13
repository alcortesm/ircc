#ifndef MSG_FACTORY_H
#define MSG_FACTORY_H

#include <string>
#include "Msg.h"
#include "Server.h"

const Msg msg_factory(const std::string& rLine, Server& rServer);

#endif // MSG_FACTORY_H
