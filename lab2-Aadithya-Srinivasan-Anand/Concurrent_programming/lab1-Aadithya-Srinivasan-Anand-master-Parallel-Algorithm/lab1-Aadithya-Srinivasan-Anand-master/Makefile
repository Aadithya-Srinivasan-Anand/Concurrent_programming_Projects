CFLAGS = -Wall -g -std=c++11
CXX = g++

all: sort

sort:
	$(CXX) op_test_1.cpp parallel_qsort_alg.cpp parallel_bsort_alg.cpp -o mysort -pthread $(CFLAGS)

clean:
	rm mysort