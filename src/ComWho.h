#ifndef COMWHO_H
#define COMWHO_H

#include "Command.h"
#include "Server.h"

class ComWho : public Command {
 public:
   ComWho(Server& rServer);
   void Run();
   static const std::string STR;
 protected:
   Server& mrServer;
};

#endif /* COMWHO_H */
