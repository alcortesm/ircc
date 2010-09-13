#include "ComLeave.h"
#include "ircc.h"
#include "irc.h"
#include <iostream>
#include <ostream>
#include <sstream>

extern std::ostream* gpDebug;

ComLeave::ComLeave(Server& rServer)
   : mrServer(rServer)
{
   *gpDebug << FROM_DEBUG << "ComLeave::ComLeave()" << std::endl;
}

/* static */
const std::string ComLeave::STR = std::string("/leave");

/* just do nothing */
void
ComLeave::Run() {

   *gpDebug << FROM_DEBUG << "ComLeave::Run()" << std::endl ;

   try {
      std::stringstream ss;
      ss << COM_JOIN << MESSAGE_SEPARATOR
         << LEAVE_ALL_CHANNELS_CHANNEL << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      std::cout << FROM_PROGRAM << "Can not set nick: not connected to server"
                << std::endl;
   } catch (Server::SendException & e) {
      std::cout << FROM_PROGRAM << "Can not set nick: " << e.what()
                << std::endl;
   } catch (Server::ConnectionClosedByPeerException& e) {
      std::cout << FROM_PROGRAM << e.what() << std::endl;
   }

   return ;
}
