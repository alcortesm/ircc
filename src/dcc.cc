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

// TODO: the followin functions to parse the DCC msg should be replaced
// by a vector<string> dcc_msg_to_vector(const string& str) function
// that tokenize the msg.
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

std::string
get_file_name_from_dcc_send_msg(const std::string& str)
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

   size_t argument_length = space_after_argument - argument;
   return str.substr(argument, argument_length);
}

size_t
addr_pos(const string& str)
{
   size_t argument = argument_pos(str);

   // no argument found
   if (argument == string::npos)
      return string::npos;

   size_t space_after_argument = str.find(" ", argument);

   if (space_after_argument == string::npos)
      return string::npos;

   // check if no addr after argument
   if (space_after_argument + 1 >= str.length())
      return string::npos;

   return space_after_argument + 1;
}

std::string
get_addr_from_dcc_msg(const std::string& str)
{
   if (!is_dcc_msg(str))
      return string();

   size_t addr = addr_pos(str);
   if (addr == string::npos)
      return string();

   size_t space_after_addr = str.find(" ", addr);

   // malformed DCC SEND, nothing after addr
   if (space_after_addr == string::npos)
      return string();

   size_t addr_length = space_after_addr - addr;
   return str.substr(addr, addr_length);
}

size_t
port_pos(const string& str)
{
   size_t addr = addr_pos(str);

   // no addr found
   if (addr == string::npos)
      return string::npos;

   size_t space_after_addr = str.find(" ", addr);

   if (space_after_addr == string::npos)
      return string::npos;

   // check if no port after addr
   if (space_after_addr + 1 >= str.length())
      return string::npos;

   return space_after_addr + 1;
}

std::string
get_port_from_dcc_msg(const std::string& str)
{
   if (!is_dcc_msg(str))
      return string();

   size_t port = port_pos(str);
   if (port == string::npos)
      return string();

   size_t space_after_port = str.find(" ", port);

   // malformed DCC SEND, nothing after port
   if (space_after_port == string::npos)
      return string();

   size_t port_length = space_after_port - port;
   return str.substr(port, port_length);
}

size_t
file_size_pos(const string& str)
{
   size_t port = port_pos(str);

   // no port found
   if (port == string::npos)
      return string::npos;

   size_t space_after_port = str.find(" ", port);

   if (space_after_port == string::npos)
      return string::npos;

   // check if no file_size after port
   if (space_after_port + 1 >= str.length())
      return string::npos;

   return space_after_port + 1;
}

std::string
get_file_size_from_dcc_send_msg(const std::string& str)
{
   if (!is_dcc_send_msg(str))
      return string();

   size_t file_size = file_size_pos(str);
   if (file_size == string::npos)
      return string();

   size_t space_after_file_size = str.find(" ", file_size);

   // malformed DCC SEND, something after file_size
   if (space_after_file_size != string::npos)
      return string();

   size_t ctcp_x_delim_after_file_size = str.find(CTCP_X_DELIM, file_size);

   size_t file_size_length = ctcp_x_delim_after_file_size - file_size;
   return str.substr(file_size, file_size_length);
}
