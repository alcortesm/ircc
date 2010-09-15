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

// boot up the DCC server and send PRIVMSG to victim with download
// instructions
void
ComUpload::Run() {

   *gpDebug << FROM_DEBUG << "ComUpload::Run()" << std::endl ;

   try {
      if (!mrDccServer.IsSleeping())
         mrDccServer.Sleep();

      // boot up DCC server
      mrDccServer.Listen(mFileName);
      int         host_int  = mrDccServer.GetHostInt();
      std::string host_str  = mrDccServer.GetHost();
      int         port      = mrDccServer.GetPort();
      int         file_size = mrDccServer.GetFileSize();

      // send PRIVMSG to victim with download instructions
      std::stringstream ss;
      ss << COM_PRIVMSG << MESSAGE_SEPARATOR
         << mNick << MESSAGE_SEPARATOR
         << ":" << CTCP_X_DELIM << "DCC SEND "
         << mFileName << MESSAGE_SEPARATOR
         << host_int << MESSAGE_SEPARATOR
         << port << MESSAGE_SEPARATOR
         << file_size
         << CTCP_X_DELIM << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

      // message to user
      std::cout << FROM_PROGRAM
                << "Sent upload "
                << "[" << host_str
                << ":" << port
                << "] request to "
                << mNick
                << std::endl;

   } catch (DccServer::AlreadyListeningException& e) {
      std::cout << FROM_PROGRAM
                << "Already uploading a file"
                << std::endl;
   } catch (DccServer::FileException& e) {
      std::cout << FROM_PROGRAM
                << e.what()
                << std::endl;
   } catch (DccServer::ListenException& e) {
      std::cout << FROM_PROGRAM
                << e.what()
                << std::endl;
   } catch (Server::NotConnectedException & e) {
      std::cout << FROM_PROGRAM
                << "Can not send message: not connected to server"
                << std::endl;
   } catch (Server::SendException & e) {
      std::cout << FROM_PROGRAM
                << "Can not send message: " << e.what()
                << std::endl;
   } catch (Server::ConnectionClosedByPeerException & e) {
      std::cout << FROM_PROGRAM << e.what() << std::endl;
   }

   return ;
}


ComUpload::ComUpload(DccServer& rDccServer,
                     Server& rServer,
                     std::string & rNick,
                     std::string & rFileName)
   : mrDccServer(rDccServer), mrServer(rServer), mNick(rNick), mFileName(rFileName)
{
   *gpDebug << FROM_DEBUG << "ComUpload::ComUpload("
            << mrDccServer << ", "
            << mrServer << ", "
            << mNick << ", "
            << "\"" << rFileName << "\")"
            << std::endl ;   
}
