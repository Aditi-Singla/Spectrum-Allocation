all: allocate.o main.o helpers.o
	g++ -O3 allocate.o main.o helpers.o -o output

allocate.o: src/allocate.cpp src/tower_allocation.h
	g++ -O3 -std=c++11 -c src/allocate.cpp

main.o: src/main.cpp src/tower_allocation.h
	g++ -O3 -std=c++11 -c src/main.cpp

helpers.o: src/helpers.cpp src/tower_allocation.h
	g++ -O3 -std=c++11 -c src/helpers.cpp

clean:
	rm -rf output
	rm -rf *.o