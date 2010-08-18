#include "Args.h"

#include <cstring>
#include "utils.h"

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

Args::InvalidArgumentException::InvalidArgumentException()
   : std::runtime_error("Invalid argument")
{}

Args::InvalidArgumentException::InvalidArgumentException(std::string const & s)
   : std::runtime_error("Invalid argument : " + s)
{}

Args*
Args::Build(int argc, char ** argv) throw (Args::InvalidArgumentException)
{
   bool test = false;
   bool debug = false;

   for (int i=1; i<argc; i++) {

      if (argv == NULL)
         throw Args::InvalidArgumentException("Args::Build() : argv is NULL");

      if (argv[i] == NULL)
         throw Args::InvalidArgumentException("Args::Build() : argv[" + stringify(i) + "] is NULL");
      
      *gpDebug << "Args::Build() : argv[" + stringify(i) + "] = " + argv[i] << std::endl ;
   
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
