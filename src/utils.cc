#include "utils.h"

#include <sstream>
#include <cerrno>
#include <cstring>
#include <cstdio>

#include <stdarg.h>

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
   if (!result)
      fatal("strdup", NULL);
   return result;
}

void *
xmalloc(size_t size)
{
   void * result;
   result = malloc(size);
   if (!result)
      fatal("malloc", NULL);
   return result;
}

void *
xcalloc(size_t n, size_t size)
{
   void * result;
   result = calloc(n, size);
   if (!result)
      fatal("calloc", NULL);
   return result;
}

/*
 * Prints the string supplied, the string from errno,
 * any other argument.
 *
 * Use it if errno is meaningful, if not, use fatalne()
 *
 * Example of use:
 *
 * void *value = malloc (size);
 * if (!value)
 *     fatal("malloc", "Creating node %d", node_number);
 *
 * "malloc(): Out of memmory: Creating node 17"
 */
void
fatal(const char* const funcName, char *fmt, ...)
{
   va_list val;

   char *prev_errstr;
   prev_errstr = strdup(strerror(errno));
   if (!prev_errstr) {
      fprintf(stderr, "strdup: out of memory\n");
      fflush(NULL);
      exit(EXIT_FAILURE);
   }

   fprintf(stderr, "%s(): %s", funcName, prev_errstr);
   free(prev_errstr);

   if (fmt != NULL) {
      fprintf(stderr, ": ");
      va_start(val, fmt);
      vfprintf(stderr, fmt, val);
      va_end(val);
   }

   putc('\n', stderr);
   fflush(NULL);
   exit(EXIT_FAILURE);
}

/*
 * Prints the command name and the string supplied.
 * If errno is meaningful, use fatal instead.
 */
void
fatalne(const char* const funcName, char *fmt, ...)
{
   va_list val;

   fprintf(stderr, "%s", funcName);

   if (fmt != NULL) {
      fprintf(stderr, ": ");
      va_start(val, fmt);
      vfprintf(stderr, fmt, val);
      va_end(val);
   }

   putc('\n', stderr);
   fflush(NULL);
   exit(EXIT_FAILURE);
}
