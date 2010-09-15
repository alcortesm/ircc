#include <sys/types.h>
#include <sys/socket.h>
#include "DccServer.h"
#include "utils.h"
#include "ircc.h"
#include <ostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <stdlib.h>

extern std::ostream* gpDebug;

DccServer::DccServer()
   : mHost()
   , mPort()
   , mSocket()
   , mState(DccServer::SLEEPING)
{
   *gpDebug << FROM_DEBUG
            << "DccServer::DccServer()"
            << std::endl;
}

DccServer::~DccServer()
{
   *gpDebug << FROM_DEBUG
            << "DccServer::~DccServer()"
            << std::endl;
   Sleep();
}

bool
DccServer::IsSleeping() const
{
   return mState == DccServer::SLEEPING;
}

bool
DccServer::IsListening() const
{
   return mState == DccServer::LISTENING;
}

bool
DccServer::IsConnected() const
{
   return mState == DccServer::CONNECTED;
}

void
DccServer::Listen(const std::string& rFileName)
   throw (DccServer::FileException, DccServer::ListenException)
{
   *gpDebug << FROM_DEBUG
            << "DccServer::Listen("
            << "\"" << rFileName << "\""
            << ")" << std::endl;

   // check if rFileName exists
   std::string path("./");
   path.append(rFileName);
   int fd = open(path.c_str(), O_RDONLY);
   if (fd == -1) {
      size_t buf_sz = 1024; /* I hope is enough for errstr */
      char* buf = (char *) xmalloc(buf_sz);
      char* r;
      r = strerror_r(errno, buf, buf_sz);
      DccServer::FileException e(rFileName, r);
      free(buf);
      throw e;
   }
   close(fd);
   mFileName = rFileName;


   mHost = std::string("192.168.1.1");
   mPort = 2345;
   mSocket = socket(AF_INET, SOCK_STREAM, 0);
   mState = DccServer::LISTENING;
}

void
DccServer::Sleep()
{
   *gpDebug << FROM_DEBUG
            << "DccServer::Sleep()"
            << std::endl;

   if (mState != DccServer::SLEEPING)
      close(mSocket);
   mState = DccServer::SLEEPING;
}

int
host_to_int(const std::string& rHost)
{
   UNUSED(rHost);
   return 12;
}

const std::string&
DccServer::GetHost() const
   throw (DccServer::NotServingException)
{
   if (IsSleeping())
      throw DccServer::NotServingException();
   return mHost;
}

int
DccServer::GetHostInt() const
   throw (DccServer::NotServingException)
{
   if (!IsListening())
      throw DccServer::NotServingException();
   return host_to_int(mHost);
}

int
DccServer::GetPort() const
   throw (DccServer::NotServingException)
{
   if (!IsListening())
      throw DccServer::NotServingException();
   return mPort;
}

int
DccServer::GetSocket() const
   throw (DccServer::NotServingException)
{
   if (!IsListening())
      throw DccServer::NotServingException();
   return mSocket;
}

const std::string&
DccServer::GetFileName() const
   throw (DccServer::NotServingException)
{
   if (!IsListening())
      throw DccServer::NotServingException();
   return mFileName;
}

int
get_file_size(const std::string& file_name)
{
   UNUSED(file_name);
   return 1234567;
}

int
DccServer::GetFileSize() const
   throw (DccServer::NotServingException)
{
   if (!IsListening())
      throw DccServer::NotServingException();

   int file_size = get_file_size(mFileName);
   return file_size;
}

std::ostream&
operator<<(std::ostream& os, const DccServer& server)
{
   os << "[DccServer state=";

   if (server.IsSleeping()) {
      os << "SLEEPING";
   } else { // SERVING or CONNECTED
      if (server.IsListening())
         os << "LISTENING";
      else
         os << "CONNECTED";
      os << ", file=\"" << server.mFileName << "\""
         << ", size=" << server.GetFileSize()
         << ", host=\"" << server.mHost << "\""
         << ", port=" << server.mPort;
   }
   os << "]";
   return os;
}
