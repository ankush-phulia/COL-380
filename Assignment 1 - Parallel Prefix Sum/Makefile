CC = g++
CXXFLAGS = -std=c++11 -o2 -lssl -lcrypto -I include/
PFLAGS = -fopenmp -pthread -Wall

seq:
	$(CC) src/seq-prefix.cpp $(CXXFLAGS) $(PFLAGS) -o seq

par :
	$(CC) src/par-prefix.cpp $(CXXFLAGS) $(PFLAGS) -o par

clean : 
	rm -f par seq