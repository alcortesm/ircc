#include "msg_factory.h"
#include "MsgTellUser.h"
#include "irc.h"
#include "utils.h"

using std::string;
using std::vector;

bool
there_is_a_prefix(const string& rLine)
{
   return (rLine.at(1) != PREFIX_PREFIX);
}

string
get_prefix(const string& rLine)
{
   if (there_is_a_prefix(rLine))
      return string("");
   return string("");
}

string
get_command(const string& rLine)
{
   if (there_is_a_prefix(rLine))
      return string("");
   return string("");
}

vector<string>
get_params(const string& rLine)
{
   UNUSED(rLine);
   return vector<string>();
}

Msg*
msg_factory(const string& rLine)
{
   string prefix = get_prefix(rLine);
   string command = get_command(rLine);
   vector<string> params = get_params(rLine);

   return new MsgTellUser(prefix, command, params);
}
