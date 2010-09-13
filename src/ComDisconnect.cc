#include "ComDisconnect.h"
#include "ircc.h"
#include <ostream>
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
      mrServer.Disconnect();
   } catch (Server::NotConnectedException& e) {
      std::cout << FROM_PROGRAM << "Not connected to any server"
                << std::endl;
   }
   return ;
}
