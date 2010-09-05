#ifndef COMCONNECT_H
#define COMCONNECT_H

#include "Command.h"
#include "Server.h"

class ComConnect : public Command {
 public:
   ComConnect(Server& server, std::string & host, std::string & port);
   void Run();
   static const std::string STR;
 private:
   Server& mrServer;
   const std::string mHost;
   const std::string mPort;
};

#endif /* COMCONNECT_H */
