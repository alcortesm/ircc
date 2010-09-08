BIN = ircc
INSTALL_DIR = ~/bin
SRCS = \
    ComJoin.cc \
    ComSleep.cc \
    ComUser.cc \
    ComNick.cc \
    ComMsg.cc \
    ComError.cc \
    Server.cc \
    ComConnect.cc \
    Args.cc \
    CommandFactory.cc \
    ComQuit.cc \
    ComUnknown.cc \
    ComNop.cc \
    NullStream.cc \
    utils.cc \
    main.cc
INCLUDES =
LIBS =



CC = g++
CFLAGS = -Wfatal-errors -Werror -Wall -Wextra -std=c++98 -pedantic-errors -g -O0 -D_BSD_SOURCE
LFLAGS = -g -lpthread
OBJS = $(SRCS:.cc=.o)
DEPS = $(OBJS:.o=.d)
INSTALL_BIN=$(INSTALL_DIR)/$(BIN)
# can't use --track-origins==yes, because monitor01 use
# an older version of valgrind
VALGRIND_FLAGS = --tool=memcheck --show-reachable=yes --leak-check=full -q



.PHONY: all install clean nuke test valgrind debugvalgrind debugtestvalgrind

all: $(BIN)

install: $(BIN)
	cp $(BIN) $(INSTALL_BIN)

uninstall:
	- rm -f $(INSTALL_BIN)

dist:
	tar -cvzf $(BIN).tgz $(SRCS) *.h Makefile

TAGS:
	cscope -b -q *.cc *.h

test: debugtestvalgrind

valgrind: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN)

debugvalgrind: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) -d

testvalgrind: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) -t

debugtestvalgrind: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) -t -d


clean:
	- rm -f $(OBJS)
	- rm -f $(BIN)
	- rm -f $(DEPS)
	- rm -f cscope.in.out cscope.out cscope.po.out
	- rm -f *~
	- rm -f $(BIN).tgz

nuke: clean
	- rm -f $(INSTALL_DIR)/$(BIN)


$(BIN): $(OBJS)
	$(CC) $(LFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBS)

.cc.o:
	$(CC) -MMD $(CFLAGS) -c -o $@ $<
	@sed -i -e '1s,\($*\)\.o[ :]*,\1.o $*.d: ,' $*.d

-include $(DEPS)
