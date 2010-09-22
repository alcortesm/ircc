#ifndef COMOFFER_H
#define COMOFFER_H

#include "Command.h"
#include "Server.h"
#include "DccServer.h"

class ComOffer : public Command {
 public:
   ComOffer(DccServer& rDccServer, Server& server, std::string & nick, std::string & fileName);
   void Run();
   static const std::string STR;
 private:
   DccServer& mrDccServer;
   Server& mrServer;
   const std::string mNick;
   const std::string mFileName;
};

#endif /* COMOFFER_H */
