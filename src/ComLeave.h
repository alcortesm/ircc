#ifndef COMLEAVE_H
#define COMLEAVE_H

#include "Command.h"
#include "Server.h"

class ComLeave : public Command {
 public:
   ComLeave(Server& rServer);
   void Run();
   static const std::string STR;
 protected:
   Server& mrServer;
};

#endif /* COMLEAVE_H */
