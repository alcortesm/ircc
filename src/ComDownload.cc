#include "ComDownload.h"
#include <ostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <stdexcept>
#include "ircc.h"
#include "irc.h"
#include "ctcp.h"
#include "utils.h"
#include <sstream>
#include <iostream>

extern std::ostream* gpDebug;

using std::cout;
using std::endl;

/* static */
const std::string ComDownload::STR = std::string("/download");

void
ComDownload::Run() {

   *gpDebug << FROM_DEBUG << "ComDownload::Run("
            << "host = \"" << mHost << "\", "
            << "port = \"" << mPort << "\", "
            << "file_name = \"" << mFileName << "\")"
            << std::endl ;

   return ;
}

ComDownload::ComDownload(const std::string& rHost
                         , const std::string& rPort
                         , const std::string& rFileName)
   : mHost(rHost)
   , mPort(rPort)
   , mFileName(rFileName)
{
   *gpDebug << FROM_DEBUG << "ComDownload::ComDownload("
            << "host = \"" << mHost << "\", "
            << "port = \"" << mPort << "\", "
            << "file_name = \"" << mFileName << "\")"
            << std::endl ;
}
