#include "ComMsg.h"
#include <ostream>
#include <iostream>
#include <sstream>
#include "irc.h"
#include "ircc.h"

extern std::ostream* gpDebug;

const std::string ComMsg::STR = std::string("/msg");

void
ComMsg::Run() {

   *gpDebug << FROM_DEBUG << "ComMsg::Run()" << std::endl ;

   if (mrServer.GetChannel().empty()) {
      std::cout << FROM_PROGRAM << "You must join a channel first"
                << std::endl;
      return;
   }

   try {

      std::stringstream ss;
      ss << COM_PRIVMSG << MESSAGE_SEPARATOR
         << mrServer.GetChannel() << MESSAGE_SEPARATOR
         << ":" << mMsg << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);
      std::cout << "> " << mMsg << std::endl;

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

ComMsg::ComMsg(const std::string& rMsg, Server& rServer)
   : mMsg(rMsg), mrServer(rServer)
{
   *gpDebug << FROM_DEBUG << "ComMsg::ComMsg(\""
            << rMsg << "\", "
            << rServer << ")" << std::endl;
}

