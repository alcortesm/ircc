#include "ctcp.h"

bool
is_ctcp_msg(const std::string& str) {
   size_t ctcp_x_delim_length = CTCP_X_DELIM.length();
   if (str.substr(0, ctcp_x_delim_length) == CTCP_X_DELIM
       && str.substr(str.length()-ctcp_x_delim_length) == CTCP_X_DELIM)
      return true;
   return false;
}
