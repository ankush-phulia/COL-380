#pragma once
#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

struct Info{
	int comm_size;
	int proc_Id;
	int64_t *input;
	int64_t input_size;
	int64_t *proc_array;
	int64_t proc_load_size;
	int64_t *low, *high;
};

template <typename T>
void print(vector<T> &vec) {
 for (const auto& e : vec) {
     cout << e << ' ';
 }
 cout << endl;
}

template <typename T>
void sort(vector<T> &arr){
    sort(arr.begin(), arr.end());
}

// void getLoad(string in_file, Info &ProcInfo) {

// 	if (ProcInfo.proc_Id == 0) {
// 		ifstream f_in;
// 		f_in.open(in_file);
// 		f_in >> ProcInfo.input_size;
// 		ProcInfo.input = new long[ProcInfo.input_size];/*+ ProcInfo.comm_size*/
// 		for (int64_t i = 0; i < ProcInfo.input_size; i++){
// 			f_in >> ProcInfo.input[i];
// 		}
// 		// for (int64_t i = 0; i < ProcInfo.comm_size; i++){
// 		// 	ProcInfo.input[i + ProcInfo.input_size] = MAX_INT;
// 		// }
// 		f_in.close();
// 	}
	
// 	MPI_Bcast(&ProcInfo.input_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);
// 	int chunk_size = ProcInfo.input_size / ProcInfo.comm_size;
// 	ProcInfo.proc_load_size =  /*ceil(1.0 * ProcInfo.input_size / ProcInfo.comm_size) + 0.1*/chunk_size;
// 	ProcInfo.proc_array = new long[(chunk_size ) * ProcInfo.comm_size];
// 	ProcInfo.low  = new long[(chunk_size ) * ProcInfo.comm_size];
// 	ProcInfo.high = new long[(chunk_size ) * ProcInfo.comm_size];

// 	MPI_Scatter(ProcInfo.input, ProcInfo.proc_load_size, MPI_LONG, ProcInfo.proc_array, ProcInfo.proc_load_size, MPI_LONG, 0, MPI_COMM_WORLD);
// }