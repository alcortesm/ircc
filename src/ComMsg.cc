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

   *gpDebug << "ComMsg::Run()" << std::endl ;

   try {

      std::stringstream ss;
      ss << ComMsg::COMMAND << MESSAGE_SEPARATOR
         //         << mrServer.Channel() << MESSAGE_SEPARATOR
         << "#ro2" << MESSAGE_SEPARATOR
         << ":" << mMsg << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      std::cout << FROM_PROGRAM << "Can not send message: not connected to server"
                << std::endl;
   } catch (Server::SendException & e) {
      std::cout << FROM_PROGRAM << "Can not send message: " << e.what()
                << std::endl;
   }

   return ;
}
