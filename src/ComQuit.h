#ifndef COMQUIT_H
#define COMQUIT_H

#include "Command.h"

class ComQuit : public Command {
 public:
   ComQuit();
   void Run();
   static const std::string STR;
   bool MustQuit() const;
};

#endif /* COMQUIT_H */
