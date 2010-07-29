#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cassert>
#include <stdexcept>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <cstring>
#include <error.h>
#include <errno.h>

#include "Url.h"
#include "utils.h"
#include "DebugStream.h"
#include "prompt.h"
#include "ircc.h"

using std::string;
using std::endl;
using std::cerr;
using std::cout;

DebugStream debug(cout, DEF_DEBUG);

void usage(void);
void digest_args(int argc, char ** argv);
void inf_loop(void);
void initialize(const Url & rWebCache) throw (std::runtime_error);
void tests(void);

int
main(int argc, char ** argv)
{
   digest_args(argc, argv);

   if (debug.on())
      tests();

   exit(EXIT_SUCCESS);

   /*
   try {
      Conf conf = Conf::FromFile();
      initialize(conf.WebCache());
   } catch (std::exception & e) {
      cerr << e.what() << endl;
      goto error;
   }

   launch_prompt_and_wait();

   exit(EXIT_SUCCESS);
 error:
   exit(EXIT_FAILURE);
   */
}

void
usage(void)
{
   cerr << "Usage:\n\tircc [-d]" << endl;
   exit(EXIT_FAILURE);
}

void
digest_args(int argc, char ** argv)
{
   if (argc > 2)
      usage();
   if (argc == 2) {
      if (string("-d") == argv[1]) {
         debug.on() = true;
      } else {
         usage();
      }
   }
}

void
inf_loop(void)
{
   const struct timespec onesec = {1, 0};
   int r;

   while (true) {
      r = nanosleep(&onesec, NULL);
      if (r == -1) {
         perror("nanosleep");
         exit(EXIT_FAILURE);
      }
   }
}

void
initialize(const Url & rWebCache) throw (std::runtime_error)
{
   debug << "** Initialization started" << endl;
   UNUSED(rWebCache);
   debug << "** Initialization finnished" << endl;
   return;
}

void
tests(void)
{
   debug << endl << endl << "Running unit tests!" <<  endl;
   Url::Test();
   //   test_prompt();
   debug << "All tests passed!" <<  endl << endl << endl;
}
