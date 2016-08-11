CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-Wl,-Bstatic -lboost_system -Wl,-Bdynamic -lpthread
INCLUDE=-I include -I lib

.PHONY: all build test docs clean
all: build test docs clean

build:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -shared -fPIC -o obj/liblc2pp.so src/core/message.cc src/core/connection.cc

install:
	# requires sudo
	cp obj/liblc2pp.so /usr/lib/

test:
	# shared library compilation: Requires install!
	#$(CXX) $(CXXFLAGS) $(INCLUDE) -o test/bin/main.o test/src/main.cc -Lobj/ -llc2pp $(LDFLAGS)
	# test/bin/main.o

	# static library compilation:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o test/bin/main.o\
		test/src/main.cc\
		src/core/message.cc\
		src/core/connection.cc $(LDFLAGS)
	test/bin/main.o

docs:
	doxygen doc/Doxyfile

clean:
	rm -r -f bin/*.o test/bin/*.o obj/* doc/html doc/latex
