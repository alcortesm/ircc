#include "ComWho.h"
#include "ircc.h"
#include "irc.h"
#include <ostream>
#include <sstream>
#include <iostream>

extern std::ostream* gpDebug;

ComWho::ComWho(Server& rServer)
   : mrServer(rServer)
{
   *gpDebug << FROM_DEBUG << "ComWho::ComWho("
            << mrServer << ")" << std::endl;
}

/* static */
const std::string ComWho::STR = std::string("/who");

/* just do nothing */
void
ComWho::Run() {

   *gpDebug << FROM_DEBUG << "ComWho::Run()" << std::endl ;

   if (mrServer.IsChannelClear()) {
      std::cout << FROM_PROGRAM << "You are not in any channel"
                << std::endl;
      return;
   }

   try {
      std::stringstream ss;
      ss << COM_WHO << MESSAGE_SEPARATOR
         << mrServer.GetChannel() << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      std::cout << FROM_PROGRAM << "Unable to get list of users: not connected to server"
                << std::endl;
   } catch (Server::SendException & e) {
      std::cout << FROM_PROGRAM << "Unable to get list of users: " << e.what()
                << std::endl;
   } catch (Server::ConnectionClosedByPeerException& e) {
      std::cout << FROM_PROGRAM << e.what() << std::endl;
   }

   return ;
}
