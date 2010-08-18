#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include "Command.h"
#include <stdexcept>

class CommandFactory
{
 public:
   class BadSyntaxException : public std::runtime_error {
   public:
   BadSyntaxException() : std::runtime_error("*** bad syntax") { }
   BadSyntaxException(std::string s) : std::runtime_error("*** bad syntax: " + s) { }
   };

   static Command* Build(const std::string& line) throw (std::runtime_error);
};

#endif /* COMMANDFACTORY_H */
