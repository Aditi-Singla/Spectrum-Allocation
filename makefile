all: allocate.o main.o helpers.o
	g++ allocate.o main.o helpers.o -o output

allocate.o: allocate.cpp tower_allocation.h
	g++ -c allocate.cpp

main.o: main.cpp tower_allocation.h
	g++ -c main.cpp

helpers.o: helpers.cpp tower_allocation.h
	g++ -c helpers.cpp

clean:
	rm -rf output
	rm -rf *.o