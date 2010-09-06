#ifndef COMNICK_H
#define COMNICK_H

#include "Command.h"
#include "Server.h"

class ComNick : public Command {
 public:
   ComNick(std::string & rNick, Server& server);
   void Run();
   static const std::string STR;
   static const std::string COMMAND;
 private:
   Server& mrServer;
   const std::string mNick;
};

#endif /* COMNICK_H */
