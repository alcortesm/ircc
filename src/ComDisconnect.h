#ifndef COMDISCONNECT_H
#define COMDISCONNECT_H

#include "Command.h"
#include "Server.h"

class ComDisconnect : public Command {
 public:
   ComDisconnect(Server& rServer);
   void Run();
   static const std::string STR;
 protected:
   Server& mrServer;
};

#endif /* COMDISCONNECT_H */
