#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <cassert>
#include <limits>

#include "prompt.h"
#include "utils.h"
#include "DebugStream.h"
extern DebugStream debug;

const int LINESZ = 1024;
const char * COMMAND_FIND = "find ";
const char * COMMAND_GET = "get ";
const char * COMMAND_INFO = "info";
const char * COMMAND_INFO_SPC = "info ";
const char * COMMAND_STOP = "stop ";
const char * COMMAND_EXIT = "exit";
const char * COMMAND_SET = "set";
const char * COMMAND_SET_SPC = "set ";

const char * INFO_HOSTS     = "hosts";
const char * INFO_UPLOADS   = "uploads";
const char * INFO_DOWNLOADS = "downloads";

const char * SET_ROWEBCACHE   = "rowebcache";
const char * SET_LOCALPORT    = "localport";
const char * SET_SHAREPATH    = "sharepath";
const char * SET_INCOMINGPATH = "incomingpath";
const char * SET_DOWNLOADPATH = "downloadpath";

int
line_is_too_big(const char * line)
{
   if (strlen(line) == LINESZ -1)
      return 1;
   else
      return 0;
}

bool
a_is_space(char c)
{
   if (c == ' ')
      return true;
   else
      return false;
}

// line is a clean line;
// words are names or numbers
bool
is_word(const char * line)
{
   if (a_is_space(line[0]))
      return false;
   if (line[0] == '\0')
      return false;
   return true;
}

bool
there_are_more_words(const char * line)
{
   if (strchr(line, ' '))
      return true;
   return false;
}

bool
is_uint16(const char * pText)
{
   if (!pText)
      return false;

   const int DECIMAL_BASE = 10;
   long int value = 0;
   char *p_end = NULL;
   errno = 0;
   value = strtol(pText, &p_end, DECIMAL_BASE);

   if (errno == ERANGE) {
      return false; /* number out of range (bigger than long) */
   } else if (value > std::numeric_limits<uint16_t>::max()) {
      return false; /* number too large for a 16 bits unsigned integer */
   } else if (value < 0) {
      return false; /* negative number */
   } else if (p_end == pText) {
      return false; /* invalid numeric input */
   } else if (*p_end != '\0') {
      return false;/* invalid numeric input, it has extra characters at the end */
   }
   return true;
}

bool
is_only_one_word(const char * line)
{
   if (is_word(line))
      if (! there_are_more_words(line))
         return true;
   return false;
}

// check if string starts with an info surname
bool
is_info_surname(const char * line)
{
   if (memcmp(line, INFO_HOSTS, strlen(INFO_HOSTS)) == 0)
      return true;
   if (memcmp(line, INFO_UPLOADS, strlen(INFO_UPLOADS)) == 0)
      return true;
   if (memcmp(line, INFO_DOWNLOADS, strlen(INFO_DOWNLOADS)) == 0)
      return true;
   return false;
}

// check if string starts with a var name
bool
is_set_var(const char * line)
{
   if (memcmp(line, SET_ROWEBCACHE, strlen(SET_ROWEBCACHE)) == 0)
      return true;
   if (memcmp(line, SET_LOCALPORT, strlen(SET_LOCALPORT)) == 0)
      return true;
   if (memcmp(line, SET_SHAREPATH, strlen(SET_SHAREPATH)) == 0)
      return true;
   if (memcmp(line, SET_INCOMINGPATH, strlen(SET_INCOMINGPATH)) == 0)
      return true;
   if (memcmp(line, SET_DOWNLOADPATH, strlen(SET_DOWNLOADPATH)) == 0)
      return true;
   return false;
}

int
count_spaces(const char * line)
{
   int count = 0;
   for (size_t i = 0; line[i] != '\0' ; i++) {
      if (a_is_space(line[i]))
         count++;
   }
   return count;
}

