#include "Msg.h"
#include "ctcp.h"
#include "dcc.h"
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

std::string
get_nick_from_prefix(const std::string& rPrefix)
{
   size_t exclam_pos = rPrefix.find("!");
   std::string nick = rPrefix.substr(0, exclam_pos);
   return nick;
}

void
Msg::Run() const
{
   /* JOIN */
   if (mCommand == COM_JOIN) {
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

   /* PRIVMSG */
   if (mCommand == COM_PRIVMSG) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      // check it's a private message with DCC content
      if (mParams[1].find(CTCP_X_DELIM) == 0) {
         // check if it's a known DCC message (we only support DCC SEND) */
         if (mParams[1].find(DCC_SEND_PREFIX) != 0) {
            std::cout << "Received unsupported DCC message: "
                      << *this << std::endl;
         }
         std::string sender = mParams[0];
         std::string file_name = ".emacs"; // TODO extract from msg
         std::string file_size = "123123"; // TODO extract from msg
         std::string host = "127.0.0.1"; // TODO extract from msg
         std::string port = "53037"; // TODO extract from msg
         std::cout << FROM_PROGRAM
                   << "DCC SEND (" << file_name << " "
                   << file_size << ") request received from "
                   << sender << " ["
                   << host << ":" << port << "]"
                   << std::endl;
         std::cout << FROM_PROGRAM
                   << "Use \"/download "
                   << " " << file_name
                   << " " << host
                   << " " << port
                   << "\" to download"
                   << std::endl;
            return;
      }
      // check if message from channel
      if (mrServer.GetChannel() == mParams[0]) {
         std::string nick = get_nick_from_prefix(mPrefix);
         std::cout << "<" << nick << "> " << mParams[1] <<  std::endl;
      }
      return;
   }

   /* PART */
   if (mCommand == COM_PART) {
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
   if (mCommand == COM_PING) {
      if (mParams.size() != 1) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      /* send PONG */
      try {

         std::stringstream ss;
         ss << COM_PONG << MESSAGE_SEPARATOR
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

   /* RPL_ENDOFWHO */
   if (mCommand == RPL_ENDOFWHO) {
      if (mParams.size() != 3) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      // do nothing
      return;
   }

   /* RPL_LIST */
   if (mCommand == RPL_LIST) {
      if (mParams.size() != 4) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << mParams[1]
                << "\t\t" << mParams[2]
                << "\t" << mParams[3]
                << std::endl;
      return;
   }

   /* RPL_LISTEND */
   if (mCommand == RPL_LISTEND) {
      if (mParams.size() != 2) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      // do nothing
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

   /* RPL_WHOREPLY */
   if (mCommand == RPL_WHOREPLY) {
      if (mParams.size() != 8) {
         std::cout << FROM_PROGRAM << "Received invalid message from server!: "
                   << *((Msg*) this) << std::endl;
         return;
      }
      std::cout << FROM_PROGRAM << mParams[1]
                << "\t\t" << mParams[5]
                << std::endl;
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
