#include <omp.h>
#include <chrono>
#include <thread>
#include "population.h"

#define CROSS_PROB 0.8
#define MUT_PROB 0.1
#define INIT_POP 100
#define GENS_CUTOFF 320000

using namespace std;

void getSol(string &in_file, int num_threads, string &sol, double &fitness) {
    population tsp(in_file, INIT_POP, 2, CROSS_PROB, MUT_PROB);
    int generations = 0;
    string best_sol = tsp.chromosomes_pop[0];
    double best_fitness = tsp.chrm_fitnesses[0];
    string best;
    double best_fit;
    while (generations < GENS_CUTOFF / (num_threads)) {
        tsp.nextGen();
        tsp.getBest(best, best_fit);
        if (best_fit > best_fitness) {
            best_sol = best;
            best_fitness = best_fit;
        }
        generations++;
    }
    sol = best_sol;
    fitness = best_fitness;
}

int main(int argc, char const *argv[]) {
    string in_file = argv[1];
    int num_threads = atoi(argv[2]);
    int restarts = 10;
    int n = num_threads;

    if (num_threads > 32) {
        num_threads = 32;
    }

    // set up timer, http://en.cppreference.com/w/cpp/chrono
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    string solution = "";
    double fitness = -1.0;

    cout << "Thread count : " << num_threads << endl;
// #pragma omp parallel for num_threads(num_threads) schedule(dynamic)
#pragma omp parallel
#pragma omp for schedule(dynamic)
    for (int i = 0; i < num_threads; i += n) {
        population tsp(in_file, INIT_POP, 2, CROSS_PROB, MUT_PROB);
        int generations = 0;
        string best_sol = tsp.chromosomes_pop[0];
        double best_fitness = tsp.chrm_fitnesses[0];
        string best;
        double best_fit;
        for (int gen = 0; gen < GENS_CUTOFF / num_threads; gen++) {
            tsp.nextGen();
            tsp.getBest(best, best_fit);
            if (best_fit > best_fitness) {
                best_sol = best;
                best_fitness = best_fit;
            }
        }
#pragma omp critical
        if (best_fitness > fitness) {
            solution = best_sol;
            fitness = best_fitness;
        }
    }

    // calculate the time
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    cout << "Best Solution Found : " << solution << endl;
    cout << "Distance : " << 1.0 / fitness << endl;
    cout << "Time : " << elapsed_seconds.count() << endl;
}
