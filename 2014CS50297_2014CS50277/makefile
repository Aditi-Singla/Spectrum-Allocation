all: allocate.o main.o helpers.o
	g++ -O3 allocate.o main.o helpers.o -o output

allocate.o: allocate.cpp tower_allocation.h
	g++ -O3 -std=c++11 -c allocate.cpp

main.o: main.cpp tower_allocation.h
	g++ -O3 -std=c++11 -c main.cpp

helpers.o: helpers.cpp tower_allocation.h
	g++ -O3 -std=c++11 -c helpers.cpp

clean:
	rm -rf output
	rm -rf *.o