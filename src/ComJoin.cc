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

   *gpDebug << "ComJoin::Run()" << std::endl ;

   try {

      std::stringstream ss;
      ss << ComJoin::COMMAND << MESSAGE_SEPARATOR << mChannel << END_OF_MESSAGE;
      std::string s = ss.str();
      mrServer.Send(s);

   } catch (Server::NotConnectedException & e) {
      cout << "*** Can not set nick: not connected to server"
           << endl;
   } catch (Server::SendException & e) {
      cout << "*** Can not set nick: " << e.what()
           << endl;
   }

   return ;
}


ComJoin::ComJoin(const std::string & rChannel, Server& rServer)
   : mrServer(rServer), mChannel(rChannel)
{
   *gpDebug << "ComJoin::ComJoin(" << rServer << ", \""
            << rChannel << "\")" << std::endl ;   
}
