#include "Server.h"
#include "utils.h"
#include <ostream>
#include <iostream>
#include <string>
#include "MockDnsResolver.h"
#include <cassert>

extern std::ostream* gpDebug;

using std::endl;
using std::cout;
using std::string;

Server::Server()
   : mpUrl(NULL),
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

void
Server::Connect(const Url & url)
   throw (Server::AlreadyConnectedException)
{
   *gpDebug << "Server::Connect(" << url << ")" << endl ;

   if (IsConnected())
      throw Server::AlreadyConnectedException();

   mpUrl = new Url(url);
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

   delete mpUrl;
   mpUrl = NULL;
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

const Url&
Server::GetUrl() const
   throw (Server::NotConnectedException)
{
   if (!IsConnected())
      throw Server::NotConnectedException();
   
   return *mpUrl;
}

void
Server::Test()
{
   cout << "*** Begin of Server test" << endl ;

   {

      Server server;
      *gpDebug << server;

      try {
         MockDnsResolver mock_dns_resolver;
         string url_string("http://localhost:2000");
         const Url url(url_string, &mock_dns_resolver);
         
         server.Connect(url);
         *gpDebug << server;
         
         server.Disconnect();
         *gpDebug << server;

      } catch (DnsResolver::NetworkException & e) {
         assert(true);
      } catch (std::bad_alloc & e) {
         assert(true);
      } catch (Url::MalformedUrlException & e) {
         assert(true);
      } catch (Server::AlreadyConnectedException & e) {
         assert(true);
      } catch (Server::NotConnectedException & e) {
         assert(true);
      }
   }

   {
      try {
         MockDnsResolver mock_dns_resolver;
         const string url_string("http://localhost:2000");
         const Url url(url_string, &mock_dns_resolver);

         Server* p_server = new Server();
         *gpDebug << *p_server;
         p_server->Connect(url);
         *gpDebug << *p_server;
         p_server->Disconnect();
         *gpDebug << *p_server;
         delete p_server;

      } catch (DnsResolver::NetworkException & e) {
         assert(true);
      } catch (std::bad_alloc & e) {
         assert(true);
      } catch (Url::MalformedUrlException & e) {
         assert(true);
      } catch (Server::AlreadyConnectedException & e) {
         assert(true);
      } catch (Server::NotConnectedException & e) {
         assert(true);
      }
   }

   cout << "*** End of Server test" << endl ;
   return;
}

std::ostream &
operator<<(std::ostream & os, Server & server) {
   os << "[Server state=" ;

   if (server.IsConnected()) {
      os << "CONNECTED ";
      os << "url=" << server.GetUrl();
   } else {
      os << "DISCONNECTED";
   }
   os << "]" << endl;
   return os;
}
