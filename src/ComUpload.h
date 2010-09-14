#ifndef COMUPLOAD_H
#define COMUPLOAD_H

#include "Command.h"
#include "Server.h"

class ComUpload : public Command {
 public:
   ComUpload(Server& server, std::string & nick, std::string & fileName);
   void Run();
   static const std::string STR;
 private:
   Server& mrServer;
   const std::string mNick;
   const std::string mFileName;
};

#endif /* COMUPLOAD_H */
