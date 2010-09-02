#ifndef COMCONNECT_H
#define COMCONNECT_H

#include "Command.h"

class ComConnect : public Command {
 public:
   ComConnect();
   void Run(void);
   static const std::string STR;
};

#endif /* COMCONNECT_H */
