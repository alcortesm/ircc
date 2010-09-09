#ifndef COMECHO_H
#define COMECHO_H

#include "Command.h"

class ComEcho : public Command {
 protected:
   std::string mMsg;
 public:
   static const std::string STR;
   ComEcho(const std::string& msg) : mMsg(msg) {};
   void Run();
};

#endif /* COMECHO_H */

