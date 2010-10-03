#ifndef DCC_H
#define DCC_H

#include <string>

const std::string DCC_PREFIX = "DCC ";
const std::string DCC_SEND_PREFIX = "DCC SEND ";

bool is_dcc_msg(const std::string& str);
bool is_dcc_send_msg(const std::string& str);

#endif // DCC_H
