#ifndef IRC_H
#define IRC_H

#include <string>

// protocol constants
const int SERVERNAME_MAX_LENGTH   = 63;  /* RFC2812 1.1 */
const int NICKNAME_MAX_LENGTH     = 9;   /* RFC2812 1.2.1, longer
                                           nicknames are not supported
                                           as no future evolutions of
                                           the protocol are supported by
                                           this software */
const int  CHANNELNAME_MAX_LENGTH  = 50;   /* RFC2812 1.3 */
const int  MESSAGE_MAX_PARAMS      = 15;   /* RFC2812 2.3 */
const char PREFIX_PREFIX           = ':';  /* RFC2812 2.3 */
const std::string COLON_PARAM_SEPARATOR = " :"; /* RFC2812 2.3 */
const int  MESSAGE_MAX_LENGTH      = 152;  /* RFC2812 2.3 */
const char MESSAGE_SEPARATOR       = ' ';  /* RFC2812 2.3 */
const std::string END_OF_MESSAGE("\r\n");  /* RFC2812 2.3 */
const std::string LEAVE_ALL_CHANNELS_CHANNEL("0"); /* RFC2812 3.2.1 */

const std::string COM_NICK      = "NICK";    /* RFC2812 3.1.2 */
const std::string COM_USER      = "USER";    /* RFC2812 3.1.3 */
const std::string COM_JOIN      = "JOIN";    /* RFC2812 3.2.1 */
const std::string COM_PRIVMSG   = "PRIVMSG"; /* RFC2812 3.3.1 */
const std::string COM_LIST      = "LIST";    /* RFC2812 3.2.6 */
const std::string COM_WHO       = "WHO";     /* RFC2812 3.6.1 */
const std::string COM_PART      = "PART";    /* RFC2812 3.2.2 */
const std::string COM_PING      = "PING";    /* RFC2812 3.7.2 */
const std::string COM_PONG      = "PONG";    /* RFC2812 3.7.3 */

/* RFC2812 5.1 Command responses */
const std::string RPL_WELCOME           = "001";
const std::string RPL_YOURHOST          = "002";
const std::string RPL_CREATED           = "003";
const std::string RPL_MYINFO            = "004";
// 250 is unknown to RFC2812
const std::string RPL_LUSERCLIENT       = "251";
const std::string RPL_LUSERCHANNELS     = "254";
const std::string RPL_LUSERME           = "255";
// 265 is unknown to RFC2812
// 266 is unknown to RFC2812
const std::string RPL_ENDOFWHO          = "315";
const std::string RPL_LIST              = "322";
const std::string RPL_LISTEND           = "323";
const std::string RPL_TOPIC             = "332";
const std::string RPL_WHOREPLY          = "352";
const std::string RPL_MOTD              = "372";
// 375 is unknown to RFC2812
// 333 is unknown to RFC2812
const std::string RPL_NAMREPLY          = "353";
const std::string RPL_ENDOFNAMES        = "366";
const std::string RPL_ENDOFMOTD         = "376";
const std::string ERR_NONICKNAMEGIVEN   = "431";
const std::string ERR_ERRONEUSNICKNAME  = "432";
const std::string ERR_NICKNAMEINUSE     = "433";
const std::string ERR_NICKCOLLISION     = "436";
const std::string ERR_UNAVAILRESOURCE   = "437";
const std::string ERR_RESTRICTED        = "484";
const std::string ERR_NEEDMOREPARAMS    = "461";
const std::string ERR_ALREADYREGISTRED  = "462";
#endif
