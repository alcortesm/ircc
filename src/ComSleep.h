#ifndef COMSLEEP_H
#define COMSLEEP_H

#include "Command.h"

class ComSleep : public Command {
 protected:
   const int mSecs; /* seconds to sleep */
 public:
   ComSleep(int secs) : mSecs(secs) {};
   static const std::string STR;
   static const int DEFAULT_SLEEP_TIME_SECS;
   void Run();
};

#endif /* COMSLEEP_H */

