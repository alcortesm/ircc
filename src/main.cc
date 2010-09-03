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
#include "NullStream.h"
#include "ircc.h"
#include "ComNop.h"
#include "ComUnknown.h"
#include "CommandFactory.h"
#include "Args.h"
#include "ComQuit.h"
#include "Server.h"

using std::string;
using std::endl;
using std::cerr;
using std::cout;

std::ostream* gpDebug = (std::ostream*) &NullStream::cnull;

void usage(void);
void tests(void);
void main_loop();
bool process_line_is_quit(const string & rLine);

int
main(int argc, char ** argv)
{

   try {

      Args* p_args;
      p_args = Args::Build(argc, argv);

      if (p_args->IsDebug())
         gpDebug = (std::ostream*) &cout;

      if (p_args->IsTest())
         tests();
      else
         main_loop();

      delete(p_args);

      return 0;

   } catch (Args::InvalidArgumentException & e) {
      usage();
   }

}

void
usage(void)
{
   std::cerr << USAGE << std::endl ;
   exit(1);
}

void
tests(void)
{
   cout << "*** Running unit tests!" <<  endl ;
   Url::Test();
   Args::Test();
   Server::Test();
   cout << "*** All tests passed!" <<  endl ;
}

/* when there is a line waiting to be read at stdin, the main loop calls
 * this function. The function must read one full line from stdin and
 * nothing more (the next lines will be read by subsequent invocations to
 * this same function). The returned string does not have the ending '\n'.
 *
 * It exits with error messages if an error is found reading the line.
 *
 * It returns EofException if the end of file is found on stdin.
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
class EofException : public std::runtime_error {
public:
   EofException() : std::runtime_error("EOF found!") { }
};

string
fetch_line(void) throw (LineTooLongException, EofException)
{
   size_t total_bytes_read = 0;

   const size_t buf_sz = MAX_LINE_LEN + 1; /* line + '\n' */
   char* const buf = (char* const) xmalloc(buf_sz);

   ssize_t bytes_read;
   size_t remaining_buf_sz;
   for (;;) {
      remaining_buf_sz = buf_sz - total_bytes_read;
      bytes_read = read(STDIN_FD, (void*)(buf+total_bytes_read),
                        remaining_buf_sz);
      //      *gpDebug << "fetch_line bytes_read = " << bytes_read << endl ;
      if (bytes_read == -1 && errno == EINTR)
         continue;
      if (bytes_read == -1)
         fatal("read", NULL);
      if (bytes_read == 0) { /* EOF found */
         free(buf);
         throw EofException();
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
         string str(buf, total_bytes_read-1); /* remove ending '\n' */
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
      //      *gpDebug << "main_loop's select() returns " << retval << endl ;

      /* select error: exit */
      if (retval == -1) {
         if (errno == EINTR)
            continue;
         fatal("select", NULL);
      }

      /* select's timeout expire: reset timer */
      if (retval == 0) {
         //         *gpDebug << "main_loop(): timeout expire!" << endl ;
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
         } catch(EofException& e) {
            line = ComQuit::STR; /* eof = /quit */
         }

         /* process line: build a command from the line and execute it */
         try {
            Command* p_command;
            p_command = CommandFactory::Build(line);
            p_command->Run();
            if (p_command->MustQuit()) {
               delete p_command;
               cout << "bye!" << endl ;
               break;
            }
            delete p_command;
         } catch (CommandFactory::BadSyntaxException e) {
            cout << e.what() << endl ;
         }
      }
   }
   return;
}
