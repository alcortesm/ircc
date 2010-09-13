#ifndef COMLIST_H
#define COMLIST_H

#include "Command.h"
#include "Server.h"

class ComList : public Command {
 public:
   ComList(Server& rServer);
   void Run();
   static const std::string STR;
 protected:
   Server& mrServer;
};

#endif /* COMLIST_H */
