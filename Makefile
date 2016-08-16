CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-Wl,-Bstatic -lboost_system -Wl,-Bdynamic -lpthread
INCLUDE=-I include
LOGFLAGS=-DELPP_THREAD_SAFE

.PHONY: all build test docs clean
all: build test docs clean

debug:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -g -shared -fPIC -o lib/liblc2pp.so src/core/message.cc src/core/connection.cc include/md5/md5.cpp $(LOGFLAGS)

build:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -shared -fPIC -o lib/liblc2pp.so src/core/message.cc src/core/connection.cc include/md5/md5.cpp $(LOGFLAGS) -DELPP_DISABLE_DEBUG_LOGS

test:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o test/bin/main.o test/src/main.cc -Llib/ -llc2pp $(LDFLAGS) $(LOGFLAGS) -DELPP_DISABLE_DEBUG_LOGS
	LD_LIBRARY_PATH=lib/ test/bin/main.o

docs:
	doxygen doc/Doxyfile

clean:
	rm -r -f bin/*.o test/bin/*.o obj/* doc/html doc/latex lib/*
