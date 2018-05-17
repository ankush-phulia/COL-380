#include "Util.h"
#include <chrono>

std::chrono::time_point<std::chrono::system_clock> start1, start2, endd;

void getLoad(string in_file, Info &ProcInfo) {
	if (ProcInfo.proc_Id == 0) {
		//get input in proc 0
		ifstream f_in;
		f_in.open(in_file);
		f_in >> ProcInfo.input_size;
		ProcInfo.input = new int64_t[ProcInfo.input_size]; 
		for (int64_t i = 0; i < ProcInfo.input_size; i++){
			f_in >> ProcInfo.input[i];
		}
		f_in.close();
	}
	//tell all other procs about their loads and give work
	MPI_Bcast(&ProcInfo.input_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	int chunk_size = ProcInfo.input_size / ProcInfo.comm_size;
	ProcInfo.proc_load_size =  chunk_size;
	ProcInfo.proc_array = new int64_t[(chunk_size ) * ProcInfo.comm_size];
	ProcInfo.low  = new int64_t[(chunk_size ) * ProcInfo.comm_size];
	ProcInfo.high = new int64_t[(chunk_size ) * ProcInfo.comm_size];
	//scatter the array
	MPI_Scatter(ProcInfo.input, ProcInfo.proc_load_size, MPI_LONG, ProcInfo.proc_array, ProcInfo.proc_load_size, MPI_LONG, 0, MPI_COMM_WORLD);
}

int main(int argc, char *argv[]){

	Info ProcInfo;
	//init the environment
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcInfo.comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcInfo.proc_Id);
	
	string in_file = argv[1];
	getLoad(in_file, ProcInfo);
	//sort induvidual arrays
	start1 = std::chrono::system_clock::now();
	sort(ProcInfo.proc_array, ProcInfo.proc_array + ProcInfo.proc_load_size);

	if (ProcInfo.proc_Id == 0){
	   	start2 = std::chrono::system_clock::now();
	}

	MPI_Comm current_group = MPI_COMM_WORLD;
	int group_size = 2 * ProcInfo.comm_size;
	int64_t runs = 1;
	while (runs < ProcInfo.comm_size) {
		//subgroups of comm, into groups
		MPI_Comm_split(current_group, ProcInfo.proc_Id < group_size / 2, ProcInfo.proc_Id, &current_group);
		MPI_Comm_size(current_group, &group_size);
		MPI_Comm_rank(current_group, &ProcInfo.proc_Id);

		//get pivot from proc 0 and broadcast
		int64_t pivot = ProcInfo.proc_array[ProcInfo.proc_load_size / 2];
		MPI_Bcast(&pivot, 1, MPI_LONG, 0, current_group);

		//split array
		int64_t low_size = upper_bound(ProcInfo.proc_array, ProcInfo.proc_array + ProcInfo.proc_load_size, pivot) - ProcInfo.proc_array;
		int64_t high_size = ProcInfo.proc_load_size - low_size;

		//get swap partner
		int64_t partner_Id = (ProcInfo.proc_Id + group_size / 2) % group_size;
		if (ProcInfo.proc_Id >= group_size / 2) {
			//swap low region for high
			int64_t temp;
			MPI_Recv(&temp, 1, MPI_LONG, partner_Id, 0, current_group, MPI_STATUS_IGNORE);
			MPI_Recv(ProcInfo.low, temp, MPI_LONG, partner_Id, 0, current_group, MPI_STATUS_IGNORE);
			MPI_Send(&low_size, 1, MPI_LONG, partner_Id, 0, current_group);
			MPI_Send(ProcInfo.proc_array, low_size, MPI_LONG, partner_Id, 0, current_group);
			copy(ProcInfo.proc_array + low_size, ProcInfo.proc_array + low_size + high_size, ProcInfo.high);
			low_size = temp;
		}
		else {
			//swap high for low region
			MPI_Send(&high_size, 1, MPI_LONG, partner_Id, 0, current_group);
			MPI_Send(ProcInfo.proc_array + low_size, high_size, MPI_LONG, partner_Id, 0, current_group);
			MPI_Recv(&high_size, 1, MPI_LONG, partner_Id, 0, current_group, MPI_STATUS_IGNORE);
			MPI_Recv(ProcInfo.high, high_size, MPI_LONG, partner_Id, 0, current_group, MPI_STATUS_IGNORE);
			copy(ProcInfo.proc_array, ProcInfo.proc_array + low_size, ProcInfo.low);
		}
		ProcInfo.proc_load_size = low_size + high_size;
		runs *= 2;
		merge(ProcInfo.low, ProcInfo.low + low_size, ProcInfo.high, ProcInfo.high + high_size, ProcInfo.proc_array);
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &ProcInfo.proc_Id);
	if (ProcInfo.proc_Id != 0) { 
	//send to proc 0
		MPI_Send(&ProcInfo.proc_load_size, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
		MPI_Send(ProcInfo.proc_array, ProcInfo.proc_load_size, MPI_LONG, 0, 0, MPI_COMM_WORLD);
	}
	else { 
	//gather from all procs
		copy(ProcInfo.proc_array, ProcInfo.proc_array + ProcInfo.proc_load_size, ProcInfo.input);
		int64_t final = ProcInfo.proc_load_size;
		for (int64_t i = 1; i < ProcInfo.comm_size; i++) {
			int64_t chunk;
			MPI_Recv(&chunk, 1, MPI_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(ProcInfo.input + final, chunk, MPI_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			final += chunk;
		}
		//get time
		endd = std::chrono::system_clock::now();
	    std::chrono::duration<double> elapsed_seconds1 = endd - start1;
	    std::chrono::duration<double> elapsed_seconds2 = endd - start2;
	    cout << "Time1 : " << elapsed_seconds1.count() << endl;
	    cout << "Time2 : " << elapsed_seconds2.count() << endl;

		//write to file
		ofstream f_out;
		f_out.open("output" + in_file.substr(5));
		for (int64_t i = 0; i < ProcInfo.input_size; i++){
			f_out << ProcInfo.input[i] << ' ';
		}
		f_out.close();
	}


	MPI_Finalize();

	return 0;
}


// #include <mpi.h>
// const uint32_t MAX_UINT = -1;
// const int MAX_INT = MAX_UINT/2;
		// + ProcInfo.comm_size
		// for (int64_t i = 0; i < ProcInfo.comm_size; i++){
		// 	ProcInfo.input[i + ProcInfo.input_size] = MAX_INT;
		// }
	// ceil(1.0 * ProcInfo.input_size / ProcInfo.comm_size) + 0.1

			// memcpy(ProcInfo.low, ProcInfo.proc_array, low_size * sizeof(long));
			// ProcInfo.low = ProcInfo.proc_array;

		// memcpy(ProcInfo.input, ProcInfo.proc_array, ProcInfo.proc_load_size * sizeof(long));
		// ProcInfo.input = ProcInfo.proc_array;