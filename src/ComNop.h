#ifndef COMNOP_H
#define COMNOP_H

#include "Command.h"

class ComNop : public Command {
 public:
   ComNop();
   void Run();
   static const std::string STR;
};

#endif /* COMNOP_H */
