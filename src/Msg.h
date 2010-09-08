#ifndef MSG_H
#define MSG_H

#include <string>
#include <vector>

class Msg {
 public:
   Msg(const std::string& rPrefix, const std::string& rCommand, const std::vector<std::string>& rParams);
   virtual ~Msg() {};
   virtual void Run() = 0;
 protected:
   std::string mPrefix;
   std::string mCommand;
   std::vector<std::string> mParams;
};

#endif // MSG_H
