#include "utils.h"

#include <sstream>
#include <cerrno>
#include <cstring>
#include <cstdio>

#include "stdlib.h"

int
string_to_int(std::string & s) throw (std::invalid_argument) {
    int i;
    char c;
    std::istringstream ss(s);
    if (! (ss >> i) || ss.get(c))
       throw std::invalid_argument(s + " is not a number");
    return i;
}

std::string
uint16_to_string(uint16_t i)
{
   std::ostringstream oss;
   oss<< i;
   return oss.str();
}

uint16_t
int_to_uint16(int i) throw (std::invalid_argument) {
   if (i < 0)
      throw std::invalid_argument("less than 0");
   if (i > 0xFFFF)
      throw std::invalid_argument("greater than 0xFFFF");
   return (uint16_t) i;
}

void
to_lower(char * s) {
   char * cp;
   for (cp=s; *cp; cp++)
      *cp = tolower(*cp);
}

void
throw_fname_errno(const char * fname)
   throw (std::runtime_error)
{
   throw std::runtime_error(std::string(fname) + "(): " + strerror(errno));
}

void
throw_fname(const char * fname, const char * reason)
   throw (std::runtime_error)
{
   throw std::runtime_error(std::string(fname) + "(): " + reason);
}

bool
begins_with(std::string a, std::string b)
{
   if (a.compare(0, b.size(), b) == 0)
      return true;
   return false;
}

char *
xstrdup(char const * const str)
{
   char * result;
   result = strdup(str);
   if (!result) {
      perror("strdup");
      exit(EXIT_FAILURE);
   }
   return result;
}

void *
xmalloc(size_t size)
{
   void * result;
   result = malloc(size);
   if (!result) {
      perror("malloc");
      exit(EXIT_FAILURE);
   }
   return result;
}

void *
xcalloc(size_t n, size_t size)
{
   void * result;
   result = calloc(n, size);
   if (!result) {
      perror("calloc");
      exit(EXIT_FAILURE);
   }
   return result;
}
