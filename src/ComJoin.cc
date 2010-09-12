#include "ComJoin.h"
#include <ostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <stdexcept>
#include "ircc.h"
#include "utils.h"
#include <sstream>
#include <iostream>
#include "irc.h"

extern std::ostream* gpDebug;

using std::cout;
using std::endl;

/* static */
const std::string ComJoin::STR = std::string("/join");
const std::string ComJoin::COMMAND = std::string("JOIN");

void
ComJoin::Run() {

   *gpDebug << FROM_DEBUG << "ComJoin::Run()" << std::endl ;

   try {

      std::string chan_to_join;
      // if we are leaving all channels, just send "JOIN 0"
      if (mChannel == "0") {
         chan_to_join = "0";
      }
      // if we are joinning some actual channel...
      else {
         // if we are not member of any channel, just send the "JOIN channel" command
         if (mrServer.IsChannelClear()) {
            chan_to_join = mChannel;
         } else {
            // if we are already members of a channel, send a "JOIN 0"
            // command now to leave the current channel and set the desired
            // channel to remind we must send a "JOIN desired" command when
            // the notification of leaving the current channel arraives
            chan_to_join = "0";
            mrServer.SetDesiredChannel(mChannel);
         }
      }
      std::stringstream ss;
      ss << ComJoin::COMMAND << MESSAGE_SEPARATOR << chan_to_join << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      cout << FROM_PROGRAM << "Can not set nick: not connected to server"
           << endl;
   } catch (Server::SendException & e) {
      cout << FROM_PROGRAM << "Can not set nick: " << e.what()
           << endl;
   } catch (Server::ConnectionClosedByPeerException& e) {
      cout << FROM_PROGRAM << e.what() << endl;
   }

   return ;
}


ComJoin::ComJoin(const std::string & rChannel, Server& rServer)
   : mrServer(rServer), mChannel(rChannel)
{
   *gpDebug << FROM_DEBUG << "ComJoin::ComJoin(" << rServer << ", \""
            << rChannel << "\")" << std::endl ;   
}
