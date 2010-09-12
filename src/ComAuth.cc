#include "ComAuth.h"
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
const std::string ComAuth::STR = std::string("/auth");

void
ComAuth::Run() {

   *gpDebug << FROM_DEBUG << "ComAuth::Run()" << std::endl ;

   if (mrServer.IsAuthenticated()) {
      cout << FROM_PROGRAM << "You are already authenticated as " << mrServer.GetNick()
           << " on server " << mrServer.GetHost() << ":" << mrServer.GetPort() << endl ;
      return;
   }

   try {

      std::stringstream ss;
      ss << COM_NICK << MESSAGE_SEPARATOR << mNick << END_OF_MESSAGE;
      ss << COM_USER << MESSAGE_SEPARATOR << mNick << " 0 * :" << mNick << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException& e) {
      cout << FROM_PROGRAM << "Unable to authenticate, not connected to any server"
           << endl;
   } catch (Server::SendException& e) {
      cout << FROM_PROGRAM << "Unable to authenticate, " << e.what()
           << endl;
   } catch (Server::ConnectionClosedByPeerException& e) {
      std::cout << FROM_PROGRAM << e.what() << std::endl;
   }

   return ;
}


ComAuth::ComAuth(const std::string & rNick, Server& rServer)
   : mNick(rNick), mrServer(rServer)
{
   *gpDebug << FROM_DEBUG << "ComAuth::ComAuth(\""
            << rNick << "\", " << mrServer << ")" << std::endl ;   
}
