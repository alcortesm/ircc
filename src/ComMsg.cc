#include "ComMsg.h"
#include <ostream>
#include <iostream>
#include <sstream>
#include "irc.h"
#include "ircc.h"

extern std::ostream* gpDebug;

const std::string ComMsg::STR = std::string("/msg");
const std::string ComMsg::COMMAND = std::string("PRIVMSG");

void
ComMsg::Run() {

   *gpDebug << FROM_DEBUG << "ComMsg::Run()" << std::endl ;

   if (mrServer.GetChannel().empty()) {
      std::cout << FROM_PROGRAM << "Error: you must join a channel first"
                << std::endl;
      return;
   }

   try {

      std::stringstream ss;
      ss << ComMsg::COMMAND << MESSAGE_SEPARATOR
         << mrServer.GetChannel() << MESSAGE_SEPARATOR
         << ":" << mMsg << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      std::cout << FROM_PROGRAM << "Can not send message: not connected to server"
                << std::endl;
   } catch (Server::SendException & e) {
      std::cout << FROM_PROGRAM << "Can not send message: " << e.what()
                << std::endl;
   } catch (Server::ConnectionClosedByPeerException & e) {
      std::cout << FROM_PROGRAM << e.what() << std::endl;
   }

   return ;
}
