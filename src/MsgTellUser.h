#ifndef MSGTELLUSER_H
#define MSGTELLUSER_H

#include "Msg.h"

// This is a placeholder for the many server messages that we only
// need to show to the user
class MsgTellUser : public Msg {
 public:
   MsgTellUser(
               const std::string& rPrefix,
               const std::string& rCommand,
               const std::vector<std::string>& rParams);
   void Run();
};

#endif /* MSGTELLUSER_H */
