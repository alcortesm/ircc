/* this is an abstract class to represent that commands issued by the
   user will be run() by the main_loop. */

#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command {
 protected:
   const std::string mStr;
 public:
   virtual void run(void) = 0;
   const std::string& Str() const { return mStr; };
};

#endif /* COMMAND_H */
