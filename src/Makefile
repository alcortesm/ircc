BIN = ircc
INSTALL_DIR = ~/bin
SRCS = \
    dcc.cc \
    ctcp.cc \
    DccServer.cc \
    msg_factory.cc \
    Msg.cc \
    ComOffer.cc \
    ComWho.cc \
    ComList.cc \
    ComDisconnect.cc \
    ComLeave.cc \
    ComAuth.cc \
    ComHelp.cc \
    ComJoin.cc \
    ComSleep.cc \
    ComMsg.cc \
    ComError.cc \
    Server.cc \
    ComConnect.cc \
    Args.cc \
    com_factory.cc \
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



.PHONY: all install uninstall dist TAGS test testdebug valgrind valgrinddebug selftest selftestdebug clean nuke upload

all: $(BIN)

install: $(BIN)
	cp $(BIN) $(INSTALL_BIN)

uninstall:
	- rm -f $(INSTALL_BIN)

dist:
	tar -cvzf $(BIN).tgz $(SRCS) *.h Makefile TODO

TAGS:
	cscope -b -q *.cc *.h

test: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) < ../test/offer_local_path

testdebug: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) -d < ../test/offer_local_path

valgrind: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN)

valgrinddebug: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) -d

selftest: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) -t

selftestdebug: $(BIN)
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

upload:
	- ../sh/git-upload

$(BIN): $(OBJS)
	$(CC) $(LFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBS)

.cc.o:
	$(CC) -MMD $(CFLAGS) -c -o $@ $<
	@sed -i -e '1s,\($*\)\.o[ :]*,\1.o $*.d: ,' $*.d

-include $(DEPS)
