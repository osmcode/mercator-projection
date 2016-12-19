
CXXFLAGS := -std=c++11 -O3 -Werror -Wall -Wextra

PROGRAMS := compare_results benchmark

all: ${PROGRAMS}

compare_results.o: compare_results.cpp
	${CXX} -c $^ ${CXXFLAGS} -o $@

compare_results: compare_results.o
	${CXX} $^ -o $@

benchmark.o: benchmark.cpp
	${CXX} -c $^ ${CXXFLAGS} -o $@

benchmark: benchmark.o
	${CXX} $^ -o $@

clean:
	rm -f ${PROGRAMS} *.o

