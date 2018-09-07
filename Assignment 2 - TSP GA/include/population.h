#pragma once
#include <string.h>

#include <cstdlib>
#include <fstream>
#include "Util.h"

using namespace std;

class population {
   public:
    population();
    population(popultaion &p);
    population(string input_file, int pop_size, int num_elites,
               double cross_prob, double mut_prob);
    ~population();

   public:
    vector<int> genes;
    int gene_pool;
    vector<double> gX, gY;
    vector<vector<double> > gDist;
    vector<string> chromosomes_pop;
    vector<double> chrm_fitnesses;
    int pop_size;
    int num_elites;
    int member_size;
    double cross_prob;
    double mut_prob;
    double fitsum;

    void genChromosomes(vector<int> genes);
    void getFitSum();
    void getDistances();
    void getElites(vector<string> &dest);
    string selParent();
    void crossOver(string &P1, string &P2, string &C1, string &C2);
    void mutate(string &chrm);
    void nextGen();
    void getBest(string &best, double &bestFit);
};
