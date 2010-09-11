#ifndef MSG_H
#define MSG_H

#include <string>
#include <vector>
#include "Server.h"

class Msg {
 public:
   Msg(const std::string& rPrefix, const std::string& rCommand, const std::vector<std::string>& rParams);
   virtual ~Msg() {};

   void Run(Server& rServer) const ;

   friend std::ostream& operator<<(std::ostream& os, Msg& msg);

 protected:
   std::string mPrefix;
   std::string mCommand;
   std::vector<std::string> mParams;
};


#endif // MSG_H
