// -*- Mode: c++ -*-
#ifndef URL_H
#define URL_H

#include <string>    // USES, HASA & HASA-USES string interface
#include <stdexcept> // USES runtime_error & bad_alloc exceptions

#include <netinet/ip.h> // HASA struct in_addr

#include "DnsResolver.h" // HASA DnsResolver

class Url
{
public:
   class MalformedUrlException : public std::runtime_error {
   public:
      MalformedUrlException() : std::runtime_error("Malformed URL") { }
      MalformedUrlException(std::string s) : std::runtime_error("Malformed URL: " + s) { }
   };

   class NameResolutionException : public std::runtime_error {
   public:
      NameResolutionException() : std::runtime_error("Name resolution error") { }
      NameResolutionException(std::string s) : std::runtime_error("Name resolution error: " + s) { }
   };

   class NetworkException : public std::runtime_error {
   public:
      NetworkException() : std::runtime_error("Network error") { }
      NetworkException(std::string s) : std::runtime_error("Network error: " + s) { }
   };

   static const std::string DOMAIN_SEPARATOR;
   static const std::string PORT_SEPARATOR;
   static const std::string PATH_SEPARATOR;
   static const std::string QUERY_SEPARATOR;
   static const std::string ANCHOR_SEPARATOR;
   static const std::string PROTO_HTTP;
   static const uint16_t    HTTP_DEFAULT_PORT;
   static const std::string HTTP_DEFAULT_PORT_STRING;

   Url(const std::string & rUrl, const DnsResolver * pDnsResolver) throw (Url::MalformedUrlException, std::bad_alloc);
   static Url AddQuery(const Url & rUrl, const std::string & rQuery) throw (Url::MalformedUrlException, std::bad_alloc);

   const DnsResolver *    PDnsResolver() const { return mpDnsResolver; };
   const std::string &    Proto() const { return mProto; };
   uint16_t               Port() const { return mPort; };
   const std::string &    PortString() const { return mPortString; };
   uint16_t               PortNbo() const { return mPortNbo; };
   const std::string &    Domain() const { return mDomain; };
   const struct in_addr & Addr() const throw (Url::NameResolutionException, Url::NetworkException);
   const std::string &    Ip() const throw (Url::NameResolutionException, Url::NetworkException);
   const std::string &    Path() const { return mPath; };
   const std::string &    Query() const { return mQuery; };
   const std::string &    Anchor() const { return mAnchor; };
   const std::string &    Canonical() const { return mCanonical; };

   bool                   HasPath() const { return ! mPath.empty(); };
   bool                   HasQuery() const { return ! mQuery.empty(); };
   bool                   HasAnchor() const { return ! mAnchor.empty(); };

   static void            Test();

private:
   const DnsResolver*     mpDnsResolver;
   std::string            mProto;
   uint16_t               mPort;
   std::string            mPortString;
   uint16_t               mPortNbo; // port in network byte order
   std::string            mDomain;
   std::string            mPath;   // includes the path separator
   std::string            mQuery;  // includes the query separator
   std::string            mAnchor; // includes the anchor separator
   /* mAddr and mIp will only be resolved upon request (by its access methods)
      to prevent premature network traffic from the resolver */
   mutable struct in_addr mAddr;
   mutable std::string    mIp;
   std::string            mCanonical;

   Url(const DnsResolver * pDnsResolver,
       const std::string & rProto,
       const std::string & rPortString,
       const std::string & rDomain,
       const std::string & rPath,
       const std::string & rQuery  = "",
       const std::string & rAnchor = "") throw (Url::MalformedUrlException, std::bad_alloc);

   void Init(const DnsResolver * pDnsResolver,
             const std::string & rProto,
             const std::string & rPort,
             const std::string & rDomain,
             const std::string & rPath,
             const std::string & rQuery,
             const std::string & rAnchor) throw (Url::MalformedUrlException);

   void get_ip_and_addr() const throw (Url::NameResolutionException, Url::NetworkException);
};

std::ostream & operator<<(std::ostream & os, Url url);
#endif // URL_H
