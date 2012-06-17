LIBNAME  = tincan

TARGETLIBDIR = lib/$(LIBNAME)
TARGETLIB    = $(TARGETLIBDIR)/lib$(LIBNAME).a
TINCANLIBS   = -L$(TARGETLIBDIR) -l$(LIBNAME)

# Dependency on dbc-cpp

DBCCPPDIR    = lib/dbccpp
DBCCPPLIB    = $(DBCCPPDIR)/lib/libdbccpp.a
DBCPPINCPATH = -I$(DBCCPPDIR)/include
DBCCPPLIBS   = -L$(DBCCPPDIR)/lib -ldbccpp -lsqlite3

# For building with clang++ 3.1 in Ubuntu 12.04, install system clang and
# add -I/usr/include/clang/3.0/include to compile flags

OPTIMIZE = -O2 -g # -std=c++0x | -std=c++11
COMPILER = clang++ # g++

CXX      = $(COMPILER)
CXXFLAGS = -pipe $(OPTIMIZE) -fPIC -Wall -Wextra -Werror -D_REENTRANT
INCPATH  = -Iinclude $(DBCPPINCPATH)

TEST        = tincan-test
TESTCPPDIR  = test/testcpp
TESTCPPLIB  = $(TESTCPPDIR)/lib/libtestcpp.a
TESTINCPATH = $(INCPATH) -I$(TESTCPPDIR)/include
TESTCPPLIBS = -L$(TESTCPPDIR)/lib -ltestcpp

LINK     = $(COMPILER)
LFLAGS   = -Wl,-O1
LIBS     = $(TINCANLIBS) $(TESTCPPLIBS) $(DBCCPPLIBS)

AR       = ar cqs

DEP      = Makefile.dep

# Generic source file lists

SRC      = $(wildcard src/*.cpp) \
           $(wildcard src/db/*.cpp) \
           $(wildcard src/xml/*.cpp)

OBJS     = $(patsubst src/%.cpp, obj/%.o, $(SRC))

TESTSRC  = $(wildcard test/src/*.cpp)
TESTOBJS = $(patsubst test/src/%.cpp, test/obj/%.o, $(TESTSRC))

# Targets

obj/%.o: src/%.cpp
	mkdir -p obj/db obj/xml
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

test/obj/%.o: test/src/%.cpp
	mkdir -p test/obj
	$(CXX) -c $(CXXFLAGS) $(TESTINCPATH) -o $@ $<

$(TARGETLIB): $(OBJS)
	mkdir -p $(TARGETLIBDIR)
	rm -f $@
	$(AR) $@ $(OBJS)

$(DBCCPPLIB): $(DBCCPPDIR)/Makefile
	cd $(DBCCPPDIR); make -j 4

$(TESTCPPLIB): $(TESTCPPDIR)/Makefile
	cd $(TESTCPPDIR); make -j 4

$(TEST): $(TARGETLIB) $(DBCCPPLIB) $(TESTCPPLIB) $(TESTOBJS)
	$(LINK) $(LFLAGS) -o $@ $(TESTOBJS) $(LIBS)

test: $(TEST)
	./$(TEST)

dbg: $(TEST)
	cgdb ./$(TEST)

clean:
	rm -f $(OBJS) $(TARGETLIB) $(TESTOBJS) $(TEST)

# Automatic dependency handling

dep: $(SRC) $(TESTSRC)
	$(CXX) $(TESTINCPATH) -MM $(SRC) $(TESTSRC) \
		| sed -r 's#^[^[:space:]]+: (test/)?(src)/([^[:space:]]+).cpp#\1obj/\3.o: \1\2/\3.cpp#' \
		> $(DEP)

include $(DEP)
