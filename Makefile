CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-Wl,-Bdynamic -lpthread
LOGFLAGS=-DELPP_THREAD_SAFE -DELPP_DISABLE_DEFAULT_CRASH_HANDLING
GMOCK_DIR=test/include/googletest/googlemock
GTEST_DIR=test/include/googletest/googletest

DEBUGFLAGS = -Iinclude $(LDFLAGS) $(LOGFLAGS) -g -shared -fPIC -DASIO_STANDALONE
BUILDFLAGS = -Iinclude $(LDFLAGS) $(LOGFLAGS) -g -shared -fPIC -DELPP_DISABLE_LOGS -DASIO_STANDALONE
TESTFLAGS=-Iinclude -Itest/include -I$(GMOCK_DIR)/include -I$(GTEST_DIR)/include

.PHONY: build test docs clean

build:
	$(CXX) $(CXXFLAGS) $(BUILDFLAGS) -o lib/liblc2pp.a src/core/*.cc src/*.cc include/md5/md5.cpp

debug:
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -o lib/liblc2pp.a src/core/*.cc src/*.cc include/md5/md5.cpp

test: build
	# compiling luci2
	mvn -f test/bin/luci2/pom.xml clean install

	# starting luci2 test server
	(cd test/bin/luci2 && (mvn exec:java -pl core >> /dev/null & echo $$!> /tmp/luci2.pid))

	# compiling googletest library
	$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -isystem $(GTEST_DIR)/include -I$(GMOCK_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc -o test/bin/gtest-all.o
	$(CXX) -isystem $(GMOCK_DIR)/include -I$(GMOCK_DIR) -isystem $(GTEST_DIR)/include -I$(GMOCK_DIR) -pthread -c $(GMOCK_DIR)/src/gmock-all.cc -o test/bin/gmock-all.o
	ar -rv test/lib/libgmock.a test/bin/gtest-all.o test/bin/gmock-all.o

	# compiling test file
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -o test/bin/main.o test/src/core/*.cc test/src/*.cc -Llib/ -llc2pp -Ltest/lib -lgmock $(LDFLAGS) $(LOGFLAGS)

	# run tests
	LD_LIBRARY_PATH=lib/ test/bin/main.o

	# killing luci2 test server
	kill `cat /tmp/luci2.pid`

docs:
	doxygen doc/Doxyfile

clean:
	rm -r -f bin/*.o test/bin/*.o obj/* doc/html doc/latex lib/* test/lib/* logs/*
	mvn -f test/bin/luci2/pom.xml clean
