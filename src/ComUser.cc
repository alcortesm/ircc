#include "ComUser.h"
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
const std::string ComUser::STR = std::string("/user");
const std::string ComUser::COMMAND = std::string("USER");

void
ComUser::Run() {

   *gpDebug << FROM_DEBUG << "ComUser::Run()" << std::endl ;

   try {

      std::stringstream ss;
      ss << ComUser::COMMAND << MESSAGE_SEPARATOR << mUser << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      cout << FROM_PROGRAM << "Can not set user: not connected to server"
           << endl;
   } catch (Server::SendException & e) {
      cout << FROM_PROGRAM << "Can not set user: " << e.what()
           << endl;
   } catch (Server::ConnectionClosedByPeerException & e) {
      std::cout << FROM_PROGRAM << e.what() << std::endl;
   }

   return ;
}


ComUser::ComUser(const std::string & rUser, Server& rServer)
   : mrServer(rServer), mUser(rUser)
{
   *gpDebug << FROM_DEBUG << "ComUser::ComUser(" << rServer << ", \""
            << rUser << "\")" << std::endl ;   
}
