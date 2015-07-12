# Makefile

.PHONY: all clean

CXXFLAGS=-O0 -Wall -Wextra -std=c++11 -pedantic -ggdb -I. -Igmock-1.7.0

all : bit test

bit : bit.o
	$(CXX) -o $@ $^

bit.o : bit.cpp bitset.hpp

test : custom_main.o gmock-gtest-all.o test_bitset.o
	$(CXX) -o $@ $^ -pthread

test_bitset.o : test_bitset.cpp bitset.hpp

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

