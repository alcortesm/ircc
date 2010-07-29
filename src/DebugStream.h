// -*- Mode: c++ -*-

// Stream to use for debugging
//
// It is thread safe, but funny things can happend
// with on().
//
// Example of use:
// #include "DebugStream.h"
// DebugStream debug(std::cout);
// int main() {
//   debug << "Debug" << std::endl;
//   debug.on() = false;
//   debug << "This line will not print" << std::endl;
//   return 0;
// }

#ifndef DEBUGSTREAM_H
#define DEBUGSTREAM_H

#include <ostream> // HASA-USES ostream for writting data

class DebugStream {
private:
   std::ostream & mrStream;
   bool           mIsOn;
    
public:
   DebugStream(std::ostream & rStream, bool isOn = true)
      : mrStream(rStream), mIsOn(isOn) { };
   bool on() const { return mIsOn; };
   bool& on() { return mIsOn; };
   template <typename T> inline DebugStream & operator<<(const T & t) {
      if (mIsOn)
         mrStream << t;
      return *this;
   }

   inline DebugStream & operator<<(std::ostream& (*f)(std::ostream &)) {
      if (mIsOn)
         mrStream << f;
      return *this;
   }
};

#endif // DEBUGSTREAM_H
