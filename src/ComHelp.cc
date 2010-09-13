#include "ComHelp.h"
#include <ostream>
#include <iostream>

extern std::ostream* gpDebug;

const std::string ComHelp::STR("/help");
const std::string ComHelp::MSG("*** A typical user session goes like this:\n\
*** \n\
***     /connect localhost 6757\n\
***     /auth alcortes\n\
***     /list\n\
***     /join #ro2\n\
***     /who\n\
***     hello\n\
***     /msg hello again\n\
***     /join\n\
***     /disconnect\n\
***     /quit\n\
*** \n\
*** The full list of available commands are\n\
*** \n\
***     /connect <host> <port>   - connect to a server\n\
***     /auth <nick>             - authenticate to the server.\n\
***                                set nick, username & real name)\n\
***     /list                    - list the channels in the server\n\
***     /who                     - list users in your channel\n\
***     /join [<channel>]        - join a channel or leave the channel\n\
***                                if no arg.\n\
***     /msg <txt>               - send <txt> to the channel\n\
***     /disconnect              - disconnect from server\n\
***     /quit                    - close program\n\
***     /echo <txt>              - echoes <txt> back (debugging)\n\
***     /nop                     - no operation, do nothing (debugging)\n\
***     /sleep [<secs>]          - pause for <secs> seconds (debugging)\n\
***     /help                    - show this text\n\
*** \n\
*** Lines from the user that don't begin with '/' are equivalent to\n\
*** \n\
***     /msg line\n\
*** \n\
*** Lines beginning with \"*** \" are messages from the program to the user.\n\
*** Lines beginning with \"<user> \" are messages from some user.\n\
*** Lines beginning with \"<<< \" are from the server.\n\
*** Lines beginning with \"DDD \" are debug messages from the program.\n\
*** The rest of the lines are echoes of the user commands.");

void
ComHelp::Run() {
   std::cout << ComHelp::MSG << std::endl ;
   return ;
}
