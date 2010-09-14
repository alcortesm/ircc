#include "ComUpload.h"
#include <ostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <stdexcept>
#include "ircc.h"
#include "irc.h"
#include "ctcp.h"
#include "utils.h"
#include <sstream>
#include <iostream>

extern std::ostream* gpDebug;

using std::cout;
using std::endl;

/* static */
const std::string ComUpload::STR = std::string("/upload");

void
ComUpload::Run() {

   *gpDebug << FROM_DEBUG << "ComUpload::Run()" << std::endl ;

   try {
      std::string host = "192.168.0.1"; // TODO don't make this up
      std::string host_str = "2130706433"; // TODO don't make this up
      std::string port = "12345"; // TODO don't make this up
      std::string file_size = "12354123"; // TODO don't make this up
      std::stringstream ss;
      ss << COM_PRIVMSG << MESSAGE_SEPARATOR
         << mNick << MESSAGE_SEPARATOR
         << ":" << CTCP_X_DELIM << "DCC SEND "
         << mFileName << MESSAGE_SEPARATOR
         << host_str << MESSAGE_SEPARATOR
         << port << MESSAGE_SEPARATOR
         << file_size
         << CTCP_X_DELIM << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);
      std::cout << FROM_PROGRAM
                << "Sent upload "
                << "[" << host
                << ":" << port
                << "] request to "
                << mNick
                << std::endl;

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


ComUpload::ComUpload(Server& rServer,
                     std::string & rNick,
                     std::string & rFileName)
   : mrServer(rServer), mNick(rNick), mFileName(rFileName)
{
   *gpDebug << FROM_DEBUG << "ComUpload::ComUpload(" << rServer
            << ", \"" << rNick
            << "\", \"" << rFileName << "\")" << std::endl ;   
}
