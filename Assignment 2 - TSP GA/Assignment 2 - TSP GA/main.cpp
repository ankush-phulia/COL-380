#include "population.h"
#include <omp.h>
#include <chrono>
#include <thread>

#define CROSS_PROB 0.8
#define MUT_PROB 0.1
#define INIT_POP 100
#define GENS_CUTOFF 320000

using namespace std;

void getSol(string &in_file, int num_threads, string &sol, double &fitness){
	population tsp = population(in_file, INIT_POP, 2, CROSS_PROB, MUT_PROB);
	int generations = 0;
	string best_sol = tsp.chromosomes_pop[0];
	double best_fitness = tsp.chrm_fitnesses[0];
	string best; double best_fit;
	while (generations < GENS_CUTOFF/(num_threads)) {
		// cout << omp_get_thread_num() << ' ' << generations << endl;
		tsp.nextGen();
		tsp.getBest(best, best_fit);
		if (best_fit > best_fitness) {
			best_sol = best;
			best_fitness = best_fit;
			// generations = 0;
		}
		// else {
		// }
		generations++;
	}
	sol = best_sol;
	fitness = best_fitness;
}

int main(int argc, char const *argv[]) {

	// #pragma omp parallel for num_threads(num)
	string in_file = argv[1];
	int num_threads = atoi(argv[2]);
	int restarts = 10;
	// vector<string> bests;
	// vector<double> fitnesses;
	// bests.resize(num_threads);
	// fitnesses.resize(num_threads);
	
	// #pragma omp parallel for num_threads(num_threads) schedule(dynamic)
	// for (int i = 0; i < num_threads; i++){
	// // for each thread
	// 	string thread_best; double thread_best_fitness = -1.0;
	// 	for (int j = 0; j < restarts; j++){ 
	// 	// for each start, do this sequentially
	// 		population tsp = population(in_file, INIT_POP, 2, CROSS_PROB, MUT_PROB);
	// 		int generations = 0;
	// 		string best_sol = tsp.chromosomes_pop[0];
	// 		double best_fitness = tsp.chrm_fitnesses[0];
	// 		string best; double best_fit;
	// 		while (generations < GENS_CUTOFF/(num_threads * num_threads)) {
	// 			//cout << "HERE" << endl;
	// 			tsp.nextGen();
	// 			//cout << "HERE2" << endl;
	// 			tsp.getBest(best, best_fit);
	// 			if (best_fit > best_fitness) {
	// 				best_sol = best;
	// 				best_fitness = best_fit;
	// 				generations = 0;
	// 			}
	// 			else {
	// 				generations++;
	// 			}
	// 		}
	// 		if (best_fitness > thread_best_fitness){
	// 			thread_best_fitness = best_fitness;
	// 			thread_best = best;
	// 		}
	// 	}
	// 	bests[i] = thread_best;
	// 	fitnesses[i] = thread_best_fitness;
	// }
	// string solution = bests[0];
	// double fitness = fitnesses[0];
	// for (int i = 1; i < num_threads; i++){
	// 	if (fitnesses[i] > fitness){
	// 		fitness = fitnesses[i];
	// 		solution = bests[i];
	// 	}
	// }
	int n = num_threads;

    if (num_threads > 32){
    	num_threads = 32;
    }

	//set up timer, http://en.cppreference.com/w/cpp/chrono
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    string solution = "";
	double fitness = -1.0;

    cout << "Thread count : " << num_threads << endl;
	// #pragma omp parallel for num_threads(num_threads) schedule(dynamic)
	#pragma omp parallel /*{*/
    	#pragma omp for schedule(dynamic)
		for (int i = 0; i < num_threads; i+= n){
			population tsp = population(in_file, INIT_POP, 2, CROSS_PROB, MUT_PROB);
			int generations = 0;
			string best_sol = tsp.chromosomes_pop[0];
			double best_fitness = tsp.chrm_fitnesses[0];
			string best; double best_fit;
			for (int gen = 0; gen < GENS_CUTOFF/num_threads; gen++){
				// cout << omp_get_thread_num() << ' ' << generations << endl;
				//cout << "HERE" << endl;
				tsp.nextGen();
				//cout << "HERE2" << endl;
				tsp.getBest(best, best_fit);
				if (best_fit > best_fitness) {
					best_sol = best;
					best_fitness = best_fit;
					// generations = 0;
				}
				else {
				}
				// generations++;
			}
			#pragma omp critical
				if (best_fitness > fitness){
					solution = best_sol;
					fitness = best_fitness;
				}
			// getSol(in_file, num_threads, bests[i], fitnesses[i]);
		// 	// fitnesses[i] = best_fitness;
		// 	// bests[i] = best_sol;
		// thread t0(getSol, ref(in_file), num_threads, ref(bests[0]), ref(fitnesses[0]));
		// thread t1(getSol, ref(in_file), num_threads, ref(bests[1]), ref(fitnesses[1]));
		// thread t2(getSol, ref(in_file), num_threads, ref(bests[2]), ref(fitnesses[2]));
		// thread t3(getSol, ref(in_file), num_threads, ref(bests[3]), ref(fitnesses[3]));
		// t0.join();
		// t1.join();
	    // t2.join();
	    // t3.join();
		}
	// }


	// fitnesses[0] = s1;
	// bests[0] = p1;
	// fitnesses[1] = s2;
	// bests[1] = p2;
	//calculate the time
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

	/*string solution = bests[0];
	double fitness = fitnesses[0];*/
	// for (int i = 1; i < num_threads; i++){
	// 	if (fitnesses[i] > fitness){
	// 		fitness = fitnesses[i];
	// 		solution = bests[i];
	// 	}
	// }

	cout << "Best Solution Found : " << solution << endl;
	cout << "Distance : " << 1.0 / fitness << endl;
	cout << "Time : " << elapsed_seconds.count() << endl;

	// int term;
	// cin >> term;
}