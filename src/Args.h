#ifndef ARGS_H
#define ARGS_H

#include <stdexcept>

class Args
{
 private:
   bool mTest;
   bool mDebug;
   Args(bool test, bool debug);
 public:
   bool IsTest() const ;
   bool IsDebug() const ;
   class InvalidArgumentException : public std::runtime_error {
   public:
   InvalidArgumentException() : std::runtime_error("Invalid argument") { }
   InvalidArgumentException(std::string s) : std::runtime_error("Invalid argument: " + s) { }
   };
   static Args* New(int argc, char ** argv) throw (Args::InvalidArgumentException);
   static void Test();
};
#endif /* ARGS_H */