// the line is clean
// commands are: find, get, info, stop, exit, set
bool
is_command(const char * line)
{
   // find something
   if (memcmp(line, COMMAND_FIND, strlen(COMMAND_FIND)) == 0) {
      char * second;
      second = (char *) line + strlen(COMMAND_FIND);
      if (is_only_one_word(second))
            return true;
   }

   // get somenumber
   if (memcmp(line, COMMAND_GET, strlen(COMMAND_GET)) == 0) {
      char * second;
      second = (char *) line + strlen(COMMAND_GET);
      if (is_only_one_word(second))
         if (is_uint16(second))
            return true;
   }

   // info
   if (strcmp(line, COMMAND_INFO) == 0)
      return true;

   // info something
   if (memcmp(line, COMMAND_INFO_SPC, strlen(COMMAND_INFO_SPC)) == 0) {
      char * second;
      second = (char *) line + strlen(COMMAND_INFO_SPC);
      if (is_only_one_word(second))
         if (is_info_surname(second))
            return true;
   }

   // info something something
   if (memcmp(line, COMMAND_INFO_SPC, strlen(COMMAND_INFO_SPC)) == 0) {
      if (count_spaces(line) == 2) {
         char * second;
         second = strchr((char *) line, ' ') + 1;
         char * third;
         third = strchr(second, ' ') + 1;
      
         // check if they are info surnames and if they are different
         if (is_info_surname(second) && is_info_surname(third))
            if (second[0] != third[0])
               return true;
      }
   }

   // info something something something
   if (memcmp(line, COMMAND_INFO_SPC, strlen(COMMAND_INFO_SPC)) == 0) {
      if (count_spaces(line) == 3) {
         char * second;
         second = strchr((char *)line, ' ') + 1;
         char * third;
         third = strchr(second, ' ') + 1;
         char * last;
         last = strchr(third, ' ') + 1;
      
         // check if they are info surnames and if they are different
         if (is_info_surname(second) && is_info_surname(third) && is_info_surname(last))
            if (second[0] != third[0])
               if (second[0] != last[0])
                  if (third[0] != last[0])
                     return true;
      }
   }

   // stop somenumber
   if (memcmp(line, COMMAND_STOP, strlen(COMMAND_STOP)) == 0) {
      char * second;
      second = (char *) line + strlen(COMMAND_STOP);
      if (is_only_one_word(second))
         if (is_uint16(second))
            return true;
   }

   // exit
   if (memcmp(line, COMMAND_EXIT, strlen(COMMAND_EXIT)+1) == 0)
      return true;

   // set
   if (strcmp(line, COMMAND_SET) == 0)
      return true;

   // set var
   if (memcmp(line, COMMAND_SET_SPC, strlen(COMMAND_SET_SPC)) == 0) {
      char * second;
      second = (char *) line + strlen(COMMAND_SET_SPC);
      if (is_only_one_word(second))
         if (is_set_var(second))
            return true;
   }

   // set var value
   if (count_spaces(line) == 2) {
      char * second;
      second = strchr((char *)line, ' ') + 1;
      char * third;
      third = strchr(second, ' ') + 1;
      
      // check if there is a set var and a valid value
      if (is_set_var(second))
         if (is_word(third))
            return true;
   }

   return false;
}

void
a_turn_tabs_into_spaces(const char * from, char * to)
{
   size_t len = strlen(from);

   for (size_t i=0; i<len; i++) {
      if (from[i] == '\t')
         to[i] = ' ';
      else
         to[i] = from[i];
   }
   to[len + 1] = '\0';
}

void
a_drop_repeated_spaces(const char * from, char * to)
{
   size_t len = strlen(from);
   bool prev_was_space = false;

   size_t j = 0;
   for (size_t i=0; i<len; i++) {
      if (prev_was_space && a_is_space(from[i]))
         continue;
      if (a_is_space(from[i]))
         prev_was_space = true;
      else
         prev_was_space = false;
      to[j++] = from[i];
   }
   to[j] = '\0';
}

void
a_drop_ending_eol(const char * from, char * to)
{
   size_t len = strlen(from);

   strcpy(to, from);
   if (to[len-1] == '\n')
      to[len-1] = '\0';
}


void
a_drop_space_at_start(const char * from, char * to)
{
   if (a_is_space(from[0]))
      strcpy(to, from+1);
   else
      strcpy(to, from);
}


void
a_drop_space_at_end(const char * from, char * to)
{
   size_t len = strlen(from);

   strcpy(to, from);
   if (to[len-1] == ' ')
      to[len-1] = '\0';
}

