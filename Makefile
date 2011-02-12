# Makefile

.PHONY: all clean

CXX=g++
CXXFLAGS=-Wall -Wextra -ansi -pedantic -ggdb

all : bit

bit : bit.o
	$(CXX) -o $@ $^

clean :
	rm -f *.o
	rm -f bit

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

