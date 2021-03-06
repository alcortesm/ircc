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
#include "irc.h"
#include "ctcp.h"
#include <sstream>

extern std::ostream* gpDebug;

using std::endl;
using std::cout;
using std::string;

Server::Server()
   : mHost(),
     mPort(),
     mChannel(string()),
     mNick(string()),
     mState(Server::DISCONNECTED),
     mSock(-1)
{
   *gpDebug << FROM_DEBUG << "Server::Server()" << endl ;
   return;
}

Server::~Server()
{
   *gpDebug << FROM_DEBUG << "Server::~Server()" << endl ;
   if (IsConnected())
      Disconnect();
   return;
}

void
Server::reset_internal_state()
{
   mHost = string();
   mPort = string();
   mChannel = string();
   mNick = string();
   mState = Server::DISCONNECTED;
   mSock = -1;
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
   *gpDebug << FROM_DEBUG << "Server::Connect(\"" << host << "\", \"" << port << "\")" << endl ;

   if (IsConnected())
      throw Server::AlreadyConnectedException();

   // learn what proto is for TCP
   //   int tcp_proto = get_tcp_proto();
   int tcp_proto = 0;

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
   mHost = string(host);
   mPort = string(port);
   mState = Server::CONNECTED;
   return;
}

void
Server::Disconnect()
   throw (Server::NotConnectedException)
{
   *gpDebug << FROM_DEBUG << "Server::Disconnect()" << endl ;
   
   if (!IsConnected())
      throw Server::NotConnectedException();

   close(mSock);
   reset_internal_state();
   return;
}

string
textify(string str)
{
   size_t current;
   while (true) {
      current = str.find(END_OF_MESSAGE);
      if (current == string::npos)
         break;
      str.replace(current, END_OF_MESSAGE.length(), "[\\r][\\n]");
   }
   while (true) {
      current = str.find(CTCP_X_DELIM);
      if (current == string::npos)
         break;
      str.replace(current, CTCP_X_DELIM.length(), "[\\001]");
   }
   return str;
}

void
Server::Send(const std::string& str)
   throw (Server::NotConnectedException, Server::SendException, Server::ConnectionClosedByPeerException)
{
   *gpDebug << FROM_DEBUG << "Server::Send(\"" << textify(str) << "\")" << endl ;

   if (!IsConnected())
      throw Server::NotConnectedException();

   size_t buf_sz = str.length();
   char* buf = (char*) xmalloc(buf_sz);
   memcpy(buf, str.c_str(), buf_sz);

   size_t offset = 0;
   while (true) {
      ssize_t nw;
      nw = send(mSock, buf+offset, buf_sz-offset, NO_FLAGS);
      if (nw == -1) {
         size_t errbuf_sz = 1024; /* I hope is enough for errstr */
         char* errbuf = (char *) xmalloc(errbuf_sz);
         char* r;
         r = strerror_r(errno, errbuf, errbuf_sz);
         Server::SendException e(r);
         free(errbuf);
         free(buf);
         throw e;
      }
      if (nw == 0) {
         free(buf);
         close(mSock);
         reset_internal_state();
         throw ConnectionClosedByPeerException();
      }
      offset += nw ;
      if (offset == str.length())
         break;
   }

   free(buf);
   return;
}

std::string
Server::Recv()
   throw (Server::NotConnectedException, Server::RecvException, Server::ConnectionClosedByPeerException)
{
   *gpDebug << FROM_DEBUG << "Server::Recv()" << endl ;

   if (!IsConnected())
      throw Server::NotConnectedException();

   size_t buf_sz = 1024;
   char* buf = (char*) xcalloc(buf_sz, sizeof(char));

   int nr;
   nr = recv(mSock, buf, buf_sz-1, NO_FLAGS);
   if (nr == -1) {
      free(buf);
      throw RecvException();
   }
   if (nr == 0) { // server close the connection
      free(buf);
      close(mSock);
      reset_internal_state();
      throw ConnectionClosedByPeerException();
   }

   buf[buf_sz-1] = '\0';
   string r(buf);
   free(buf);
   return r;
}

void
Server::SetAuthenticated(const std::string& rNick)
{
   if (IsAuthenticated())
      throw Server::AlreadyAuthenticatedException();
   if (!IsConnected())
      throw Server::NotConnectedException();
   mNick = rNick;
   mState = Server::AUTHENTICATED;
}

bool
Server::IsConnected() const
{
   return mState >= Server::CONNECTED;
}

bool
Server::IsAuthenticated() const
{
   return mState >= Server::AUTHENTICATED;
}

const string&
Server::GetHost() const
   throw (Server::NotConnectedException)
{
   if (!IsConnected())
      throw Server::NotConnectedException();
   
   return mHost;
}

const string&
Server::GetPort() const
   throw (Server::NotConnectedException)
{
   if (!IsConnected())
      throw Server::NotConnectedException();
   
   return mPort;
}

const string&
Server::GetNick() const
   throw (Server::NotAuthenticatedException)
{
   if (!IsAuthenticated())
      throw Server::NotAuthenticatedException();
   
   return mNick;
}

const string&
Server::GetChannel() const
{
   return mChannel;
}

void
Server::SetChannel(const std::string& rChannel)
{
   mChannel = rChannel;
   return;
}

void
Server::ClearChannel()
{
   mChannel = string();
   return;
}

bool
Server::IsChannel()
{
   return !IsChannelClear();
}

bool
Server::IsChannelClear()
{
   return mChannel.empty();
}

int
Server::GetSock() const
{
   return mSock;
}

bool
Server::TestOk()
{
   cout << FROM_PROGRAM << "Begin of Server test" << endl ;

   {
      Server server;
      *gpDebug << FROM_DEBUG << server << endl;

      try {
         const string host("localhost");
         const string port("6757");
         
         server.Connect(host, port);
         *gpDebug << FROM_DEBUG << server << endl;
         
         server.Disconnect();
         *gpDebug << FROM_DEBUG << server << endl;

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
         *gpDebug << FROM_DEBUG << *p_server << endl;

         p_server->Connect(host, port);
         *gpDebug << FROM_DEBUG << *p_server << endl;

         p_server->Disconnect();
         *gpDebug << FROM_DEBUG << *p_server << endl;

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
      *gpDebug << FROM_DEBUG << server << endl;

      try {
         const string host("badhost");
         const string port("6757");
         
         server.Connect(host, port);
         *gpDebug << FROM_DEBUG << server << endl;
         
         server.Disconnect();
         *gpDebug << FROM_DEBUG << server << endl;

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

   cout << FROM_PROGRAM << "End of Server test" << endl ;
   return true;
}

std::ostream&
operator<<(std::ostream& os, Server& rServer) {
   os << "[Server state=" ;

   if (rServer.IsConnected()) {
      if (rServer.IsAuthenticated())
         os << "AUTHENTICATED";
      else
         os << "CONNECTED";
      os << ", host=\"" << rServer.GetHost() << "\"";
      os << ", port=\"" << rServer.GetPort() << "\"";
      if (rServer.IsAuthenticated())
         os << ", nick=\"" << rServer.GetNick() << "\"";
      if (rServer.IsChannel())
         os << ", channel=\"" << rServer.GetChannel() << "\"";
      else
         os << ", --no channel--";
   } else {
      os << "DISCONNECTED";
   }
   os << "]";
   return os;
}
