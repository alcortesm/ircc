#ifndef SERVER_H
#define SERVER_H

#include <stdexcept>

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
   class ConnectException : public std::runtime_error {
   public:
      ConnectException() : std::runtime_error("Server can not connect") { }
      ConnectException(std::string s) : std::runtime_error("Server can not connect: " + s) { }
   };

   Server();
   ~Server();

   void Connect(const std::string & host, const std::string & port)
      throw (Server::AlreadyConnectedException, Server::ConnectException);
   void Disconnect()
      throw (Server::NotConnectedException);
   void Send(const char* buf, const size_t bufSize) const
      throw (Server::NotConnectedException);
   size_t Recv(char* buf, const size_t bufSize) const
      throw (Server::NotConnectedException);

   bool IsConnected() const;
   const std::string & GetHost() const throw (Server::NotConnectedException);
   const std::string & GetPort() const throw (Server::NotConnectedException);

   static bool TestOk();
   static bool TestFail() { return ! Server::TestOk(); };

 private:
   Server(const Server&); // not implemented, prevent copy ctor
   Server& operator=(const Server&); // not implemented, prevent copy ctor

   std::string * mpHost;
   std::string * mpPort;
   enum state {DISCONNECTED, CONNECTED} mState;
   int mSock;
};

std::ostream & operator<<(std::ostream & os, Server & server);

#endif // SERVER_H
