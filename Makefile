CC = g++
CXXFLAGS = -std=c++11 -O3

all: allocate.o main.o helpers.o
	$(CC) $(CXXFLAGS) allocate.o main.o helpers.o -o output

allocate.o: src/allocate.cpp src/tower_allocation.h
	$(CC) $(CXXFLAGS) -c src/allocate.cpp

main.o: src/main.cpp src/tower_allocation.h
	$(CC) $(CXXFLAGS) -c src/main.cpp

helpers.o: src/helpers.cpp src/tower_allocation.h
	$(CC) $(CXXFLAGS) -c src/helpers.cpp

clean:
	rm -rf output
	rm -rf *.o