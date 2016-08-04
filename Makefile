test-service:
	g++ test/src/main.cpp -o test/bin/main.o -I "." -Wl,-Bstatic -lboost_system -Wl,-Bdynamic
	test/bin/main.o

build:
	# dynamic library compilation here
