#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;

int main(int argc, char const *argv[]){

	vector<int> array;
	int element;

	string infile = argv[1];//get input file from command line
	ifstream f_in;
    f_in.open(infile);

    if (f_in.is_open()){ //parsing file and writing to array
    	while (f_in >> element){
    		array.push_back(element);
    	}
    }

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for (int i = 1; i < array.size(); i++){
    	array[i] += array[i-1];
    }

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    
    std::cout << "Time: " << elapsed_seconds.count() << std::endl; 

}