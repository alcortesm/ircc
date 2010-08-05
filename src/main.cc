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
#include "ircc.h"

using std::string;
using std::endl;
using std::cerr;
using std::cout;

DebugStream debug(cerr, DEF_DEBUG);

void usage(void);
void digest_args(int argc, char ** argv);
void initialize(const Url & rWebCache) throw (std::runtime_error);
void tests(void);
void main_loop();
void process_line(const string & rLine);

int
main(int argc, char ** argv)
{
   digest_args(argc, argv);

   if (debug.on())
      tests();

   main_loop();

   return 0;
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
   debug << endl << endl << "****** Running unit tests!" <<  endl;
   debug << endl ;
   Url::Test();
   debug << endl ;
   debug << "****** All tests passed!" <<  endl << endl << endl;
}

/* when there is a line waiting to be read at stdin, the main loop calls
 * this function. The function must read one full line from stdin and
 * nothing more (the next lines will be read by subsequent invocations to
 * this same function).
 *
 * It exits with error messages if an error is found reading the line.
 *
 * It returns the empty string in case an end of file is found on stdin.
 *
 * If the line is longer than MAX_LINE_LEN, then the whole line is read,
 * forgotten and a string with a description of this error is thrown.
 *
 * If the line is OK, it is returned as a string. 
 */
class LineTooLongException : public std::runtime_error {
public:
   LineTooLongException() : std::runtime_error("Line too long") { }
};

string
fetch_line(void) throw (LineTooLongException)
{
   size_t total_bytes_read = 0;

   const size_t buf_sz = MAX_LINE_LEN;
   char* const buf = (char* const) xmalloc(buf_sz);

   ssize_t bytes_read;
   size_t remaining_buf_sz;
   for (;;) {
      remaining_buf_sz = buf_sz - total_bytes_read;
      bytes_read = read(STDIN_FD, (void*)(buf+total_bytes_read),
                        remaining_buf_sz);
      //      debug << "fetch_line bytes_read = " << bytes_read << endl ;
      if (bytes_read == -1 && errno == EINTR)
         continue;
      if (bytes_read == -1)
         fatal("read", NULL);
      if (bytes_read == 0) { /* EOF found */
         free(buf);
         return string();
      }
      total_bytes_read += bytes_read ;
      if (total_bytes_read == buf_sz
          && buf[buf_sz-1] != '\n') { /* line too long */
         /* fetch all the line, while forgetting it and throw a warning */
         for (;;) {
            bytes_read = read(STDIN_FD, (void*)buf, buf_sz);
            if (bytes_read == -1 && errno == EINTR)
               continue;
            if (bytes_read == -1)
               fatal("read", NULL);
            if (buf[bytes_read-1] == '\n') /* end of line detected */
               break;
         }
         free(buf);
         throw LineTooLongException();
      }
      /* if last char is a \n, we had fetch all the line */
      if (buf[total_bytes_read-1] == '\n') {
         string str(buf, total_bytes_read);
         free(buf);
         return str;
      }
      /* else continue fetching */
   }
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
      //      debug << "main_loop's select() returns " << retval << endl ;

      /* select error: exit */
      if (retval == -1) {
         if (errno == EINTR)
            continue;
         fatal("select", NULL);
      }

      /* select's timeout expire: reset timer */
      if (retval == 0) {
         //         debug << "main_loop(): timeout expire!" << endl ;
         tv.tv_sec = MAIN_LOOP_TIMEOUT_SECS;
         tv.tv_usec = MAIN_LOOP_TIMEOUT_SECS;
         continue;
      }

      /* some fd was ready: which one?, read and process data */
      if (FD_ISSET(STDIN_FD, &read_fds)) { /* stdin: user input */
         string line;
         try {
            line = fetch_line();
         } catch(LineTooLongException& e) {
            cout << e.what() << endl ;
            continue;
         }
         /* process line */
         /* if EOF found (empty string), exit the program */
         if (line.size() == 0) {
            cout << "bye!" << endl ;
            break;
         }
         process_line(line);
      }
   }
   return;
}

void
process_line(const string & rLine)
{
   debug << "process_line argument: " << rLine << endl ;

   if (rLine.empty())
      return;

   if (rLine.substr(1,4) == "/nop")
      return;
   if (rLine.substr(1,4) == "/nop ")
      return;

   return;
}
