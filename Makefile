# Makefile

.PHONY: all clean

CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -pedantic -ggdb -I. -Igmock-1.7.0

all : bit test

bit : bit.o
	$(CXX) -o $@ $^

bit.o : bit.cpp bitset.hpp

test : custom_main.o gmock-gtest-all.o test_bitset.o
	$(CXX) -o $@ $^ -pthread

custom_main.o : gmock-1.7.0/custom_main.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

gmock-gtest-all.o : gmock-1.7.0/gmock-gtest-all.cc
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean :
	rm -f *.o
	rm -f bit
	rm -f test

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

