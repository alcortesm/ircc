#include "dcc.h"
#include "ctcp.h"

using std::string;

// DCC SEND message example:
//   [\001]DCC SEND main.cc 12 2345 6342[\001]
//   DCC type argument addr port [size]
//   if type == SEND, argument is file_name and size is file_size

bool
is_dcc_msg(const string& str)
{
   if (is_ctcp_msg(str)
       && str.substr(CTCP_X_DELIM.length(), DCC_PREFIX.length()) == DCC_PREFIX)
      return true;
   return false;
}

bool
is_dcc_send_msg(const string& str)
{
   if (is_ctcp_msg(str)
       && is_dcc_msg(str)
       && str.substr(CTCP_X_DELIM.length(), DCC_SEND_PREFIX.length()) == DCC_SEND_PREFIX)
      return true;
   return false;
}

size_t
type_pos(const string& str)
{
   if (!is_dcc_msg(str))
      return string::npos;

   size_t first_space = str.find(" ");

   // check if no space
   if (first_space == string::npos)
      return string::npos;

   // check if only a space at the end
   if (first_space+1 >= str.length())
      return string::npos;

   return first_space + 1;
}

size_t
argument_pos(const string& str)
{
   size_t type = type_pos(str);

   // no type found
   if (type == string::npos)
      return string::npos;

   size_t space_after_type = str.find(" ", type);

   if (space_after_type == string::npos)
      return string::npos;

   // check if no argument after type
   if (space_after_type + 1 >= str.length())
      return string::npos;

   return space_after_type + 1;
}

std::string get_file_name_from_dcc_send_msg(const std::string& str)
{
   if (!is_dcc_send_msg(str))
      return string();

   size_t argument = argument_pos(str);
   if (argument == string::npos)
      return string();

   size_t space_after_argument = str.find(" ", argument);

   // malformed DCC SEND, nothing after argument
   if (space_after_argument == string::npos)
      return string();

   return str.substr(argument, space_after_argument -1);
}
