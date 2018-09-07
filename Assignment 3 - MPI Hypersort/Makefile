CC = mpic++
CXXFLAGS = -std=c++11 -O2 -I include/
PFLAGS = -fopenmp -pthread -Wall

all:
	$(CC) src/* $(CXXFLAGS) $(PFLAGS) -o sort

clean : 
	rm -f sort