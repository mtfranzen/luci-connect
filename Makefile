CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-Wl,-Bstatic -lboost_system -Wl,-Bdynamic -lpthread
INCLUDE=-I include

.PHONY: all build test docs clean
all: build test docs clean

debug:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -g -shared -fPIC -o lib/liblc2pp.so src/core/message.cc src/core/connection.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) -g -o test/bin/main.o test/src/main.cc -Llib/ -llc2pp $(LDFLAGS)

build:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -shared -fPIC -o lib/liblc2pp.so src/core/message.cc src/core/connection.cc

test:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o test/bin/main.o test/src/main.cc -Llib/ -llc2pp $(LDFLAGS)
	LD_LIBRARY_PATH=lib/ test/bin/main.o

docs:
	doxygen doc/Doxyfile

clean:
	rm -r -f bin/*.o test/bin/*.o obj/* doc/html doc/latex lib/*
