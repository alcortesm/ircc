/* this is an abstract class to represent that commands issued by the
   user will be run() by the main_loop. */

#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command {
 public:
   virtual void Run() = 0;
   virtual bool MustQuit() const { return false ; };
   virtual ~Command() {};
};

#endif /* COMMAND_H */
