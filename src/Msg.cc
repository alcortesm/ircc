#include "Msg.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include "ComJoin.h"
#include "irc.h"
#include "ircc.h"

extern std::ostream* gpDebug;

Msg::Msg(const std::string& rPrefix, const std::string& rCommand, const std::vector<std::string>& rParams)
   : mPrefix(rPrefix), mCommand(rCommand), mParams(rParams)
{
   *gpDebug << FROM_DEBUG << FROM_PROGRAM << *this << std::endl ;
}

void
Msg::Run(Server& rServer) const
{
   /* JOIN */
   if (mCommand == "JOIN") {
      rServer.SetChannel(mParams[0]);
      std::cout << FROM_PROGRAM << "you have join channel "
                << mParams[0] << std::endl ;
   }

   /* PART */
   if (mCommand == "PART") {
      if (rServer.GetChannel() == mParams[0]) {
         std::cout << FROM_PROGRAM << "you have leave channel "
                   << mParams[0] << std::endl ;
         rServer.ClearChannel();
      }
   }

   /* PING */
   if (mCommand == "PING") {
      /* send PONG */
      try {

         std::stringstream ss;
         ss << "PONG" << MESSAGE_SEPARATOR
            << ":" << mParams[0] << END_OF_MESSAGE;
         std::string s = ss.str();
         rServer.Send(s);

      } catch (Server::NotConnectedException & e) {
         std::cout << FROM_PROGRAM << "Can not send message: not connected to server"
                   << std::endl;
      } catch (Server::SendException & e) {
         std::cout << FROM_PROGRAM << "Can not send message: " << e.what()
                   << std::endl;
      }

   }
}

std::ostream &
operator<<(std::ostream & os, Msg& msg) {

   os << "[ Msg ";
   if (!msg.mPrefix.empty())
      os << "(" << msg.mPrefix << ") ";

   os << "<" << msg.mCommand << "> " ;
   for(std::vector<std::string>::iterator it = msg.mParams.begin();
       it != msg.mParams.end();
       ++it) {
      if (it != msg.mParams.begin())
         os << " " ;

      os << *it ;
   }
   os << "]";
   return os;
}
