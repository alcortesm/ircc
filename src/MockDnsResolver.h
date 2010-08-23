#ifndef MOCKDNSRESOLVER_H
#define MOCKDNSRESOLVER_H

#include "DnsResolver.h"

class MockDnsResolver : public DnsResolver
{
 public:
   void resolve(struct in_addr * p_ia, std::string const & domain) const throw (DnsResolver::NetworkException);
};
#endif /* MOCKDNSRESOLVER_H */
