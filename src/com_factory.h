#ifndef COM_FACTORY_H
#define COM_FACTORY_H

#include "Command.h"
#include "Server.h"
#include <stdexcept>

Command* com_factory(const std::string& rLine, Server& rServer);

#endif /* COM_FACTORY_H */
