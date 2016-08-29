CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-Wl,-Bdynamic -lpthread
LOGFLAGS=-DELPP_THREAD_SAFE -DELPP_DISABLE_DEFAULT_CRASH_HANDLING
GMOCK_DIR=test/include/googletest/googlemock
GTEST_DIR=test/include/googletest/googletest

DEBUGFLAGS = -Iinclude $(LDFLAGS) $(LOGFLAGS) -g -shared -fPIC -DASIO_STANDALONE
BUILDFLAGS = -Iinclude $(LDFLAGS) $(LOGFLAGS) -g -shared -fPIC -DELPP_DISABLE_LOGS -DASIO_STANDALONE
TESTFLAGS=-Iinclude -Itest/include -I$(GMOCK_DIR)/include -I$(GTEST_DIR)/include
SAMPLEFLAGS=-Iinclude $(LDFLAGS) $(LOGFLAGS)

.PHONY: build test docs clean samples

build:
	$(CXX) $(CXXFLAGS) $(BUILDFLAGS) -o lib/libluciconnect.a src/core/*.cc src/*.cc include/md5/md5.cpp

debug:
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -o lib/libluciconnect.a src/core/*.cc src/*.cc include/md5/md5.cpp

test: build
	# compiling helen
	(cd test/bin/qua-kit/libs/hs/helen && stack install)

	# start helen
	helen >> /dev/null &

	# compiling googletest library
	$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -isystem $(GTEST_DIR)/include -I$(GMOCK_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc -o test/bin/gtest-all.o
	$(CXX) -isystem $(GMOCK_DIR)/include -I$(GMOCK_DIR) -isystem $(GTEST_DIR)/include -I$(GMOCK_DIR) -pthread -c $(GMOCK_DIR)/src/gmock-all.cc -o test/bin/gmock-all.o
	ar -rv test/lib/libgmock.a test/bin/gtest-all.o test/bin/gmock-all.o

	# compiling test file
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -o test/bin/main.o test/src/luciconnect/core/*.cc test/src/luciconnect/*.cc test/src/*.cc -Llib/ -lluciconnect -Ltest/lib -lgmock $(LDFLAGS) $(LOGFLAGS)

	# run tests
	LD_LIBRARY_PATH=lib/ test/bin/main.o

	# killing helen test server
	killall helen

docs:
	doxygen doc/Doxyfile

samples: build
	$(CXX) $(CXXFLAGS) $(SAMPLEFLAGS) -o samples/bin/simple-client.o samples/src/simple-client.cc -Llib/ -lluciconnect -pthread
	$(CXX) $(CXXFLAGS) $(SAMPLEFLAGS) -o samples/bin/simple-service.o samples/src/simple-service.cc -Llib/ -lluciconnect -pthread

clean:
	rm -r -f bin/*.o test/bin/*.o obj/* doc/html doc/latex lib/* test/lib/* logs/*
	mvn -f test/bin/luci2/pom.xml clean
