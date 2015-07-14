# Makefile

.PHONY: all clean

CXXFLAGS=-O0 -Wall -Wextra -std=c++11 -pedantic -ggdb -I. -Igmock-1.7.0

all : bit test prof

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

prof : prof.o
	$(CXX) -o $@ $^ -fprofile-arcs -pg

prof.o : prof.cpp bitset.hpp
	$(CXX) -o $@ -c $< -O0 -Wall -Wextra -std=c++11 -pedantic -ggdb -I. -fprofile-arcs -pg

clean :
	rm -f *.o
	rm -f *.gcda
	rm -f gmon.out
	rm -f bit
	rm -f test
	rm -f prof

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

