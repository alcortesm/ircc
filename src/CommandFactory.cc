#include "CommandFactory.h"
#include "ComNop.h"
#include "ComUnknown.h"
#include "ComQuit.h"
#include "ComConnect.h"
#include "ComError.h"
#include "ComMsg.h"
#include "ComNick.h"
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
new_connect(Server& rServer, const string& rLine)
{
   // if rLine is just the /connect command without arguments
   if (rLine == ComConnect::STR)
      return new ComError("The /connect command needs a hots and a port");
   
   // there must be exactly 2 spaces in "/command host port"
   size_t space1 = rLine.find(SPACE, 0);
   size_t space2 = rLine.find(SPACE, space1+1);
   if (space2 == string::npos)
      return new ComError("The /connect command needs a hots and a port");
   size_t space3 =  rLine.find(SPACE, space2+1);
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

// Returns true if the line is just: "word [ ' ' ]" the check for the
// ending space is needed for "/msg" as it's line is not cleaned before
// reaching here, so an ending space is posible.
bool
there_is_no_args(const string& rLine)
{
   size_t space = rLine.find(SPACE);

   // there is no space, just a word
   if (space == string::npos)
      return true;

   // the first space is the last char in the string
   if (rLine.length()-1 == space)
      return true;

   return false;
}

// TODO, strip /msg from line before calling ComMsg ctor
Command*
new_msg(Server& rServer, const string& rLine)
{
   if (there_is_no_args(rLine))
      return new ComError("can not send message: which message?");

   return new ComMsg(rLine, rServer);
}

Command*
new_nick(Server& rServer, const string& rLine)
{
   // if line is just "/nick" -> error
   if (there_is_no_args(rLine))
      return new ComError("can not set nick: what is your nick?");

   string nick(rLine, ComNick::STR.length()+1);
   return new ComNick(nick, rServer);
}

Command*
CommandFactory::Build(const std::string& rLine, Server& rServer)
   throw (std::runtime_error)
{

   *gpDebug << "CommandFactory::Build(\"" << rLine << "\", "
            << rServer << ")" << endl;

   /* NOP */
   if (rLine.empty())
      return new ComNop();

   /* nop */
   if (starts_with(rLine,ComNop::STR))
      return new ComNop();
   
   /* MESG */
   if (rLine.at(0) != '/')
      return new ComMsg(rLine, rServer);
   if (starts_with(rLine, ComMsg::STR))
      return new_msg(rServer, rLine);

   /* NICK */
   if (starts_with(rLine, ComNick::STR))
      return new_nick(rServer, rLine);

   /* QUIT */
   if (starts_with(rLine,ComQuit::STR))
      return new ComQuit();

   /* CONNECT */
   if (starts_with(rLine, ComConnect::STR))
      return new_connect(rServer, rLine);

   /* UNKNOWN */
   return new ComUnknown(rLine);
}
