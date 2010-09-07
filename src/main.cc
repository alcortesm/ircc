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

void usage();
void tests();
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
usage()
{
   std::cerr << USAGE << std::endl ;
   exit(1);
}

void
tests()
{
   cout << "*** Running unit tests!" <<  endl ;

   Args::Test();

   if (Server::TestFail()) {
      cerr << "*** Server test failed!" << endl ;
      return;
   }

   cout << "*** All tests passed!" <<  endl ;
}


class InputErrorException : public std::runtime_error {
public:
   InputErrorException() : std::runtime_error("Input error") { }
};
class EofException : public std::runtime_error {
public:
   EofException() : std::runtime_error("EOF found!") { }
};

string*
fetch_line() throw (EofException, InputErrorException)
{
   string line;
   getline(std::cin, line);
   if (std::cin.eof())
      throw EofException();
   if (std::cin.fail())
      throw InputErrorException();
   if (std::cin.bad())
      throw InputErrorException();
   return new string(line);
}

void
main_loop()
{
   // a server will be needed during the main loop
   Server server;

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
      if (server.IsConnected()) {
         FD_SET(server.GetSock(), &read_fds);
         max_fds = MAX(max_fds, server.GetSock());
      }

      int retval;
      retval = select(max_fds+1, &read_fds, NULL, NULL, &tv);
      //*gpDebug << "main_loop's select() returns " << retval << endl ;
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
      if (FD_ISSET(server.GetSock(), &read_fds)) { /* server socket */
         *gpDebug << "server socket is ready to be read" << endl;

         try {
            string data = server.Recv();
            cout << "*** data from server: \"" << data << "\"" << endl;
         } catch (Server::NotConnectedException& e) {
            // that's OK, keep looping
         } catch (Server::RecvException& e) {
            cout << "*** error receiving data from server: " << e.what() << endl;
         }
      }

      if (FD_ISSET(STDIN_FD, &read_fds)) {
         /* process line: build a command from the line and execute it */
         try {
            string line(*fetch_line());
            Command* p_command;
            p_command = CommandFactory::Build(line, server);
            p_command->Run();
            if (p_command->MustQuit()) {
               delete p_command;
               throw EofException();
            }
            delete p_command;
         } catch (InputErrorException& e) {
            cout << e.what() << endl ;
         } catch (EofException& e) {
            cout << "bye!" << endl ;
            break;
         } catch (CommandFactory::BadSyntaxException e) {
            cout << e.what() << endl ;
         }
      }

   }
   return;
}
