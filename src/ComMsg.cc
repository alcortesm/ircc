#include "ComMsg.h"
#include <ostream>
#include <iostream>

extern std::ostream* gpDebug;

const std::string ComMsg::STR = std::string("/msg");

/* Send the message to the server */
void
ComMsg::Run() {

   *gpDebug << "ComMsg::Run() : will try to send \"" << mMsg << "\" to "
            << mrServer << std::endl ;

   try {

      mrServer.Send(mMsg);

   } catch (Server::NotConnectedException& e) {
      std::cout << "*** Can not send message: " << e.what() << std::endl;
   }

   return ;
}
