#ifndef SERVER_H
#define SERVER_H

#include "Url.h"

class Server {
 public:
   class AlreadyConnectedException : public std::runtime_error {
   public:
      AlreadyConnectedException() : std::runtime_error("Server already connected") { }
      AlreadyConnectedException(std::string s) : std::runtime_error("Server already connected: " + s) { }
   };
   class NotConnectedException : public std::runtime_error {
   public:
      NotConnectedException() : std::runtime_error("Server not connected") { }
      NotConnectedException(std::string s) : std::runtime_error("Server not connected: " + s) { }
   };

   Server();
   ~Server();

   void Connect(const Url & url) throw (Server::AlreadyConnectedException);
   void Disconnect() throw (Server::NotConnectedException);
   void Send(const char* buf, const size_t bufSize) const throw (Server::NotConnectedException);
   size_t Recv(char* buf, const size_t bufSize) const throw (Server::NotConnectedException);

   bool IsConnected() const;
   const Url& GetUrl() const throw (Server::NotConnectedException);

   static void Test(void);

 private:
   Server(const Server&); // not implemented, prevent copy ctor
   Server& operator=(const Server&); // not implemented, prevent copy ctor
   Url* mpUrl;
   enum state {DISCONNECTED, CONNECTED} mState;
   int mSock;
};

std::ostream & operator<<(std::ostream & os, Server & server);

#endif // SERVER_H
