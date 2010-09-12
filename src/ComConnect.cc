#include "ComConnect.h"
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

extern std::ostream* gpDebug;

using std::cout;
using std::endl;

/* static */
const std::string ComConnect::STR = std::string("/connect");

int
open_socket()
{

   int proto;
   {
      struct protoent pe;
      struct protoent * p_pe = NULL;
      const size_t buf_sz = PROTOENT_BUF_SZ;
      char* buf = (char*) xmalloc(buf_sz);
      int r;
      r = getprotobyname_r(TCP.c_str(), &pe, buf, buf_sz, &p_pe);
      if (r != 0) { /* error */
         if (r == ERANGE) {
            std::stringstream ss;
            ss << "ComConnect::Run() : getprotobyname_r returned ERANGE : a buffer of "
               << buf_sz
               << " is too smal for the struct protoent, try again with a bigger buffer";
            throw std::runtime_error(ss.str());
         }
         std::stringstream ss;
         ss << "ComConnect::Run() : getprotobyname_r returned the unknown error "
            << r;
         throw std::runtime_error(ss.str());
      }
      if (p_pe == NULL) { /* protocol not found */
         std::stringstream ss;
         ss << "ComConnect::Run() : getprotobyname_r : protocol \""
            << TCP.c_str()
            << "\" not found";
         throw std::runtime_error(ss.str());
      }
      proto = pe.p_proto;
   }

   int sock;
   sock = socket(AF_INET, SOCK_STREAM, proto);
   if (sock == -1)
      fatal("ComConnect::Run() : socket", NULL);
   return sock;
}

void
ComConnect::Run() {

   *gpDebug << FROM_DEBUG << "ComConnect::Run()" << std::endl ;

   try {

      mrServer.Connect(mHost, mPort);
      cout << FROM_PROGRAM << "Connected to server " << mHost << ":" << mPort << endl;

   } catch (Server::AlreadyConnectedException & e) {
      cout << FROM_PROGRAM << "Already connected to " << mrServer.GetHost()
           << ":" << mrServer.GetPort() << endl;
   } catch (Server::ConnectException & e) {
      *gpDebug << FROM_DEBUG << "Unable to connect to server "
               << mHost << ":" << mPort << " : "
               << e.what() << endl ;
      cout << FROM_PROGRAM << "Unable to connect to server "
           << mHost << ":" << mPort << endl ;
   }

   return ;
}


ComConnect::ComConnect(Server& rServer, std::string & rHost, std::string & rPort)
   : mrServer(rServer), mHost(rHost), mPort(rPort)
{
   *gpDebug << FROM_DEBUG << "ComConnect::ComConnect(" << rServer
            << ", \"" << rHost
            << "\", \"" << rPort << "\")" << std::endl ;   
}
