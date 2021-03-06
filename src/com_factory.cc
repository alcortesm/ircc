#include "ComDownload.h"
#include "ComOffer.h"
#include "ComWho.h"
#include "ComList.h"
#include "ComDisconnect.h"
#include "ComLeave.h"
#include "ComAuth.h"
#include "ComHelp.h"
#include "ComNop.h"
#include "ComUnknown.h"
#include "ComQuit.h"
#include "ComConnect.h"
#include "ComError.h"
#include "ComMsg.h"
#include "ComSleep.h"
#include "ComJoin.h"
#include <ostream>
#include <sstream>
#include <stdexcept>
#include "ircc.h"
#include "irc.h"
#include "utils.h"

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
      return new ComError("The /connect command needs a host:port");
   
   // there must be exactly 1 space in "/command host:port"
   size_t space = rLine.find(SPACE, 0);
   size_t colon = rLine.find(IRCC_HOST_PORT_SEPARATOR, space+1);
   if (colon == string::npos)
      return new ComError("The /connect command also needs a port");
   size_t more_space =  rLine.find(SPACE, space+1);
   if (more_space != string::npos)
      return new ComError("The /connect command only needs a host:port");

   size_t host_start = space + 1;
   size_t host_len = colon - host_start;

   size_t port_start = colon + 1;
   size_t port_len = rLine.length() - port_start;

   string host(rLine, host_start, host_len);
   string port(rLine, port_start, port_len);

   return new ComConnect(rServer, host, port);
}

Command*
new_offer(DccServer& rDccServer, Server& rServer, const string& rLine)
{
   // if rLine is just the /offer command without arguments
   if (rLine == ComOffer::STR)
      return new ComError("The /offer command needs a nick and a file name");
   
   // there must be exactly 2 spaces in "/offer nick filename"
   size_t space1 = rLine.find(SPACE, 0);
   size_t space2 = rLine.find(SPACE, space1+1);
   if (space2 == string::npos)
      return new ComError("The /offer command needs a nick and a file name");
   size_t space3 =  rLine.find(SPACE, space2+1);
   if (space3 != string::npos)
      return new ComError("The /offer command needs a nick and a file name");

   size_t file_name_start = space1 + 1;
   size_t file_name_len = space2 - file_name_start;

   size_t nick_start = space2 + 1;
   size_t nick_len = rLine.length() - nick_start;

   string file_name(rLine, file_name_start, file_name_len);
   string nick(rLine, nick_start, nick_len);

   if (!rServer.IsConnected())
      return new ComError("Not connected to a server");

   if (!rServer.IsAuthenticated())
      return new ComError("Not authenticated with the server");

   return new ComOffer(rDccServer, rServer, nick, file_name);
}

Command*
new_download(const string& rLine)
{
   *gpDebug << FROM_DEBUG << "new_download(\"" << rLine << "\", "
            << ")" << endl;

   // if rLine is just the /download command without arguments
   if (rLine == ComDownload::STR)
      return new ComError("The /download command needs a host:port and a file name");

   // there must be exactly 2 spaces in "/download nick filename"
   const size_t space1 = rLine.find(SPACE, 0);
   const size_t space2 = rLine.find(SPACE, space1+1);
   if (space2 == string::npos)
      return new ComError("The /download command needs a host:port and a file name");
   const size_t space3 =  rLine.find(SPACE, space2+1);
   if (space3 != string::npos)
      return new ComError("The /download command needs a host:port and a file name");

   const size_t file_name_start = space1 + 1;
   const size_t file_name_len = space2 - file_name_start;

   const size_t host_port_start = space2 + 1;
   const size_t host_port_len = rLine.length() - host_port_start;

   const string host_port(rLine, host_port_start, host_port_len);
   const string file_name(rLine, file_name_start, file_name_len);

   const size_t colon = host_port.find(IRCC_HOST_PORT_SEPARATOR);
   if (colon == string::npos)
      return new ComError("The /download command also needs a port");

   const size_t host_start = 0;
   const size_t host_len = colon - host_start;

   const size_t port_start = colon + 1;
   const size_t port_len = host_port.length() - port_start;

   const string host(host_port, host_start, host_len);
   const string port(host_port, port_start, port_len);

   return new ComDownload(host, port, file_name);
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

   return new ComMsg(rLine.substr(ComMsg::STR.length()+1), rServer);
}

