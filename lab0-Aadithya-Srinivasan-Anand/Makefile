output: main.o
	g++ main.o -o mysort

main.o: main.cpp qsort.h msort.h
	g++ -c main.cpp

clean:
	rm*.o output