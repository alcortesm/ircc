#ifndef DCCSERVER_H
#define DCCSERVER_H

#include <stdexcept>
#include <ostream>

class DccServer {
 public:
   class NotServingException : public std::runtime_error {
   public:
      NotServingException() : std::runtime_error("DCC server not serving any file") { }
      NotServingException(std::string s) : std::runtime_error("DCC server not serving any file: " + s) { }
   };
   class NotConnectedException : public std::runtime_error {
   public:
      NotConnectedException() : std::runtime_error("DCC server not connected") { }
      NotConnectedException(std::string s) : std::runtime_error("DCC server not connected: " + s) { }
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

   DccServer();
   ~DccServer();

   void Serve(std::string& rFileName);
   void Sleep();

   int  GetHost() const throw (DccServer::NotServingException);
   int  GetPort() const throw (DccServer::NotServingException);
   int  GetSocket() const throw (DccServer::NotServingException);
   const std::string& GetFileName() const throw (DccServer::NotServingException);
   int  GetFileSize() const throw (DccServer::NotServingException);

   void Send(const char* buf, size_t bufSize)
      throw (DccServer::NotConnectedException, DccServer::SendException, DccServer::ConnectionClosedByPeerException);
   size_t Recv(char* buf, size_t bufSize)
      throw (DccServer::NotConnectedException, DccServer::RecvException, DccServer::ConnectionClosedByPeerException);

   bool IsSleeping() const;
   bool IsServing() const;
   bool IsConnected() const;

   friend std::ostream & operator<<(std::ostream & os, const DccServer & server);

 private:
   DccServer(const DccServer&); // not implemented, prevent copy ctor
   DccServer& operator=(const DccServer&); // not implemented, prevent copy ctor

   void reset_internal_state();

   std::string mFileName;
   int mHost;
   int mPort;
   int mSock;
   enum {SLEEPING, SERVING, CONNECTED} mState;
};

#endif // DCCSERVER_H