#include "ComHelp.h"
#include <ostream>
#include <iostream>
#include "ircc.h"

extern std::ostream* gpDebug;

const std::string ComHelp::STR("/help");
const std::string ComHelp::MSG("*** A typical user session goes like this:\n\
*** \n\
***     /connect localhost:6691\n\
***     /auth alcortes\n\
***     /list\n\
***     /join #ro2\n\
***     hello\n\
***     /leave\n\
***     /disconnect\n\
***     /quit\n\
*** \n\
*** The full list of available commands are\n\
*** \n\
***     /help                    Show this text.\n\
***     /connect <host>:<port>   Connect to a server.\n\
***     /auth <nick>             Authenticate with the server,\n\
***                              sets nick, username & real name.\n\
***     /list                    Lists all current irc channels,\n\
***                              number of users, and topic.\n\
***     /join [<channel>]        Join a channel or tell which is\n\
***                              the current channel.\n\
***     /leave                   Leaves your current channel.\n\
***     /who                     List users in current channel.\n\
***     /msg <txt>               Send <txt> to the channel.\n\
***     /offer <file> <nick>     Launch a DCC server for a file\n\
***                              transfer.  The recipient must accept\n\
***                              your offer with a '/download'.\n\
***     /download <host:port> <file>\n\
***                              Starts a file transfer by DCC.\n\
***                              The  sender must first have offered\n\
***                              the file with an '/offer' command.\n\
***     /disconnect              Disconnect from server.\n\
***     /quit                    Close program.\n\
***     /nop [<text>]            (debugging) no operation,\n\
***                              just echoes <text> back.\n\
***     /sleep [<secs>]          (debugging) pause for <secs>\n\
***                              seconds.\n\
***\n\
*** Lines from the user that don't begin with '/' are equivalent to\n\
*** \n\
***     /msg line\n\
*** \n\
*** Lines beginning with \"*** \" are messages from the program.\n\
*** Lines beginning with \"<user> \" are messages from <user>.\n\
*** Lines beginning with \"DDD \" are debug messages from the program.\n\
*** Lines beginning with \"> \" are your messages to other users.\n\
*** The rest of the lines are echoes of the user commands.");

void
ComHelp::Run() {
   std::cout << ComHelp::MSG << std::endl ;
   return ;
}

ComHelp::ComHelp() {
   *gpDebug << FROM_DEBUG << "ComHelp::ComHelp()" << std::endl;
}
