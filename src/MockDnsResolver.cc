#include "MockDnsResolver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <ostream>
#include "utils.h"

extern std::ostream* gpDebug;

void
MockDnsResolver::resolve(struct in_addr * p_ia, std::string const & domain) const
throw (DnsResolver::NetworkException)
{
   *gpDebug << "MockDnsResolver::resovle() : called with argument \""
            << domain << "\"" << std::endl ;

   struct addrinfo * ai;
   int error;
   /* resolve the domain name into a list of addresses */
   error = getaddrinfo("localhost", NULL, NULL, &ai);
   if (error != 0)
      throw DnsResolver::NetworkException(gai_strerror(error));

   struct addrinfo * curr_ai;
   struct sockaddr_in * sin;
   for (curr_ai=ai; curr_ai!=NULL; curr_ai=curr_ai->ai_next) {
      if (curr_ai->ai_addr->sa_family == AF_INET) {
         sin = (struct sockaddr_in *) curr_ai->ai_addr;
         memcpy(p_ia, &sin->sin_addr, sizeof(sin->sin_addr));
         freeaddrinfo(ai);
         return;
      }
   }
   freeaddrinfo(ai);
   throw DnsResolver::NetworkException("no ipv4 address found");   
}
