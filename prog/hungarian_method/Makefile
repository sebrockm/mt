dijkstra: dijkstra_improvable.hpp main.cpp
	g++ main.cpp -o test -std=c++11 -Wall -g

debug: dijkstra
	gdb test

test: dijkstra
	./test test.dat
