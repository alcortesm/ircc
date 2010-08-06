#ifndef FACTORY_H
#define FACTORY_H

#include "Command.h"
#include <stdexcept>

class Factory
{
 public:
   class BadSyntaxException : public std::runtime_error {
   public:
   BadSyntaxException() : std::runtime_error("*** bad syntax") { }
   BadSyntaxException(std::string s) : std::runtime_error("*** bad syntax: " + s) { }
   };

   static Command* NewCommand(const std::string& line) throw (std::runtime_error);
};

#endif /* FACTORY_H */
