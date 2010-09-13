#include "Msg.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include "ComJoin.h"
#include "irc.h"
#include "ircc.h"

extern std::ostream* gpDebug;

Msg::Msg(const std::string& rPrefix, const std::string& rCommand, const std::vector<std::string>& rParams, Server& rServer)
   : mPrefix(rPrefix), mCommand(rCommand), mParams(rParams), mrServer(rServer)
{}

void
Msg::Run() const
{
   /* JOIN */
   if (mCommand == "JOIN") {
      if (mParams.size() != 1) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      mrServer.SetChannel(mParams[0]);
      std::cout << FROM_PROGRAM << mrServer.GetNick() << " has joined channel "
                << mParams[0] << std::endl ;
      return;
   }

   /* PART */
   if (mCommand == "PART") {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      if (mrServer.GetChannel() == mParams[0]) {
         std::cout << FROM_PROGRAM << mParams[1] << " has left channel "
                   << mParams[0] << std::endl ;
         mrServer.ClearChannel();
      }
      return;
   }

   /* PING */
   if (mCommand == "PING") {
      if (mParams.size() != 4) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      /* send PONG */
      try {

         std::stringstream ss;
         ss << "PONG" << MESSAGE_SEPARATOR
            << ":" << mParams[0] << END_OF_MESSAGE;
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
      return;
   }

   /* RPL_WELCOME */
   if (mCommand == RPL_WELCOME) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << mParams[1];
      if (!mPrefix.empty())
         std::cout << " (from " << mPrefix << ")" << std::endl;
      mrServer.SetAuthenticated(mParams[0]);
      return;
   }

   /* RPL_YOURHOST */
   if (mCommand == RPL_YOURHOST) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << mParams[1] << std::endl;
      return;
   }

   /* RPL_CREATED */
   if (mCommand == RPL_CREATED) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << mParams[1] << std::endl;
      return;
   }

   /* RPL_MYINFO */
   if (mCommand == RPL_MYINFO) {
      if (mParams.size() != 5) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM
                << "umodes available " << mParams[3]
                << ", channel modes available " << mParams[4]
                << std::endl;
      return;
   }

   /* RPL_LUSERCLIENT */
   if (mCommand == RPL_LUSERCLIENT) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << mParams[1] << std::endl;
      return;
   }

   /* RPL_LUSERCHANNELS */
   if (mCommand == RPL_LUSERCHANNELS) {
      if (mParams.size() != 3) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << mParams[1] << " "
                << mParams[2] << std::endl;
      return;
   }

   /* RPL_LUSERME */
   if (mCommand == RPL_LUSERME) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << mParams[1] << std::endl;
      return;
   }

   /* RPL_MOTD */
   if (mCommand == RPL_MOTD) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << "- "
                << mPrefix << "message of the day" << std::endl;
      std::cout << FROM_PROGRAM
                << mParams[1] << std::endl;
      return;
   }

   /* RPL_TOPIC */
   if (mCommand == RPL_TOPIC) {
      if (mParams.size() != 3) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM
                << "Topic for " << mParams[1]
                << ": " << mParams[2] << std::endl;
      return;
   }

   /* RPL_NAMREPLY */
   if (mCommand == RPL_NAMREPLY) {
      if (mParams.size() != 4) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM
                << "Users in channel " << mParams[2]
                << ": " << mParams[3] << std::endl;
      return;
   }

   /* RPL_ENDOFNAMES */
   if (mCommand == RPL_ENDOFNAMES) {
      if (mParams.size() != 3) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      // do nothing
      return;
   }

   /* RPL_ENDOFMOTD */
   if (mCommand == RPL_ENDOFMOTD) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      // do nothing
      return;
   }

   /* Common commands unknown to RFC2812 */
   if (
       mCommand == "250" ||
       mCommand == "265" ||
       mCommand == "266" ||
       mCommand == "375" ||
       mCommand == "333"
       ) {
      std::cout << FROM_PROGRAM
                << "Message received with a command unknown to RFC2812 ("
                << mCommand << ")" << std::endl;
      return;
   }

   /* other commands */
   std::cout << FROM_PROGRAM << "Unknown message from server with command <"
             << mCommand << ">" << std::endl;
   return;
}

std::ostream &
operator<<(std::ostream & os, const Msg& msg) {

   os << "[ Msg ";
   if (!msg.mPrefix.empty())
      os << "(" << msg.mPrefix << ") ";

   os << "<" << msg.mCommand << "> " ;
   for(std::vector<std::string>::const_iterator it = msg.mParams.begin();
       it != msg.mParams.end();
       ++it) {
      if (it != msg.mParams.begin())
         os << ", " ;

      os << "\"" << *it << "\"";
   }
   os << "]";
   return os;
}
