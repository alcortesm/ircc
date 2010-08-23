#include "DnsResolver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <ostream>

extern std::ostream* gpDebug;

void
DnsResolver::resolve(struct in_addr * p_ia, std::string const & domain) const
throw (DnsResolver::NetworkException)
{
   *gpDebug << "DnsResolver::resovle() : called with argument \"" << domain << "\"" << std::endl ;

   struct addrinfo * ai;
   int error;
   /* resolve the domain name into a list of addresses */
   error = getaddrinfo(domain.c_str(), NULL, NULL, &ai);
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
