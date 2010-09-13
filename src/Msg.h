#ifndef MSG_H
#define MSG_H

#include <string>
#include <vector>
#include "Server.h"

class Msg {
 public:
   Msg(const std::string& rPrefix, const std::string& rCommand, const std::vector<std::string>& rParams, Server& rServer);
   virtual ~Msg() {};

   void Run() const ;

   friend std::ostream& operator<<(std::ostream& os, const Msg& msg);

 protected:
   std::string mPrefix;
   std::string mCommand;
   std::vector<std::string> mParams;
   Server& mrServer;
};


#endif // MSG_H
