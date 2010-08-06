#ifndef COMUNKNOWN_H
#define COMUNKNOWN_H

#include "Command.h"

class ComUnknown : public Command {
 protected:
   const std::string mOriginal; /* the original command that is unknown */
 public:
   ComUnknown(const std::string& original) : mOriginal(original) {};
   void run(void);
};

#endif /* COMUNKNOWN_H */
