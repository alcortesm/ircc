#include "ComDisconnect.h"
#include "ircc.h"
#include "irc.h"
#include <ostream>
#include <sstream>
#include <iostream>

extern std::ostream* gpDebug;

ComDisconnect::ComDisconnect(Server& rServer)
   : mrServer(rServer)
{
   *gpDebug << FROM_DEBUG << "ComDisconnect::ComDisconnect("
            << mrServer << ")" << std::endl;
}

/* static */
const std::string ComDisconnect::STR = std::string("/disconnect");

/* just do nothing */
void
ComDisconnect::Run() {
   try {

      std::stringstream ss;
      ss << COM_QUIT << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);
      mrServer.Disconnect();

   } catch (Server::NotConnectedException& e) {
      std::cout << FROM_PROGRAM << "Not connected to any server"
                << std::endl;
   }
   return ;
}
