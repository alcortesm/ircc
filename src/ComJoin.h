#ifndef COMJOIN_H
#define COMJOIN_H

#include "Command.h"
#include "Server.h"

class ComJoin : public Command {
 public:
   ComJoin(const std::string & rChannel, Server& rServer);
   void Run();
   static const std::string STR;
 private:
   Server& mrServer;
   const std::string mChannel;
};

#endif /* COMJOIN_H */
