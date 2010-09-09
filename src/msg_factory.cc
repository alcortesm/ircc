#include "msg_factory.h"
#include "MsgTellUser.h"
#include "irc.h"
#include "ircc.h"
#include "utils.h"
#include <ostream>

extern std::ostream* gpDebug;

using std::string;
using std::vector;

// See rfc2812 section 2.3.1 fomr message format


bool
inline
there_is_a_prefix(const string& rLine)
{
   return (rLine.at(1) != PREFIX_PREFIX);
}

size_t
inline
prefix_first(const string& rLine)
{
   if (there_is_a_prefix(rLine))
      return 1;
   else
      return string::npos;
}

//return 0 if no prefix
size_t
inline
prefix_last(const string& rLine)
{
   if (there_is_a_prefix(rLine))
      return (rLine.find(SPACE) - 1);
   else
      return string::npos;
}

string
get_prefix(const string& rLine)
{
   *gpDebug << "  prefix_first = " << prefix_first(rLine) << std::endl ;
   *gpDebug << "  prefix_last  = " << prefix_last(rLine) << std::endl ;
   if (there_is_a_prefix) {
      size_t size = prefix_last(rLine) - prefix_first(rLine) + 1;
      return rLine.substr(prefix_first(rLine), size);
   }
   return string("");
}





size_t
inline
command_first(const string& rLine)
{
   return (there_is_a_prefix(rLine))
      ? (prefix_last(rLine) + 2) // +2 means "the next to SPACE"
      : 0 ;
}

size_t
inline
command_last(const string& rLine, size_t com_first)
{
   return rLine.find(SPACE, com_first) - 1;
}

size_t
inline
command_last(const string& rLine)
{
   size_t com_first = command_first(rLine);
   return command_last(rLine, com_first);
}

string
get_command(const string& rLine)
{
   *gpDebug << "  command_first = " << command_first(rLine) << std::endl ;
   *gpDebug << "  command_last  = " << command_last(rLine) << std::endl ;
   return rLine.substr(command_first(rLine),
                       command_last(rLine)-command_first(rLine) + 1);
}




size_t
inline
param_first(const string& rLine)
{
   if (rLine.length() > (command_last(rLine) + 1))
      return (command_last(rLine) + 2); // +2 means "the next to SPACE"
   else
      return string::npos;
}

vector<string>
get_params(const string& rLine)
{
   vector<string> params;
   params.push_back(rLine.substr(param_first(rLine)));
   return params;
}




Msg*
msg_factory(const string& rLine)
{
   *gpDebug << "line = \"" << rLine << "\"" << std::endl ;
   *gpDebug << "prefix = \"" << get_prefix(rLine) << "\"" << std::endl ;
   *gpDebug << "command = \"" << get_command(rLine) << "\"" << std::endl ;
   *gpDebug << "params(1) = \"" << (get_params(rLine))[0] << "\"" << std::endl ;

   return new MsgTellUser(
                          get_prefix(rLine),
                          get_command(rLine),
                          get_params(rLine));
}
