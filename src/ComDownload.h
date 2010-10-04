#ifndef COMDOWNLOAD_H
#define COMDOWNLOAD_H

#include "Command.h"

class ComDownload : public Command {
 public:
   ComDownload(const std::string& host, const std::string& port, const std::string& fileName);
   void Run();
   static const std::string STR;
 private:
   const std::string mHost;
   const std::string mPort;
   const std::string mFileName;
};

#endif /* COMDOWNLOAD_H */
