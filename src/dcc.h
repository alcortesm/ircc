#ifndef DCC_H
#define DCC_H

#include <string>

const std::string DCC_PREFIX = "DCC ";
const std::string DCC_SEND_PREFIX = "DCC SEND ";

bool is_dcc_msg(const std::string& str);
bool is_dcc_send_msg(const std::string& str);

std::string get_file_name_from_dcc_send_msg(const std::string& str);
std::string get_addr_from_dcc_msg(const std::string& str);
std::string get_port_from_dcc_msg(const std::string& str);
std::string get_file_size_from_dcc_send_msg(const std::string& str);

#endif // DCC_H
