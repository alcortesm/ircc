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
***     hello\n\
***     /leave\n\
***     /disconnect\n\
***     /quit\n\
*** \n\
*** The full list of available commands are\n\
*** \n\
***     /help                    - show this text\n\
*** \n\
***     /connect <host> <port>   - connect to a server\n\
***\n\
***     /auth <nick>             - authenticate with the server,\n\
***                                sets nick, username & real name\n\
***\n\
***     /list                    - list available public channels\n\
***\n\
***     /who                     - list users in your channel\n\
***\n\
***     /join [<channel>]        - join a channel or tell what channel\n\
***                                you are on\n\
***\n\
***     /msg <txt>               - send <txt> to the channel\n\
***\n\
***     /disconnect              - disconnect from server\n\
***\n\
***     /quit                    - close program\n\
***\n\
***     /nop [<text>]            - (debugging) no operation,\n\
***                                just echoes <text> back\n\
***\n\
***     /sleep [<secs>]          - (debugging) pause for <secs> seconds\n\
***\n\
*** Lines from the user that don't begin with '/' are equivalent to\n\
*** \n\
***     /msg line\n\
*** \n\
*** Lines beginning with \"*** \" are messages from the program.\n\
*** Lines beginning with \"<user> \" are messages from <user>.\n\
*** Lines beginning with \"DDD \" are debug messages from the program.\n\
*** The rest of the lines are echoes of the user commands.");

void
ComHelp::Run() {
   std::cout << ComHelp::MSG << std::endl ;
   return ;
}