Command*
new_auth(Server& rServer, const string& rLine)
{
   // if line is just "/auth" -> error
   if (there_is_no_args(rLine))
      return new ComError("The /auth command needs and argument");

   string nick(rLine, ComAuth::STR.length()+1);
   return new ComAuth(nick, rServer);
}

Command*
new_join(Server& rServer, const string& rLine)
{
   // if line is just "/join" -> tell which channel you are on
   if (there_is_no_args(rLine)) {
      if (rServer.IsChannel()) {
         std::stringstream ss;
         ss << "You are on channel "
            << rServer.GetChannel() << std::endl;
         return new ComError(ss.str());
      } else {
         return new ComError("You are not on any channel");
      }
   }

   string channel(rLine, ComJoin::STR.length()+1);
   return new ComJoin(channel, rServer);
}

Command*
new_sleep(const string& rLine)
{
   // if line is just "/sleep"
   if (there_is_no_args(rLine))
      return new ComSleep(ComSleep::DEFAULT_SLEEP_TIME_SECS);

   const string duration_str(rLine, ComSleep::STR.length()+1);
   int duration_int;
   try {
      duration_int = string_to_int(duration_str);
   } catch (std::invalid_argument& e) {
      return new ComError("can not sleep: invalid time specification");
   }

   return new ComSleep(duration_int);
}

std::string
clean_line(std::string line)
{
   // replace 2 spaces with 1 space
   size_t two_spaces_pos;
   while (true) {
      two_spaces_pos = line.find("  ");
      if (two_spaces_pos == string::npos)
         break;
      line.replace(two_spaces_pos, 2, " ");
   }
   // remove last space
   if (line.at(line.length()-1) == ' ')
      line.erase(line.length()-1, 1);
   return line;
}

Command*
com_factory(const std::string& rLine, Server& rServer, DccServer& rDccServer)
{
   *gpDebug << FROM_DEBUG << "com_factory(\"" << rLine << "\", "
            << rServer << ", " << rDccServer << ")" << endl;

   /* NOP */
   if (rLine.empty())
      return new ComNop();
   if (starts_with(rLine,ComNop::STR))
      return new ComNop();
   
   /* MESG */
   if (rLine.at(0) != '/')
      return new ComMsg(rLine, rServer);
   if (starts_with(rLine, ComMsg::STR))
      return new_msg(rServer, rLine);

   /* clean the line to be nice with the other commands */
   string clean = clean_line(rLine);
   /*   *gpDebug << FROM_DEBUG
            << "com_factory() : clean = \""
            << clean << "\"" << std::endl; */

   /* LIST */
   if (starts_with(clean, ComList::STR))
      return new ComList(rServer);

   /* SLEEP */
   if (starts_with(clean, ComSleep::STR))
      return new_sleep(clean);

   /* AUTH */
   if (starts_with(clean, ComAuth::STR))
      return new_auth(rServer, clean);

   /* HELP */
   if (starts_with(clean, ComHelp::STR))
      return new ComHelp();

   /* JOIN */
   if (starts_with(clean, ComJoin::STR))
      return new_join(rServer, clean);

   /* LEAVE */
   if (starts_with(clean, ComLeave::STR))
      return new ComLeave(rServer);

   /* WHO */
   if (starts_with(clean, ComWho::STR))
      return new ComWho(rServer);

   /* QUIT */
   if (starts_with(clean,ComQuit::STR))
      return new ComQuit();

   /* OFFER */
   if (starts_with(clean, ComOffer::STR))
      return new_offer(rDccServer, rServer, clean);

   /* DOWNLOAD */
   if (starts_with(clean, ComDownload::STR))
      return new_download(clean);

   /* CONNECT */
   if (starts_with(clean, ComConnect::STR))
      return new_connect(rServer, clean);

   /* DISCONNECT */
   if (starts_with(clean, ComDisconnect::STR))
      return new ComDisconnect(rServer);

   /* UNKNOWN */
   return new ComUnknown(rLine);
}
