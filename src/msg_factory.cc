#include "msg_factory.h"
#include "irc.h"
#include "ircc.h"
#include "utils.h"
#include <ostream>
#include "NullStream.h"
#include <sstream>
#include <iostream>
#include "ComJoin.h"

extern std::ostream* gpDebug;

using std::string;
using std::vector;

// See rfc2812 section 2.3.1 fomr message format


bool
inline
there_is_a_prefix(const string& rLine)
{
   return (rLine.at(0) == PREFIX_PREFIX);
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
   if (there_is_a_prefix(rLine)) {
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
   //   *gpDebug << FROM_DEBUG << "  command_first = " << command_first(rLine) << std::endl ;
   //   *gpDebug << FROM_DEBUG << "  command_last  = " << command_last(rLine) << std::endl ;
   return rLine.substr(command_first(rLine),
                       command_last(rLine)-command_first(rLine) + 1);
}




size_t
inline
params_first(const string& rLine)
{
   //   *gpDebug << FROM_DEBUG << "  -- param_first() : command_last = " << command_last(rLine) << std::endl;
   if (rLine.length() > (command_last(rLine) + 1))
      return (command_last(rLine) + 2); // +2 means "the next to SPACE"
   else
      return string::npos;
}



vector<string>
get_params(const string& rLine)
{
   // we will only work with the params portion or the line
   //   *gpDebug << FROM_DEBUG << "  -- get_param() : rLine = \"" << rLine << "\"" << std::endl;
   string only_params_with_terminator = rLine.substr(params_first(rLine));
   size_t terminator_first = only_params_with_terminator.rfind(END_OF_MESSAGE);
   string only_params = only_params_with_terminator.substr(0, terminator_first);
   //   *gpDebug << FROM_DEBUG << "  -- get_param() : only_params = \"" << only_params << "\"" << std::endl;

   if (only_params.empty())
      return vector<string>();

   // if there is only the "colon param", there is only one param and we
   // can return a very simple vector
   if (only_params.at(0) == ':') {
      string param = only_params.substr(1);
      vector<string> v;
      v.push_back(param);
      return v;
   }

   // let's find if there is a last param starting with ':'
   // if there is, strip it from our working str and store it away
   // for later processing
   string no_colon_params;
   string colon_param;
   {
      // as there is regular params and then the "colon param" we have
      // to look for " :"
      size_t colon_pos = only_params.find(COLON_PARAM_SEPARATOR);
      if (colon_pos == string::npos) {
         no_colon_params = only_params;
         colon_param = string();
      } else {
         no_colon_params = only_params.substr(0, colon_pos);
         colon_param = only_params.substr(colon_pos + COLON_PARAM_SEPARATOR.length());
      }
   }
   //   *gpDebug << FROM_DEBUG << "  -- get_param() : no_colon_params = \"" << no_colon_params << "\"" << std::endl;
   //   *gpDebug << FROM_DEBUG << "  -- get_param() : colon_param = \"" << colon_param << "\"" << std::endl;
      
   // now, build the vector of params
   vector<string> v;
   // if there are no_colon_params
   if (!no_colon_params.empty()) {
      size_t current = 0;
      while (true) {
         
         size_t next_space = no_colon_params.find(SPACE, current);
         
         string param = (next_space == string::npos)
            ? no_colon_params.substr(current)
            : no_colon_params.substr(current, (next_space - current));
         
         //         *gpDebug FROM_DEBUG << << "  -- -- get_param() : current = " << current << std::endl;
         //         *gpDebug FROM_DEBUG << << "  -- -- get_param() : next_space = " << next_space << std::endl;
         //         *gpDebug FROM_DEBUG << << "  -- -- get_param() : adding the string \"" << param << "\" to the vector" << std::endl;  
         v.push_back(param);
          
         if (next_space == string::npos)
            break;
         else
            current = next_space + 1;
      }
   } // end of if there no_colon_params
   // if there is a colon_param
   if (!colon_param.empty()) {
      //      *gpDebug << FROM_DEBUG << "  -- -- get_param() : adding the colon string \"" << colon_param << "\" to the vector" << std::endl;
      v.push_back(colon_param);
   }

   return v;
}


const Msg*
msg_factory(const string& rLine)
{
   //   *gpDebug << FROM_DEBUG << "msg_factory(\"" << rLine.substr(0, rLine.rfind(END_OF_MESSAGE)) << "\", " << rServer << ")" << std::endl ;

   string prefix = get_prefix(rLine);
   string command = get_command(rLine);
   vector<string> params = get_params(rLine);
   const Msg* p_msg = new Msg(prefix, command, params);
   return p_msg;
}
