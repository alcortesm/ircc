#ifndef IRCC_H
#define IRCC_H

#include <string>

// protocol constants
const int SERVERNAME_MAX_LENGTH   = 63; /* RFC2812 1.1 */
const int NICKNAME_MAX_LENGTH     = 9;  /* RFC2812 1.2.1, longer
                                           nicknames are not supported
                                           as no future evolutions of
                                           the protocol are supported by
                                           this software */
const int CHANNELNAME_MAX_LENGTH  = 50; /* RFC2812 1.3 */

// mix constants
const bool DEF_DEBUG = false;

#endif
