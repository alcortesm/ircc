#include "DccServer.h"

DccServer::DccServer()
   : mHost()
   , mPort()
   , mSock()
   , mState(DccServer::SLEEPING)
{}

DccServer::~DccServer()
{}

bool
DccServer::IsSleeping() const
{
   return mState == DccServer::SLEEPING;
}

bool
DccServer::IsServing() const
{
   return mState >= DccServer::SERVING;
}

bool
DccServer::IsConnected() const
{
   return mState >= DccServer::CONNECTED;
}

void
DccServer::Serve(std::string& rFileName)
{
   // TODO: write this
   mFileName = rFileName;
   mHost = 0;
   mPort = 0;
   mSock = -1;
   mState = DccServer::SERVING;
}

void
DccServer::Sleep()
{
   mState = DccServer::SLEEPING;
}

int
DccServer::GetHost() const
   throw (DccServer::NotServingException)
{
   if (!IsServing())
      throw DccServer::NotServingException();
   return mHost;
}

int
DccServer::GetPort() const
   throw (DccServer::NotServingException)
{
   if (!IsServing())
      throw DccServer::NotServingException();
   return mPort;
}

int
DccServer::GetSocket() const
   throw (DccServer::NotServingException)
{
   if (!IsServing())
      throw DccServer::NotServingException();
   return mSock;
}

const std::string&
DccServer::GetFileName() const
   throw (DccServer::NotServingException)
{
   if (!IsServing())
      throw DccServer::NotServingException();
   return mFileName;
}

int
DccServer::GetFileSize() const
   throw (DccServer::NotServingException)
{
   if (!IsServing())
      throw DccServer::NotServingException();
   // TODO calculate real file size
   int file_size = 0;
   return file_size;
}

std::ostream&
operator<<(std::ostream& os, const DccServer& server)
{
   os << "[DccServer state=";

   if (server.IsSleeping()) {
      os << "SLEEPING";
   } else { // SERVING or CONNECTED
      if (server.IsServing())
         os << "SERVING";
      else
         os << "CONNECTED";
      os << ", file=\"" << server.mFileName
         << ", size=" << server.GetFileSize()
         << "\", host=" << server.mHost
         << ", port=" << server.mPort;
   }
   os << "]";
   return os;
}
