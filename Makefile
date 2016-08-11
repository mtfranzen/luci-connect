CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-Wl,-Bstatic -lboost_system -Wl,-Bdynamic -lpthread
INCLUDE="."

test-service:
	$(CXX) $(CXXFLAGS) test/src/main.cc -o test/bin/main.o $(LDFLAGS) -I $(INCLUDE)
	test/bin/main.o

build:
	# dynamic library compilation here

clean:
	rm -r -f bin/*.o test/bin/*.o
