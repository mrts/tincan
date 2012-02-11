LIBNAME  = tincan

# General variables

TEST     = tincan-test

TARGET   = lib/lib$(LIBNAME).a

DEBUG    =

CXX      = g++
CXXFLAGS = -pipe -O2 $(DEBUG) -fPIC -Wall -Wextra -Werror -D_REENTRANT
INCPATH  = -Iinclude

LINK     = g++
LFLAGS   = -Wl,-O1
LIBS     = -Llib -l$(LIBNAME)

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
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

$(TEST): $(TARGET) $(TESTOBJS)
	$(LINK) $(LFLAGS) -o $@ $(TESTOBJS) $(LIBS)

test: $(TEST)
	./$(TEST)

clean:
	rm -f $(OBJS) $(TARGET) $(TESTOBJS) $(TEST)

# Automatic dependency handling

dep: $(SRC) $(TESTSRC)
	$(CXX) -MM $(SRC) | sed 's%^\([[:alpha:]]\)%obj/\1%' > $(DEP)

include $(DEP)
