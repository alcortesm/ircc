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
   class ListenException : public std::runtime_error {
   public:
   ListenException() : std::runtime_error("Unable to launch DCC server: can not listen") { }
   ListenException(std::string s) : std::runtime_error("Unable to launch DCC server: can not listen: " + s) { }
   };
   class SocketException : public std::runtime_error {
   public:
   SocketException() : std::runtime_error("Unable to launch DCC server: can not open socket") { }
   SocketException(std::string s) : std::runtime_error("Unable to launch DCC server: can not open socket: " + s) { }
   };
   class AddrException : public std::runtime_error {
   public:
   AddrException() : std::runtime_error("Unable to launch DCC server: can not get local addr") { }
   AddrException(std::string s) : std::runtime_error("Unable to launch DCC server: can not get local addr: " + s) { }
   };
   class BindException : public std::runtime_error {
   public:
   BindException() : std::runtime_error("Unable to launch DCC server: can not bind") { }
   BindException(std::string s) : std::runtime_error("Unable to launch DCC server: can not bind: " + s) { }
   };
   class FileException : public std::runtime_error {
   public:
   FileException(std::string fileName, std::string err)
      : std::runtime_error(fileName.append(": ").append(err)) { }
   };
   class AlreadyListeningException : public std::runtime_error {
   public:
   AlreadyListeningException()
      : std::runtime_error("") { }
   };

   DccServer();
   ~DccServer();

   void Listen(const std::string& rFileName)
      throw (DccServer::AlreadyListeningException,
             DccServer::FileException,
             DccServer::SocketException,
             DccServer::AddrException,
             DccServer::BindException,
             DccServer::ListenException);
   void Sleep();

   const std::string& GetHost() const
      throw (DccServer::NotServingException);
   int                GetHostInt() const
      throw (DccServer::NotServingException);
   int                GetPort() const
      throw (DccServer::NotServingException);
   int                GetSocket() const
      throw (DccServer::NotServingException);
   const std::string& GetFileName() const
      throw (DccServer::NotServingException);
   unsigned long      GetFileSize() const
      throw (DccServer:: FileException, DccServer::NotServingException);

   void Send(const char* buf, size_t bufSize)
      throw (DccServer::NotConnectedException
             , DccServer::SendException
             , DccServer::ConnectionClosedByPeerException);
   size_t Recv(char* buf, size_t bufSize)
      throw (DccServer::NotConnectedException
             , DccServer::RecvException
             , DccServer::ConnectionClosedByPeerException);

   bool IsSleeping() const;
   bool IsListening() const;
   bool IsConnected() const;

   friend std::ostream & operator<<(std::ostream &os
                                    , const DccServer& server);

 private:
   DccServer(const DccServer&); // not implemented, prevent copy ctor
   DccServer& operator=(const DccServer&); // not implemented, prevent copy ctor

   void reset_internal_state();

   std::string mFileName;
   std::string mHost;
   int mPort;
   int mSocket;
   enum {SLEEPING, LISTENING, CONNECTED} mState;
};

#endif // DCCSERVER_H
