#include "ComList.h"
#include "ircc.h"
#include "irc.h"
#include <ostream>
#include <sstream>
#include <iostream>

extern std::ostream* gpDebug;

ComList::ComList(Server& rServer)
   : mrServer(rServer)
{
   *gpDebug << FROM_DEBUG << "ComList::ComList("
            << mrServer << ")" << std::endl;
}

/* static */
const std::string ComList::STR = std::string("/list");

/* just do nothing */
void
ComList::Run() {

   *gpDebug << FROM_DEBUG << "ComList::Run()" << std::endl ;

   try {
      std::stringstream ss;
      ss << COM_LIST << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      std::cout << FROM_PROGRAM << "Unable to list channels: not connected to server"
                << std::endl;
   } catch (Server::SendException & e) {
      std::cout << FROM_PROGRAM << "Unable to list channels: " << e.what()
                << std::endl;
   } catch (Server::ConnectionClosedByPeerException& e) {
      std::cout << FROM_PROGRAM << e.what() << std::endl;
   }

   return ;
}
