#include "ComSleep.h"
#include <ostream>
#include <iostream>
#include <unistd.h>
#include "ircc.h"

extern std::ostream* gpDebug;

const std::string ComSleep::STR = std::string("/sleep");
const int ComSleep::DEFAULT_SLEEP_TIME_SECS = 1;

void
ComSleep::Run() {
   *gpDebug << FROM_DEBUG << "ComSleep::Run()" << std::endl;
   sleep(mSecs);
   return ;
}

ComSleep::ComSleep(int secs)
  : mSecs(secs)
{
   *gpDebug << FROM_DEBUG << "ComSleep::ComSleep(" << secs << ")" << std::endl;
}
