
CXXFLAGS := -std=c++11 -O3 -Werror -Wall -Wextra

OSMIUM_INC := ../libosmium/include

PROGRAMS := compare_results benchmark lat_stat

all: ${PROGRAMS}

compare_results.o: compare_results.cpp
	${CXX} -c $^ ${CXXFLAGS} -o $@

compare_results: compare_results.o
	${CXX} $^ -o $@

benchmark.o: benchmark.cpp
	${CXX} -c $^ ${CXXFLAGS} -o $@

benchmark: benchmark.o
	${CXX} $^ -o $@

lat_stat.o: lat_stat.cpp
	${CXX} -c $^ ${CXXFLAGS} -I${OSMIUM_INC} -o $@

lat_stat: lat_stat.o
	${CXX} $^ -o $@ -lz -pthread

clean:
	rm -f ${PROGRAMS} *.o

