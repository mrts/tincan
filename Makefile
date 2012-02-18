LIBNAME  = tincan

# General variables

TARGET   = lib/lib$(LIBNAME).a

DEBUG    =
COMPILER = clang++

CXX      = $(COMPILER)
CXXFLAGS = -pipe -O2 $(DEBUG) -fPIC -Wall -Wextra -Werror -D_REENTRANT
INCPATH  = -Iinclude

TEST        = tincan-test
TESTCPPDIR  = test/testcpp
TESTCPPLIB  = $(TESTCPPDIR)/lib/libtestcpp.a
TESTINCPATH = $(INCPATH) -I$(TESTCPPDIR)/include

LINK     = $(COMPILER)
LFLAGS   = -Wl,-O1
LIBS     = -Llib -l$(LIBNAME) -L$(TESTCPPDIR)/lib -ltestcpp

AR       = ar cqs

DEP      = Makefile.dep

# Generic source file lists

SRC      = $(wildcard src/*.cpp)
OBJS     = $(patsubst src/%.cpp, obj/%.o, $(SRC))

TESTSRC  = $(wildcard test/src/*.cpp)
TESTOBJS = $(patsubst test/src/%.cpp, test/obj/%.o, $(TESTSRC))

# Targets

obj/%.o: src/%.cpp
	mkdir -p obj
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

$(TARGET): $(OBJS)
	mkdir -p lib
	rm -f $@
	$(AR) $@ $<

test/obj/%.o: test/src/%.cpp
	mkdir -p test/obj
	$(CXX) -c $(CXXFLAGS) $(TESTINCPATH) -o $@ $<

$(TESTCPPLIB): $(TESTCPPDIR)/Makefile
	cd $(TESTCPPDIR); make

$(TEST): $(TARGET) $(TESTOBJS) $(TESTCPPLIB)
	$(LINK) $(LFLAGS) -o $@ $(TESTOBJS) $(LIBS)

test: $(TEST)
	./$(TEST)

clean:
	rm -f $(OBJS) $(TARGET) $(TESTOBJS) $(TEST)

# Automatic dependency handling

dep: $(SRC) $(TESTSRC)
	$(CXX) $(TESTINCPATH) -MM $(SRC) $(TESTSRC) | sed 's%^\([[:alpha:]]\)%obj/\1%' > $(DEP)

include $(DEP)
