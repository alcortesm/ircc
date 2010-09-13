#ifndef COMHELP_H
#define COMHELP_H

#include "Command.h"

class ComHelp : public Command {
 protected:
   static const std::string MSG;
 public:
   ComHelp();
   static const std::string STR;
   void Run();
};

#endif /* COMHELP_H */

