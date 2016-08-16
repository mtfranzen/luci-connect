CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-Wl,-Bstatic -lboost_system -Wl,-Bdynamic -lpthread
LOGFLAGS=-DELPP_THREAD_SAFE
GMOCK_DIR=test/include/googletest/googlemock
GTEST_DIR=test/include/googletest/googletest

DEBUGFLAGS = -Iinclude $(LDFLAGS) $(LOGFLAGS) -g -shared -fPIC
BUILDFLAGS = -Iinclude $(LDFLAGS) $(LOGFLAGS) -g -shared -fPIC -DELPP_DISABLE_LOGS
TESTFLAGS=-Iinclude -I$(GMOCK_DIR)/include -I$(GTEST_DIR)/include -DELPP_DISABLE_LOGS

.PHONY: build test docs clean

build:
	$(CXX) $(CXXFLAGS) $(BUILDFLAGS) -o lib/liblc2pp.a src/core/message.cc src/core/connection.cc include/md5/md5.cpp

debug:
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -o lib/liblc2pp.a src/core/message.cc src/core/connection.cc include/md5/md5.cpp

test: build
	# compiling luci2
	mvn -f test/bin/luci2/pom.xml clean install

	# starting luci2 test server
	(cd test/bin/luci2 && (mvn exec:java -pl core >> /dev/null & echo $$!> /tmp/luci2.pid))

	# compiling googletest library
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -isystem $(GTEST_DIR)/include -I$(GMOCK_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc -o test/bin/gtest-all.o
	g++ -isystem $(GMOCK_DIR)/include -I$(GMOCK_DIR) -isystem $(GTEST_DIR)/include -I$(GMOCK_DIR) -pthread -c $(GMOCK_DIR)/src/gmock-all.cc -o test/bin/gmock-all.o
	ar -rv test/lib/libgmock.a test/bin/gtest-all.o test/bin/gmock-all.o

	# compiling test file
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -o test/bin/main.o test/src/main.cc -Llib/ -llc2pp -Ltest/lib -lgmock $(LDFLAGS) $(LOGFLAGS)

	# run tests
	LD_LIBRARY_PATH=lib/ test/bin/main.o

	# killing luci2 test server
	kill `cat /tmp/luci2.pid`

docs:
	doxygen doc/Doxyfile

clean:
	rm -r -f bin/*.o test/bin/*.o obj/* doc/html doc/latex lib/* test/lib/* logs/*
	mvn -f test/bin/luci2/pom.xml clean
