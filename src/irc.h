#ifndef IRC_H
#define IRC_H

#include <string>

// protocol constants
const int SERVERNAME_MAX_LENGTH   = 63; /* RFC2812 1.1 */
const int NICKNAME_MAX_LENGTH     = 9;  /* RFC2812 1.2.1, longer
                                           nicknames are not supported
                                           as no future evolutions of
                                           the protocol are supported by
                                           this software */
const int  CHANNELNAME_MAX_LENGTH  = 50;  /* RFC2812 1.3 */
const int  MESSAGE_MAX_PARAMS      = 15;  /* RFC2812 2.3 */
const char PREFIX_PREFIX           = ':'; /* RFC2812 2.3 */
const int  MESSAGE_MAX_LENGTH      = 152; /* RFC2812 2.3 */
const char MESSAGE_SEPARATOR       = ' '; /* RFC2812 2.3 */
const std::string END_OF_MESSAGE("\r\n"); /* RFC2812 2.3 */
const std::string LEAVE_ALL_CHANNELS_CHANNEL("0"); /* RFC2812 3.2.1 */
#endif
