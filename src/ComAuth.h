#ifndef COMAUTH_H
#define COMAUTH_H

#include "Command.h"
#include "Server.h"

class ComAuth : public Command {
 public:
   ComAuth(const std::string& rNick, Server& rServer);
   void Run();
   static const std::string STR;
 private:
   const std::string mNick;
   Server& mrServer;
};

#endif /* COMAUTH_H */
