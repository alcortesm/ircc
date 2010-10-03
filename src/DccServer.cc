#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "DccServer.h"
#include "utils.h"
#include "ircc.h"
#include <ostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <stdlib.h>
#include <climits>
#include <netdb.h>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>

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
   throw (DccServer::AlreadyListeningException
          , DccServer::FileException
          , DccServer::ListenException)
{
   *gpDebug << FROM_DEBUG
            << "DccServer::Listen("
            << "\"" << rFileName << "\""
            << ")" << std::endl;


   if (!IsSleeping())
      throw DccServer::AlreadyListeningException();


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



   // get protocol of TCP
   int proto;
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
            ss << "DccServer::Listen() : getprotobyname_r returned ERANGE : a buffer of "
               << buf_sz
               << " is too smal for the struct protoent, try again with a bigger buffer";
            throw std::runtime_error(ss.str());
         }
         std::stringstream ss;
         ss << "DccServer::Listen() : getprotobyname_r returned the unknown error "
            << r;
         throw std::runtime_error(ss.str());
      }
      if (p_pe == NULL) { /* protocol not found */
         std::stringstream ss;
         ss << "DccServer::Listen() : getprotobyname_r : protocol \""
            << TCP.c_str()
            << "\" not found";
         throw std::runtime_error(ss.str());
      }
      proto = pe.p_proto;
      free(buf);
   }

   // create the socket
   mSocket = socket(AF_INET, SOCK_STREAM, proto);
   if (mSocket == -1) {
      size_t buf_sz = 1024; /* I hope is enough for errstr */
      char* buf = (char *) xmalloc(buf_sz);
      char* r;
      r = strerror_r(errno, buf, buf_sz);
      DccServer::ListenException e(r);
      free(buf);
      throw e;
   }

   // get my local addr
   struct addrinfo hints;
   {
      memset(&hints, 0, sizeof hints);
      hints.ai_flags = AI_PASSIVE;
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = proto;
   }
   struct sockaddr_in my_addr;
   {
      struct addrinfo* p_ai;
      int error;
      error = getaddrinfo(NULL, NULL, &hints, &p_ai);
      if (error) {
         DccServer::ListenException e(gai_strerror(error));
         throw e;
      }
      memcpy(&my_addr, p_ai->ai_addr, sizeof my_addr);
      freeaddrinfo(p_ai);
   }

   // bind to local addr
   {
      struct sockaddr *p_sa = (struct sockaddr *) &my_addr;
      int r;
      r = bind(mSocket, p_sa, sizeof(struct sockaddr));
      if (r == -1) {
         size_t buf_sz = 1024; /* I hope is enough for errstr */
         char* buf = (char *) xmalloc(buf_sz);
         char* r;
         r = strerror_r(errno, buf, buf_sz);
         DccServer::ListenException e(r);
         free(buf);
         throw e;
      }
   }

   // listen in local addr
   {
      int r;
      r = listen(mSocket, BACK_LOG);
      if (r == -1) {
         size_t buf_sz = 1024; /* I hope is enough for errstr */
         char* buf = (char *) xmalloc(buf_sz);
         char* r;
         r = strerror_r(errno, buf, buf_sz);
         DccServer::ListenException e(r);
         free(buf);
         throw e;
      }
   }

   mHost = std::string(inet_ntoa(my_addr.sin_addr));
   mPort = my_addr.sin_port;

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

unsigned long
DccServer::GetFileSize() const
   throw (DccServer::FileException, DccServer::NotServingException)
{
   if (!IsListening())
      throw DccServer::NotServingException();

   std::string file_path_string("./");
   file_path_string.append(GetFileName());
   const char* file_path = file_path_string.c_str();

   struct stat file_stats;
   int r;
   r = stat(file_path, &file_stats);
   if (r == -1) {
      size_t buf_sz = 1024; /* I hope is enough for errstr */
      char* buf = (char *) xmalloc(buf_sz);
      char* r;
      r = strerror_r(errno, buf, buf_sz);
      DccServer::FileException e(mFileName, r);
      free(buf);
      throw e;
   }

   return (unsigned long) file_stats.st_size;
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
