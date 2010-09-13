#ifndef COMERROR_H
#define COMERROR_H

#include "Command.h"

class ComError : public Command {
 protected:
   const std::string mMsg; /* the message to show to the user */
 public:
   ComError(const std::string& msg);
   void Run();
};

#endif /* COMERROR_H */

