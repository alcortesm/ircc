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
   class AlreadyAuthenticatedException : public std::runtime_error {
   public:
      AlreadyAuthenticatedException() : std::runtime_error("Already authenticated") { }
      AlreadyAuthenticatedException(std::string s) : std::runtime_error("Already authenticated: " + s) { }
   };
   class NotAuthenticatedException : public std::runtime_error {
   public:
      NotAuthenticatedException() : std::runtime_error("Not authenticated") { }
      NotAuthenticatedException(std::string s) : std::runtime_error("Not authenticated: " + s) { }
   };
   class ConnectException : public std::runtime_error {
   public:
      ConnectException() : std::runtime_error("Server can not connect") { }
      ConnectException(std::string s) : std::runtime_error("Server can not connect: " + s) { }
   };
   class SendException : public std::runtime_error {
   public:
      SendException() : std::runtime_error("Can not send") { }
      SendException(std::string s) : std::runtime_error("Can not send: " + s) { }
   };
   class RecvException : public std::runtime_error {
   public:
      RecvException() : std::runtime_error("Can not recv") { }
      RecvException(std::string s) : std::runtime_error("Can not recv: " + s) { }
   };
   class ConnectionClosedByPeerException : public std::runtime_error {
   public:
      ConnectionClosedByPeerException() : std::runtime_error("Connection closed by peer") { }
   };

   Server();
   ~Server();

   void Connect(const std::string & host, const std::string & port)
      throw (Server::AlreadyConnectedException, Server::ConnectException);
   void Disconnect()
      throw (Server::NotConnectedException);
   void Send(const std::string& msg)
      throw (Server::NotConnectedException, Server::SendException, Server::ConnectionClosedByPeerException);
   std::string Recv()
      throw (Server::NotConnectedException, Server::RecvException, Server::ConnectionClosedByPeerException);

   const std::string & GetHost() const throw (Server::NotConnectedException);
   const std::string & GetPort() const throw (Server::NotConnectedException);
   const std::string & GetNick() const throw (Server::NotAuthenticatedException);
   int  GetSock() const;

   const std::string & GetChannel() const;
   void                SetChannel(const std::string& rChannel);
   void                ClearChannel();
   bool                IsChannel();
   bool                IsChannelClear();

   bool IsConnected() const;
   bool IsAuthenticated() const;
   void SetAuthenticated(const std::string& rNick);

   static bool TestOk();
   static bool TestFail() { return ! Server::TestOk(); };

   friend std::ostream & operator<<(std::ostream & os, Server & server);

 private:
   Server(const Server&); // not implemented, prevent copy ctor
   Server& operator=(const Server&); // not implemented, prevent copy ctor

   void reset_internal_state();

   std::string mHost;
   std::string mPort;
   std::string mChannel; // current channel
   std::string mNick;
   enum state {DISCONNECTED, CONNECTED, AUTHENTICATED} mState;
   int mSock;
};

#endif // SERVER_H
