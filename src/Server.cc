#include "Server.h"
#include "utils.h"
#include <ostream>
#include <iostream>
#include <string>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <stdlib.h>
#include <netdb.h>
#include "ircc.h"
#include <sstream>

extern std::ostream* gpDebug;

using std::endl;
using std::cout;
using std::string;

Server::Server()
   : mpHost(NULL),
     mpPort(NULL),
     mState(Server::DISCONNECTED)
{
   *gpDebug << "Server::Server()" << endl ;
   return;
}

Server::~Server()
{
   *gpDebug << "Server::~Server()" << endl ;
   if (IsConnected())
      Disconnect();
   return;
}

int
get_tcp_proto()
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
   free(buf);
   return pe.p_proto;
}

void
Server::Connect(const string & host, const string & port)
   throw (Server::AlreadyConnectedException, Server::ConnectException)
{
   *gpDebug << "Server::Connect(\"" << host << "\", \"" << port << "\")" << endl ;

   if (IsConnected())
      throw Server::AlreadyConnectedException();

   // learn what proto is for TCP
   int tcp_proto = get_tcp_proto();

   // create the socket
   mSock = socket(AF_INET, SOCK_STREAM, tcp_proto);
   if (mSock == -1) {
      size_t buf_sz = 1024; /* I hope is enough for errstr */
      char* buf = (char *) xmalloc(buf_sz);
      char* r;
      r = strerror_r(errno, buf, buf_sz);
      Server::ConnectException e(r);
      free(buf);
      throw e;
   }

   // bind to localport
   // TODO

   // get the list of addr where we want to connect (server's addr)
   // open the socket and connect
   {
      struct addrinfo hints;
      memset(&hints, 0, sizeof(struct addrinfo));
      hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = 0;
      hints.ai_protocol = tcp_proto;
      hints.ai_canonname = NULL;
      hints.ai_addr = NULL;
      hints.ai_next = NULL;
      hints.ai_addrlen = 0;

      const char* node = host.c_str();
      const char* service = port.c_str();
      
      struct addrinfo *result;
      int r;
      r = getaddrinfo(node, service, &hints, &result);
      if (r != 0) {
         close(mSock);
         Server::ConnectException e(gai_strerror(r));
         throw e;
      }

      /* getaddrinfo() returns a list of address structures.
         Try each address until we successfully connect(2).
         If socket(2) (or connect(2)) fails, we (close the socket
         and) try the next address. */      
      struct addrinfo* rp;
      for (rp = result; rp != NULL; rp = rp->ai_next) {
         mSock = socket(rp->ai_family, rp->ai_socktype,
                      rp->ai_protocol);
         if (mSock == -1)
            continue;
         
         if (connect(mSock, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */
         
         close(mSock);
      }
      
      freeaddrinfo(result);

      if (rp == NULL)             /* No address succeeded */
         throw Server::ConnectException("No address succeeded");
   }


   // fill the object members
   mpHost = new string(host);
   mpPort = new string(port);
   mState = Server::CONNECTED;
   return;
}

void
Server::Disconnect()
   throw (Server::NotConnectedException)
{
   *gpDebug << "Server::Disconnect()" << endl ;
   
   if (!IsConnected())
      throw Server::NotConnectedException();

   close(mSock);
   delete mpHost;
   delete mpPort;
   mpHost = NULL;
   mpPort = NULL;
   mState = Server::DISCONNECTED;
   return;
}

void
Server::Send(const char* buf, const size_t bufSize) const
   throw (Server::NotConnectedException)
{
   UNUSED(buf);
   UNUSED(bufSize);
   *gpDebug << "Server::Send(buf, " << bufSize << ")" << endl ;

   if (!IsConnected())
      throw Server::NotConnectedException();
   
   return;
}

size_t
Server::Recv(char* buf, const size_t bufSize) const
   throw (Server::NotConnectedException)
{
   UNUSED(buf);
   UNUSED(bufSize);
   *gpDebug << "Server::Recv(buf, " << bufSize << ")" << endl ;

   if (!IsConnected())
      throw Server::NotConnectedException();

   return 0;
}

bool
Server::IsConnected() const
{
   return mState == Server::CONNECTED;
}

const string&
Server::GetHost() const
   throw (Server::NotConnectedException)
{
   if (!IsConnected())
      throw Server::NotConnectedException();
   
   return *mpHost;
}

const string&
Server::GetPort() const
   throw (Server::NotConnectedException)
{
   if (!IsConnected())
      throw Server::NotConnectedException();
   
   return *mpPort;
}

bool
Server::TestOk()
{
   cout << "*** Begin of Server test" << endl ;

   {
      Server server;
      *gpDebug << server << endl;

      try {
         const string host("localhost");
         const string port("6757");
         
         server.Connect(host, port);
         *gpDebug << server << endl;
         
         server.Disconnect();
         *gpDebug << server << endl;

      } catch (Server::AlreadyConnectedException & e) {
         std::cerr << e.what() << endl;
         return false;
      } catch (Server::ConnectException & e) {
         std::cerr << e.what() << endl;
         return false;
      } catch (Server::NotConnectedException & e) {
         std::cerr << e.what() << endl;
         return false;
      }
   }

   {
      Server* p_server;
      try {
         const string host("localhost");
         const string port("6757");

         p_server = new Server();
         *gpDebug << *p_server << endl;

         p_server->Connect(host, port);
         *gpDebug << *p_server << endl;

         p_server->Disconnect();
         *gpDebug << *p_server << endl;

         delete p_server;

      } catch (Server::AlreadyConnectedException & e) {
         std::cerr << e.what() << endl;
         delete p_server;
         return false;
      } catch (Server::ConnectException & e) {
         std::cerr << e.what() << endl;
         delete p_server;
         return false;
      } catch (Server::NotConnectedException & e) {
         std::cerr << e.what() << endl;
         delete p_server;
         return false;
      }
   }

   {
      Server server;
      *gpDebug << server << endl;

      try {
         const string host("badhost");
         const string port("6757");
         
         server.Connect(host, port);
         *gpDebug << server << endl;
         
         server.Disconnect();
         *gpDebug << server << endl;

      } catch (Server::AlreadyConnectedException & e) {
         std::cerr << e.what() << endl;
         return false;
      } catch (Server::ConnectException & e) {
         goto ok0;
      } catch (Server::NotConnectedException & e) {
         std::cerr << e.what() << endl;
         return false;
      }
      std::cerr << "No ConnectException was thrown" << endl;
      return false;
   ok0:;
   }

   cout << "*** End of Server test" << endl ;
   return true;
}

std::ostream &
operator<<(std::ostream & os, Server & server) {
   os << "[Server state=" ;

   if (server.IsConnected()) {
      os << "CONNECTED";
      os << " host=" << server.GetHost();
      os << " port=" << server.GetPort();
   } else {
      os << "DISCONNECTED";
   }
   os << "]";
   return os;
}
