/* this is an abstract class to represent that commands issued by the
   user will be run() by the main_loop. */

#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command {
 public:
   virtual void run(void) = 0;
   virtual bool MustQuit() const { return false ; };
};

#endif /* COMMAND_H */
