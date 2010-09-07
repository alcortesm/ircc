#ifndef COMUSER_H
#define COMUSER_H

#include "Command.h"
#include "Server.h"

class ComUser : public Command {
 public:
   ComUser(const std::string & rUser, Server& rServer);
   void Run();
   static const std::string STR;
   static const std::string COMMAND;
 private:
   Server& mrServer;
   const std::string mUser;
};

#endif /* COMUSER_H */
