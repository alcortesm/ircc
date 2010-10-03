#include "ComOffer.h"
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
const std::string ComOffer::STR = std::string("/offer");

// boot up the DCC server and send PRIVMSG to victim with download
// instructions
void
ComOffer::Run() {

   *gpDebug << FROM_DEBUG << "ComOffer::Run()" << std::endl ;

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
                << "Sending offer of file "
                << mFileName
                << " to "
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
   } catch (DccServer::SocketException& e) {
      std::cout << FROM_PROGRAM
                << e.what()
                << std::endl;
   } catch (DccServer::AddrException& e) {
      std::cout << FROM_PROGRAM
                << e.what()
                << std::endl;
   } catch (DccServer::BindException& e) {
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


ComOffer::ComOffer(DccServer& rDccServer,
                     Server& rServer,
                     std::string & rNick,
                     std::string & rFileName)
   : mrDccServer(rDccServer), mrServer(rServer), mNick(rNick), mFileName(rFileName)
{
   *gpDebug << FROM_DEBUG << "ComOffer::ComOffer("
            << mrDccServer << ", "
            << mrServer << ", "
            << mNick << ", "
            << "\"" << rFileName << "\")"
            << std::endl ;   
}
