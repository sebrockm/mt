CC = g++
CFLAGS = -Wall -std=c++11

default: binsplitter.cpp
	$(CC) binsplitter.cpp $(CFLAGS) -o binsplitter

parts: partmaker.cpp
	$(CC) partmaker.cpp $(CFLAGS) -o partmaker

total: totale.cpp
	$(CC) totale.cpp $(CFLAGS) -o totale

test: 
	./totale test.txt test.txt.bins.dat
