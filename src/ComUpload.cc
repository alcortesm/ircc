#include "ComUpload.h"
#include <ostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <stdexcept>
#include "ircc.h"
#include "irc.h"
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
      std::stringstream ss;
      ss << COM_PRIVMSG << MESSAGE_SEPARATOR
         << mNick << MESSAGE_SEPARATOR
         << ":\\001DCC SEND " << mFileName
         << " 21341234123412 123 41234\\001" << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);
      std::cout << "> " << s << std::endl;

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
