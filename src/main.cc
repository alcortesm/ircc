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
#include <sys/select.h>

#include "Url.h"
#include "utils.h"
#include "DebugStream.h"
#include "prompt.h"
#include "ircc.h"

using std::string;
using std::endl;
using std::cerr;
using std::cout;

DebugStream debug(cerr, DEF_DEBUG);

void usage(void);
void digest_args(int argc, char ** argv);
void inf_loop(void);
void initialize(const Url & rWebCache) throw (std::runtime_error);
void tests(void);
void main_loop();

int
main(int argc, char ** argv)
{
   digest_args(argc, argv);

   if (debug.on())
      tests();

   main_loop();

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

void
main_loop(void)
{
   /* wakeup timer: there is no need to use one at all */
   struct timeval tv;
   tv.tv_sec = MAIN_LOOP_TIMEOUT_SECS;
   tv.tv_usec = MAIN_LOOP_TIMEOUT_USECS;

   for(;;) {
      int max_fds = 0;

      /* read from stdin */
      fd_set read_fds;
      FD_ZERO(&read_fds);
      FD_SET(STDIN_FD, &read_fds);
      max_fds = MAX(max_fds, STDIN_FD);

      int retval;
      retval = select(max_fds+1, &read_fds, NULL, NULL, &tv);
      debug << "main_loop's select() returns " << retval << endl ;

      /* select error: exit */
      if (retval == -1) {
         if (errno == EINTR)
            continue;
         fatal("select", NULL);
      }

      /* select's timeout expire: reset timer */
      if (retval == 0) {
         debug << "main_loop(): timeout expire!" << endl ;
         tv.tv_sec = MAIN_LOOP_TIMEOUT_SECS;
         tv.tv_usec = MAIN_LOOP_TIMEOUT_SECS;
         continue;
      }

      /* some fd was ready: which one?, run response */
      if (FD_ISSET(STDIN_FD, &read_fds)) {
         const size_t buf_sz = 1024;
         char* const buf = (char* const) xcalloc(buf_sz, sizeof(char));
         ssize_t nr;
         nr = read(STDIN_FD, (void*)buf, buf_sz);
         if (nr == -1)
            fatal("read", NULL);
         if (nr == 0) {
            free(buf);
            break;
         }
         if (strcmp(buf, "quit\n") == 0) {
            free(buf);
            break;
         }
         fprintf(stdout, "%s", buf);
         free(buf);
      }
   }
   return;
}
