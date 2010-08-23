#ifndef DNSRESOLVER_H
#define DNSRESOLVER_H

#include <stdexcept>
#include <string>

class DnsResolver
{
 public:
   class NetworkException : public std::runtime_error {
   public:
      NetworkException() : std::runtime_error("DnsResolver Network error") { }
      NetworkException(std::string s) : std::runtime_error("DnsResolver Network error: " + s) { }
   };

   virtual void resolve(struct in_addr * p_ia, std::string const & domain) const throw (DnsResolver::NetworkException) ;
};

#endif /* DNSRESOLVER_H */
