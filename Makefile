# Makefile

.PHONY: all clean

CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -pedantic -ggdb -I.

all : bit

bit : bit.o
	$(CXX) -o $@ $^

bit.o : bit.cpp bitset.hpp

clean :
	rm -f *.o
	rm -f bit

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

