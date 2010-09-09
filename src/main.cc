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
#include "com_factory.h"
#include "msg_factory.h"
#include "Args.h"
#include "ComQuit.h"
#include "Server.h"
#include "irc.h"
#include "Msg.h"
#include "MsgTellUser.h"

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
   cout << FROM_PROGRAM << " Running unit tests!" <<  endl ;

   Args::Test();

   if (Server::TestFail()) {
      cerr << FROM_PROGRAM << "Server test failed!" << endl ;
      return;
   }

   cout << FROM_PROGRAM << "All tests passed!" <<  endl ;
}


class InputErrorException : public std::runtime_error {
public:
   InputErrorException() : std::runtime_error("Input error") { }
};
class EofException : public std::runtime_error {
public:
   EofException() : std::runtime_error("EOF found!") { }
};

string
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
   return line;
}

// Takes the received data and go line by line, building a Msg for each
// line an calling Msg::Run() of all of them in order.
//
// As the previous chunk of data could have left an incomplete line, it
// also receive the previous incomplete line and returns the current one
// if any.
string
process_data_from_server(const string& data, const string& old_data)
{
   // whole = current received data preceded by prevous pending incomplete line
   string whole(old_data);
   whole.append(data);

   size_t last_line_pos = 0;
   size_t next_line_pos;
   string pending(""); // the last unended line if any
   while(true) {
      next_line_pos = whole.find(END_OF_MESSAGE, last_line_pos);
      if (next_line_pos == string::npos) {
         pending = string(whole, last_line_pos);
         break;
      }
      string line(whole, last_line_pos, next_line_pos-last_line_pos);
      Msg* p_msg = msg_factory(line);
      p_msg->Run();
      delete p_msg;
      last_line_pos = next_line_pos + END_OF_MESSAGE.length();
   }

   return pending;
}

void
main_loop()
{
   // a server will be needed during the main loop
   Server server;
   // we need to remember the still unprocessed data from the server
   std::string old_data;

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
         try {
            string data = server.Recv();
            old_data = process_data_from_server(data, old_data);
         } catch (Server::NotConnectedException& e) {
            // that's OK, keep looping
         } catch (Server::RecvException& e) {
            cout << FROM_PROGRAM << "error receiving data from server: " << e.what() << endl;
         }
      }

      if (FD_ISSET(STDIN_FD, &read_fds)) {
         /* process line: build a command from the line and execute it */
         try {
            string line = fetch_line();
            if (isatty(1))
               cout << line << endl;
            Command* p_command;
            p_command = com_factory(line, server);
            p_command->Run();
            if (p_command->MustQuit()) {
               delete p_command;
               throw EofException();
            }
            delete p_command;
         } catch (InputErrorException& e) {
            cout << e.what() << endl ;
         } catch (EofException& e) {
            cout << FROM_PROGRAM << "bye!" << endl ;
            break;
         }
      }

   }
   return;
}
