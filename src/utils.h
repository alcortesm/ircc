// -*- Mode: c++ -*-
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stdexcept>
#include <stdint.h>

/* prevent compiler warnings about unused variables */
#undef UNUSED
#define UNUSED(x) ((void)(x))
#undef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))

// string to int conversions
int         string_to_int(std::string & s) throw (std::invalid_argument);
std::string uint16_to_string(uint16_t i);
uint16_t    int_to_uint16(int i) throw (std::invalid_argument);

// c string handling
void to_lower(char * s);

// error throwing
void throw_fname_errno(const char * fname) throw (std::runtime_error);
void throw_fname(const char * fname, const char * reason) throw (std::runtime_error);

// string handling
bool begins_with(std::string a, std::string b);

// memory allocation
char * xstrdup(char const * const);
void * xmalloc(size_t size);
void * xcalloc(size_t n, size_t size);

// perror + exit
void   fatal(const char* const funcName, char *fmt, ...);
void   fatalne(const char* const funcName, char *fmt, ...);

#endif // UTILS_H
