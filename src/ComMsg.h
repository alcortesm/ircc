#ifndef COMMSG_H
#define COMMSG_H

#include "Server.h"
#include "Command.h"

class ComMsg : public Command {
 protected:
   const std::string mMsg; /* the original message to send */
   Server& mrServer;
   static const std::string COMMAND;
 public:
   ComMsg(const std::string& rMsg, Server& rServer)
      : mMsg(rMsg), mrServer(rServer) {};
   static const std::string STR;
   void Run();
};

#endif /* COMMSG_H */
