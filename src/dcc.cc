#include "dcc.h"
#include "ctcp.h"

bool
is_dcc_msg(const std::string& str)
{
   if (is_ctcp_msg(str)
       && str.substr(CTCP_X_DELIM.length(), DCC_PREFIX.length()) == DCC_PREFIX)
      return true;
   return false;
}

bool
is_dcc_send_msg(const std::string& str)
{
   if (is_ctcp_msg(str)
       && is_dcc_msg(str)
       && str.substr(CTCP_X_DELIM.length(), DCC_SEND_PREFIX.length()) == DCC_SEND_PREFIX)
      return true;
   return false;
}
