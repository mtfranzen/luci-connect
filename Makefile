build-static:
	g++ main.cpp -o bin/main.o -I "lib/" -Wl,-Bstatic -lboost_system -Wl,-Bdynamic

build-dynamic:
	g++ main.cpp -o bin/main.o -lboost_system
