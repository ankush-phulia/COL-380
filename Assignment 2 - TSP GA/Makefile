CC = g++
CXXFLAGS = -std=c++11 -O2 -I include/
PFLAGS = -fopenmp -pthread -Wall

all:
	$(CC) src/* $(CXXFLAGS) $(PFLAGS) -o tsp

clean : 
	rm -f tsp