#include "ComNick.h"
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
const std::string ComNick::STR = std::string("/nick");
const std::string ComNick::COMMAND = std::string("NICK");

void
ComNick::Run() {

   *gpDebug << FROM_DEBUG << "ComNick::Run()" << std::endl ;

   try {

      std::stringstream ss;
      ss << ComNick::COMMAND << MESSAGE_SEPARATOR << mNick << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      cout << FROM_PROGRAM << "Can not set nick: not connected to server"
           << endl;
   } catch (Server::SendException & e) {
      cout << FROM_PROGRAM << "Can not set nick: " << e.what()
           << endl;
   }

   return ;
}


ComNick::ComNick(std::string & rNick, Server& rServer)
   : mrServer(rServer), mNick(rNick)
{
   *gpDebug << FROM_DEBUG << "ComNick::ComNick(" << rServer << ", \""
            << rNick << "\")" << std::endl ;   
}
