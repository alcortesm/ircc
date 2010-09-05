#include "CommandFactory.h"
#include "ComNop.h"
#include "ComUnknown.h"
#include "ComQuit.h"
#include "ComConnect.h"
#include "ComError.h"
#include <ostream>
#include <stdexcept>
#include "ircc.h"

extern std::ostream* gpDebug;

using std::endl;
using std::string;

// returns true if haystack == needle [' ' [something]]
bool
starts_with(const string & haystack, const string & needle)
{
   size_t found;
   found = haystack.find(needle);
   if (found == string::npos)
      return false;
   if (found != 0)
      return false;

   try {
      const char& next = haystack.at(needle.length());
      if (next == ' ')
         return true; // haystack == needle + ' ' [ + something ]
   } catch (std::out_of_range & e) { // needle == haystack
      return true;
   }

   return false;
}

// Parse the (cleaned) line from the user to create a new ComConncet or
// a new ComError (if user imput was wrong).
Command*
new_connect_or_error(const string& rLine, Server& rServer)
{
   // if rLine is just the /connect command without arguments
   if (rLine == ComConnect::STR)
      return new ComError("The /connect command needs a hots and a port");
   
   // there must be exactly 2 spaces in "/command host port"
   size_t space1 = rLine.find(ARG_SEPARATORS, 0);
   size_t space2 = rLine.find(ARG_SEPARATORS, space1+1);
   if (space2 == string::npos)
      return new ComError("The /connect command needs a hots and a port");
   size_t space3 =  rLine.find(ARG_SEPARATORS, space2+1);
   if (space3 != string::npos)
      return new ComError("The /connect command only needs a hots and a port");

   size_t host_start = space1 + 1;
   size_t host_len = space2 - host_start;

   size_t port_start = space2 + 1;
   size_t port_len = rLine.length() - port_start;

   string host(rLine, host_start, host_len);
   string port(rLine, port_start, port_len);

   return new ComConnect(rServer, host, port);
}

Command*
CommandFactory::Build(const std::string& rLine, Server& rServer)
   throw (std::runtime_error)
{



   /* NOP */
   /* if the user pressed only enter: nop command */
   if (rLine.empty()) {
      return new ComNop();
   }
   /* nop */
   if (starts_with(rLine,ComNop::STR)) {
      return new ComNop();
   }
   


   /* MESG */
   if (rLine.at(0) != '/' || starts_with(rLine, "/msg")) {
      return new ComError("/msg command not yet implemmented");
   }



   /* QUIT */
   if (starts_with(rLine,ComQuit::STR)) {
      return new ComQuit();
   }


   /* CONNECT */
   if (starts_with(rLine, ComConnect::STR)) {
      return new_connect_or_error(rLine, rServer);
   }


   /* UNKNOWN */
   return new ComUnknown(rLine);
}
