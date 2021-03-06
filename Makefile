#CXX=g++-7
CXXFLAGS=-O0 -Wall -Wextra -std=c++11 -pedantic -ggdb -Isrc -Igmock-1.7.0

.PHONY: all
all : bit testrunner prof

.PHONY: unittest
unittest : testrunner
	./testrunner

.PHONY: coverage
coverage : testrunner
	lcov --quiet --zerocounters --directory src --output-file coverage.info
	./testrunner
	lcov --quiet --capture --no-external --directory src --output-file coverage.info
	lcov --quiet --remove coverage.info '/usr/*'
	genhtml coverage.info --output-directory coverage

bit : src/bit.o
	$(CXX) -o $@ $^

src/bit.o : src/bit.cpp src/bitset.hpp

testrunner : gmock-1.7.0/custom_main.o gmock-1.7.0/gmock-gtest-all.o src/test_bitset.o
	$(CXX) -o $@ $^ -pthread --coverage

src/test_bitset.o : src/test_bitset.cpp src/bitset.hpp src/bitset_string.hpp
	$(CXX) -o $@ -c src/test_bitset.cpp $(CXXFLAGS) --coverage

gmock-1.7.0/custom_main.o : gmock-1.7.0/custom_main.cpp
	$(CXX) -o $@ -c $< -O3 -Wall -std=c++11 -Igmock-1.7.0

gmock-1.7.0/gmock-gtest-all.o : gmock-1.7.0/gmock-gtest-all.cc
	$(CXX) -o $@ -c $< -O3 -Wall -std=c++11 -Igmock-1.7.0

prof : src/prof.o
	$(CXX) -o $@ $^ -fprofile-arcs -pg

src/prof.o : src/prof.cpp src/bitset.hpp
	$(CXX) -o $@ -c $< -O3 -Wall -Wextra -std=c++11 -pedantic -ggdb -I. -fprofile-arcs -pg

.PHONY: clean
clean :
	find . -name "*.o" | xargs rm -f
	find . -name "*.gcda" | xargs rm -f
	find . -name "*.gcno" | xargs rm -f
	rm -f *.info
	rm -f gmon.out
	rm -f bit
	rm -f testrunner
	rm -f prof
	rm -fr coverage

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

