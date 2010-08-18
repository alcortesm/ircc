#include "Args.h"

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

#include <iostream>
extern std::ostream* gpDebug;

using std::endl;

void
Args::Test()
{
   *gpDebug << "*** Begin of Args test" << endl ;
   {
      bool test = false;
      bool debug = false;
      Args a(test, debug);
      assert(a.IsTest() == test);
      assert(a.IsDebug() == debug);
   }
   {
      bool test = false;
      bool debug = true;
      Args a(test, debug);
      assert(a.IsTest() == test);
      assert(a.IsDebug() == debug);
   }
   {
      bool test = true;
      bool debug = false;
      Args a(test, debug);
      assert(a.IsTest() == test);
      assert(a.IsDebug() == debug);
   }
   {
      bool test = true;
      bool debug = true;
      Args a(test, debug);
      assert(a.IsTest() == test);
      assert(a.IsDebug() == debug);
   }
   {
      int argc=0;
      char ** argv=NULL;
      Args* p_a = Args::Build(argc, argv);
      assert(p_a->IsTest() == false);
      assert(p_a->IsDebug() == false);
      delete(p_a);
   }
   {
      int argc=1;
      char ** argv=NULL;
      Args* p_a = Args::Build(argc, argv);
      assert(p_a->IsTest() == false);
      assert(p_a->IsDebug() == false);      
      delete(p_a);
   }
   {
      Args* p_a = NULL;
      try {
         int argc=2;
         char ** argv=NULL;
         p_a = Args::Build(argc, argv);
      } catch (Args::InvalidArgumentException & e) {
         // OK
         goto ok3;
      } catch (std::exception & e) {
         assert((bool)"throw bad exception" & false);
      }
      // BAD
      assert((bool)"failed to catch exception with (2, NULL)" & false);
   ok3:;
      delete(p_a);
   }
   { // test 4
      int argc=2;
      char* argv0 = strdup("command");
      if (!argv0)
         throw std::runtime_error("test 4 : Out of memmory!");
      char* argv1 = strdup("-t");
      if (!argv1)
         throw std::runtime_error("test 4 : Out of memmory!");
      char* argv[2] = {argv0, argv1};
      Args* p_a;
      try {
         assert(strcmp(argv[0], "command") == 0);
         assert(strcmp(argv[1], "-t") == 0);
         p_a = Args::Build(argc, argv);
         free(argv0);
         free(argv1);
         assert(p_a->IsTest() == true);
         assert(p_a->IsDebug() == false);      
      } catch (Args::InvalidArgumentException & e) {
         throw std::runtime_error(std::string("test 4 : throw InvalidArgumentExceptionException: ").append(e.what()));
      } catch (std::exception & e) {
         throw std::runtime_error("test 4 : throw bad exception");
      }
      delete(p_a);
   }
   { // test 5
      int argc=2;
      char* argv0 = strdup("command");
      if (!argv0)
         throw std::runtime_error("test 5 : Out of memmory!");
      char* argv1 = strdup("-d");
      if (!argv1)
         throw std::runtime_error("test 5 : Out of memmory!");
      char* argv[2] = {argv0, argv1};
      Args* p_a;
      try {
         p_a = Args::Build(argc, argv);
         free(argv0);
         free(argv1);
         assert(p_a->IsTest() == false);
         assert(p_a->IsDebug() == true);      
      } catch (Args::InvalidArgumentException & e) {
         throw std::runtime_error(std::string("test 5 : throw InvalidArgumentExceptionException: ").append(e.what()));
      } catch (std::exception & e) {
         throw std::runtime_error("test 5 : throw bad exception");
      }
      delete(p_a);
   }
   { // test 6
      const int argc=3;
      char* argv0 = strdup("command");
      if (!argv0)
         throw std::runtime_error("test 6 : Out of memmory!");
      char* argv1 = strdup("-d");
      if (!argv1)
         throw std::runtime_error("test 6 : Out of memmory!");
      char* argv2 = strdup("-t");
      if (!argv2)
         throw std::runtime_error("test 6 : Out of memmory!");
      char* argv[argc] = {argv0, argv1, argv2};
      Args* p_a;
      try {
         p_a = Args::Build(argc, argv);
         free(argv0);
         free(argv1);
         free(argv2);
         assert(p_a->IsTest() == true);
         assert(p_a->IsDebug() == true);      
      } catch (Args::InvalidArgumentException & e) {
         throw std::runtime_error(std::string("test 6 : throw InvalidArgumentExceptionException: ").append(e.what()));
      } catch (std::exception & e) {
         throw std::runtime_error("test 6 : throw bad exception");
      }
      delete(p_a);
   }
   { // test 7
      const int argc=3;
      char* argv0 = strdup("command");
      if (!argv0)
         throw std::runtime_error("test 7 : Out of memmory!");
      char* argv1 = strdup("-d");
      if (!argv1)
         throw std::runtime_error("test 7 : Out of memmory!");
      char* argv2 = strdup("-t");
      if (!argv2)
         throw std::runtime_error("test 7 : Out of memmory!");
      char* argv[argc] = {argv0, argv2, argv1};
      Args* p_a;
      try {
         p_a = Args::Build(argc, argv);
         free(argv0);
         free(argv1);
         free(argv2);
         assert(p_a->IsTest() == true);
         assert(p_a->IsDebug() == true);      
      } catch (Args::InvalidArgumentException & e) {
         throw std::runtime_error(std::string("test 6 : throw InvalidArgumentExceptionException: ").append(e.what()));
      } catch (std::exception & e) {
         throw std::runtime_error("test 7 : throw bad exception");
      }
      delete(p_a);
   }
   { // test 8
      const int argc=1;
      char* argv0 = strdup("command");
      if (!argv0)
         throw std::runtime_error("test 8 : Out of memmory!");
      char* argv[argc] = {argv0};
      Args* p_a;
      try {
         p_a = Args::Build(argc, argv);
         free(argv0);
         assert(p_a->IsTest() == false);
         assert(p_a->IsDebug() == false);      
      } catch (Args::InvalidArgumentException & e) {
         throw std::runtime_error(std::string("test 8 : throw InvalidArgumentExceptionException: ").append(e.what()));
      } catch (std::exception & e) {
         throw std::runtime_error("test 8 : throw bad exception");
      }
      delete(p_a);
   }
   *gpDebug << "*** End of Args test" << endl ;
   return;
}
