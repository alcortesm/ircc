#ifndef ARGS_H
#define ARGS_H

#include <stdexcept>

class Args
{
 protected:
   bool mTest;
   bool mDebug;
   Args(bool test, bool debug);
 public:
   bool IsTest() const ;
   bool IsDebug() const ;
   class InvalidArgumentException : public std::runtime_error {
   public:
      InvalidArgumentException();
      InvalidArgumentException(std::string const & s);
   };
   static Args* Build(int argc, char ** argv) throw (Args::InvalidArgumentException);
   static void Test();
};
#endif /* ARGS_H */
