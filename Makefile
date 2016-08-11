CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-Wl,-Bstatic -lboost_system -Wl,-Bdynamic -lpthread
INCLUDE="."

.PHONY: all build test docs clean

all: build test docs clean

build:
	# TODO: dynamic class library here

test:
	$(CXX) $(CXXFLAGS) test/src/main.cc -o test/bin/main.o $(LDFLAGS) -I $(INCLUDE)
	test/bin/main.o

docs:
	doxygen doc/Doxyfile

clean:
	rm -r -f bin/*.o test/bin/*.o doc/html doc/latex
