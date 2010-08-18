#include "Args.h"

#include <cstring>
#include <iostream>
extern std::ostream* gpDebug;

bool
Args::IsTest() const {
   return mTest;
}

bool
Args::IsDebug() const {
   return mDebug;
}

Args::Args(bool test, bool debug)
   : mTest(test), mDebug(debug) {}

Args*
Args::New(int argc, char ** argv) throw (Args::InvalidArgumentException)
{
   bool test = false;
   bool debug = false;

   for (int i=1; i<argc; i++) {
      if (argv == NULL)
         throw Args::InvalidArgumentException("Null argument pointer");
      
      *gpDebug << argv[i] << std::endl ;
   
      if (strcmp("-t", argv[i]) == 0) {
         test = true;
      } else if (strcmp("-d", argv[i]) == 0) {
         debug = true;
      } else {
         throw Args::InvalidArgumentException();
      }
   }
   return new Args(test, debug);
}