// copy line to clean_line droopping repeated spaces
// changes tabs into spaces and drop initial and ending spaces
// that's a clean line, by the way
void
clean_line(const char * line, char * clean)
{
   //   debug << "clean_line() gets:\n\t\"" << std::string(line) << "\"" << std::endl;

   char tmp[LINESZ];
   memset(tmp, '\0', LINESZ);
   strcpy(tmp, line);

   // turn tabs into spaces
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      a_turn_tabs_into_spaces(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after turn tabs into spaces:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   // drop repeated spaces
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      a_drop_repeated_spaces(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after dropping repeated spaces:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   // drop '\n' at the end
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      a_drop_ending_eol(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after dropping '\\n' at the end:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   // drop space at the start
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      a_drop_space_at_start(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after dropping space at start:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   // drop space at the end
   {
      char aux[LINESZ];
      memset(aux, '\0', LINESZ);
      a_drop_space_at_end(tmp, aux);
      strcpy(tmp, aux);
      //      debug << "clean_line() after dropping space at end:\n\t\"" << std::string(tmp) << "\"" << std::endl;
   }

   strcpy(clean, tmp);

   //   debug << "clean_line() puts \"" << std::string(clean) << "\"" << std::endl;

   return;
}

int
is_command_exit(const char * line)
{
   if (memcmp(line, COMMAND_EXIT, strlen(COMMAND_EXIT)) == 0)
      return 1;
   else
      return 0;
}

void
read_clean_line(char * clean)
{
   char line[LINESZ];
   memset(line, '\0', LINESZ);

   char * p;
   p = fgets(line, LINESZ, stdin);
   if (p == NULL) memcpy(line, COMMAND_EXIT, strlen(COMMAND_EXIT)+1); // EOF detected

   //   debug << "read \"" << std::string(line) << "\" from stdin" << std::endl;

   if (line_is_too_big(line)) {
      fprintf(stderr, "error: line too long on stdin!\n");
      exit(EXIT_FAILURE);
   }

   clean_line(line, clean);
   //   debug << "line cleaned to \"" << std::string(clean) << "\"" << std::endl;
}

void *
prompt_loop(void *)
{
   char line[LINESZ];

   while (1) {
      int r;
      r = fputs("> ", stdout);
      if (r == EOF) {
         fprintf(stderr, "fputs: unknown error\n");
         exit(EXIT_FAILURE);
      }

      read_clean_line(line);

      if (is_command(line)) {
         r = fputs("unrecogniced command\n", stdout);
         if (r == EOF) {
            fprintf(stderr, "fputs: unknown error\n");
            exit(EXIT_FAILURE);
         }
         continue;
      }

      if (is_command_exit(line)) {
         r = fputs("bye!\n", stdout);
         if (r == EOF) {
            fprintf(stderr, "fputs: unknown error\n");
            exit(EXIT_FAILURE);
         }
         return NULL;
      } else {
         continue;
      }

   }
   return NULL;
}

void
launch_prompt_and_wait(void) throw (std::runtime_error)
{
   int r;

   pthread_t prompt_thread;
   r = pthread_create(&prompt_thread, NULL, &prompt_loop, NULL);
   if (r != 0) { // some pthread_create error
      if (r == EAGAIN) {
         throw_fname("pthread_create", "not enough system resources");
      } else {
         fprintf(stderr, "pthread_create: unknown error\n");
         exit(EXIT_FAILURE);
      }
   }

   r = pthread_join(prompt_thread, NULL);
   if (r != 0) {
      switch (r) {
      case ESRCH:
         fprintf(stderr, "pthread_join: thread not found\n");
         exit(EXIT_FAILURE);
         break;
      case EINVAL:
         fprintf(stderr, "pthread_join: detached thread or already joined by another thread\n");
         exit(EXIT_FAILURE);
         break;
      case EDEADLK:
         fprintf(stderr, "pthread_join: thread tried to join to itself\n");
         exit(EXIT_FAILURE);
         break;
      default:
         fprintf(stderr, "pthread_join: unknown error\n");
         exit(EXIT_FAILURE);
      }
   }
}

void
test_prompt(void)
{
   {
      const char * line = "exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = " exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "     exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = " 	exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "	exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "	  	 exit\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit \n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit 	\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit  		  		\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "	  	exit		 	\n";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit ";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "exit		";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "   	     exit	  			  ";
      char clean[LINESZ];
      const char * target = "exit";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	uploads\n";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	uploads";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	uploads\n";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	   	uploads\n";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = " info	  		  uploads	";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   {
      const char * line = "info	 	uploads	\n";
      char clean[LINESZ];
      const char * target = "info uploads";
      clean_line(line, clean);
      if (memcmp(clean, target, strlen(target)+1) != 0) {
         fprintf(stderr, "test error: clean_line() of \"%s\" failed\n", line);
         exit(EXIT_FAILURE);
      }
   }

   // test is_command
   {
      // real commands
      assert(is_command("find bla"));
      assert(is_command("get 3"));
      assert(is_command("info"));
      assert(is_command("info uploads"));
      assert(is_command("info downloads"));
      assert(is_command("info hosts"));
      assert(is_command("info uploads downloads"));
      assert(is_command("info uploads hosts"));
      assert(is_command("info downloads uploads"));
      assert(is_command("info downloads hosts"));
      assert(is_command("info hosts uploads"));
      assert(is_command("info hosts downloads"));
      assert(is_command("info uploads downloads hosts"));
      assert(is_command("info uploads hosts downloads"));
      assert(is_command("info downloads uploads hosts"));
      assert(is_command("info downloads hosts uploads"));
      assert(is_command("info hosts uploads downloads"));
      assert(is_command("info hosts downloads uploads"));
      assert(is_command("stop 12"));
      assert(is_command("exit"));
      assert(is_command("set"));
      assert(is_command("set rowebcache"));
      assert(is_command("set localport"));
      assert(is_command("set sharepath"));
      assert(is_command("set incomingpath"));
      assert(is_command("set downloadpath"));
      assert(is_command("set rowebcache http://www.google.com"));
      assert(is_command("set localport 2345"));
      assert(is_command("set sharepath /tmp/bla/share"));
      assert(is_command("set incomingpath /tmp/bla/incoming"));
      assert(is_command("set downloadpath /tmp/bla/download"));

      // bad commands
      assert(! is_command(""));
      assert(! is_command("dsfgsdfgkj ñldfggsdkfh gs"));

      assert(! is_command("get"));
      assert(! is_command("getas"));
      assert(! is_command("get -145"));
      assert(! is_command("get -1"));
      assert(! is_command("get hola"));
      assert(! is_command("get 23bla"));
      assert(! is_command("get ho23la"));
      assert(! is_command("get ho23"));

      assert(! is_command("infoa"));
      assert(! is_command("info a"));
      assert(! is_command("info afasdf"));
      assert(! is_command("info hostsa"));
      assert(! is_command("info hosts a"));
      assert(! is_command("info hosts upaloads"));
      assert(! is_command("info hosts upaloads far"));
      assert(! is_command("info hosts upaloads downloads far"));
      assert(! is_command("info hosts bla uploads downloads"));
      assert(! is_command("info hosts hosts"));
      assert(! is_command("info hosts uploads uploads downloads"));
      assert(! is_command("info hosts uploads downloads hosts"));
      assert(! is_command("info hosts downloads downloads"));
      assert(! is_command("info hostsdownloads"));
      assert(! is_command("info 12"));

      assert(! is_command("stop"));
      assert(! is_command("stop bla"));
      assert(! is_command("stop a"));
      assert(! is_command("stop -1"));
      assert(! is_command("stop 0"));
      assert(! is_command("stop -1245"));
      assert(! is_command("stop 12 32"));
      assert(! is_command("stop as 3"));
      assert(! is_command("stop 3as"));
      assert(! is_command("stop as12s"));
      assert(! is_command("stop sdasdfasdfasdfa"));

      assert(! is_command("exita"));
      assert(! is_command("exit 12"));
      assert(! is_command("exit asdfas"));
      assert(! is_command("exit exit"));

      assert(! is_command("setfas"));
      assert(! is_command("set fasdf"));
      assert(! is_command("set fasdkj hlkasdhf asdf ha"));
      assert(! is_command("set asdf fasd"));
      assert(! is_command("set rowebcache 12"));
      assert(! is_command("set rowebcache http:/www.google.com"));
      assert(! is_command("set localport as"));
      assert(! is_command("set localport -12"));
      assert(! is_command("set localport rowebcache"));
   }
}
