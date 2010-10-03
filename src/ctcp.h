#ifndef CTCP_H
#define CTCP_H

#include <string>

const std::string CTCP_X_DELIM = "\001";

bool is_ctcp_msg(const std::string& str);

#endif // CTCP_H
