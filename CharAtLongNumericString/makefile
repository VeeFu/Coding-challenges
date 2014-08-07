SRCDIR  = ./src/
TESTDIR = ./test/
INCDIR  = $(SRCDIR)include/

TESTSOURCES = $(TESTDIR)test.cpp
TESTOBJS = $(TESTSOURCES:.cpp=.o)
TESTEXE = test.exe

PROJSOURCES = $(SRCDIR)CharAtLongNumericString.cpp
PROJOBJS = $(PROJSOURCES:.cpp=.o)

SOLUTIONFILE = ./test/NumericStringGenerator/src/1000000000ints.data



all: test

test: $(TESTOBJS) $(PROJOBJS) 
	g++ $(TESTOBJS) $(PROJOBJS) -o $(TESTEXE)

runtest: test
	./$(TESTEXE) $(SOLUTIONFILE)

clean:
	rm -rf build
	rm $(TESTOBJS) $(PROJOBJS)

.cpp.o:
	g++ -c $< -I$(INCDIR) -o $@

