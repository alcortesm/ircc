#ifndef COMUPLOAD_H
#define COMUPLOAD_H

#include "Command.h"
#include "Server.h"
#include "DccServer.h"

class ComUpload : public Command {
 public:
   ComUpload(DccServer& rDccServer, Server& server, std::string & nick, std::string & fileName);
   void Run();
   static const std::string STR;
 private:
   DccServer& mrDccServer;
   Server& mrServer;
   const std::string mNick;
   const std::string mFileName;
};

#endif /* COMUPLOAD_H */
