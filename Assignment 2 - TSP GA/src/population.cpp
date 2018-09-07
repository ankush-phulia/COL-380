#include "population.h"

population::population() {}

population::~population() {}

population::population(population &p) {
    this->genes = p.genes;
    this->gene_pool = p.gene_pool;
    this->gX = p.gX;
    this->gY = p.gY;
    this->gDist = p.gDist;
    this->chromosomes_pop = p.chromosomes_pop;
    this->chrm_fitnesses = p.chrm_fitnesses;
    this->pop_size = p.pop_size;
    this->num_elites = p.num_elites;
    this->member_size = p.member_size;
    this->cross_prob = p.cross_prob;
    this->mut_prob = p.mut_prob;
    this->fitsum = p.fitsum;
}

population::population(string input_file, int pop_size, int num_elites,
                       double cross_prob, double mut_prob) {
    // get gene pool from file
    ifstream f_in;
    f_in.open(input_file);
    string tag, name;
    if (f_in.is_open()) {
        f_in >> tag >> name;
        f_in >> tag >> gene_pool;
        f_in >> tag;
        genes.resize(gene_pool);
        gX.resize(gene_pool);
        gY.resize(gene_pool);
        for (int i = 0; i < gene_pool; i++) {
            f_in >> genes[i] >> gX[i] >> gY[i];
        }
    }
    f_in.close();
    // set up some params
    this->pop_size = pop_size;
    this->member_size = gene_pool;  //# of cities = path length
    this->num_elites = num_elites;
    this->cross_prob = cross_prob;
    this->mut_prob = mut_prob;
    // seed random
    srand((unsigned int)time(NULL));
    // generate random initial population
    genChromosomes(genes);
    this->chrm_fitnesses.resize(pop_size);
    getDistances();
    getFitSum();
}

void population::genChromosomes(vector<int> genes) {
    chromosomes_pop.resize(pop_size);
    for (int i = 0; i < pop_size; i++) {
        random_unique(genes.begin(), genes.end(), member_size);
        string chromosome = "";
        for (int j = 0; j < member_size; j++) {
            chromosome += (char)(genes[j] + 'A' - 1);
        }
        chromosomes_pop[i] = chromosome;
    }
}

double euDist(double X1, double Y1, double X2, double Y2) {
    return sqrt(pow((X1 - X2), 2) + pow((Y1 - Y2), 2));
}

void population::getDistances() {
    gDist.resize(gene_pool);
    for (int i = 0; i < gene_pool; i++) {
        gDist[i].resize(gene_pool);
        for (int j = 0; j < gene_pool; j++) {
            gDist[i][j] = euDist(gX[i], gY[i], gX[j], gY[j]);
        }
    }
}

double fitness(string &chromosome, vector<vector<double> > &gDist) {
    double distance = 0;
    int n = strlen(chromosome.c_str()) - 1;
    for (int i = 0; i < n; i++) {
        distance += gDist[chromosome[i + 1] - 'A'][chromosome[i] - 'A'];
    }
    distance += gDist[chromosome[n] - 'A'][chromosome[0] - 'A'];
    return (1.0 / distance);
}

struct compare_chrm {
    vector<vector<double> > gDist;
    compare_chrm(vector<vector<double> > &gDistgDist) {
        this->gDist = gDistgDist;
    }
    inline bool operator()(string &chrm1, string &chrm2) {
        return fitness(chrm1, gDist) >= fitness(chrm2, gDist);
    }
};

void population::getFitSum() {
    fitsum = 0;
    for (int i = 0; i < pop_size; i++) {
        double temp = fitness(chromosomes_pop[i], gDist);
        chrm_fitnesses[i] = temp;
        fitsum += temp;
    }
}

void population::getElites(vector<string> &dest) {
    string e1 = chromosomes_pop[0], e2 = chromosomes_pop[0];
    double best = chrm_fitnesses[0], best2 = chrm_fitnesses[0];
    for (int i = 1; i < pop_size; i++) {
        double ftemp = chrm_fitnesses[i];
        if (ftemp > best) {
            best = ftemp;
            e1 = chromosomes_pop[i];
        }
    }
    dest[0] = e1;
    for (int i = 1; i < pop_size; i++) {
        string ctemp = chromosomes_pop[i];
        double ftemp = chrm_fitnesses[i];
        if (ftemp > best2 && ctemp != e1) {
            best2 = ftemp;
            e2 = chromosomes_pop[i];
        }
    }
    dest[1] = e2;
}

double getRandom(double max) {
    return ((double)rand() * max) / (double)RAND_MAX;
}

string population::selParent() {
    double r1 = getRandom(fitsum);
    double temp = 0;
    for (int i = 0; i < pop_size; i++) {
        temp += chrm_fitnesses[i];
        if (temp >= r1) {
            return chromosomes_pop[i];
        }
    }
    cout << "ERROR" << endl;
    return (NULL);
}

void weed(string &C1, string &C2, string &swap1, string &swap2, int mem_size,
          int strt, int end) {
    for (int i = 0; i < strt; i++) {
        if (swap1.find(C1[i]) != string::npos) {
            C1[i] = '1';
        }
        if (swap2.find(C2[i]) != string::npos) {
            C2[i] = '1';
        }
    }
    for (int i = end; i < mem_size; i++) {
        if (swap1.find(C1[i]) != string::npos) {
            C1[i] = '1';
        }
        if (swap2.find(C2[i]) != string::npos) {
            C2[i] = '1';
        }
    }
}

void repair(string &C1, string &C2, int mem_size, int strt, int end) {
    int jprev = 0;
    for (int i = 0; i < strt; i++) {
        if (C1[i] == '1') {  // look for missing gene
            for (int j = jprev; j < mem_size; j++) {
                if (C2[j] != '1') {
                    if (C1.find(C2[j]) == string::npos) {  // gene in 2 not in 1
                        C1[i] = C2[j];
                        jprev = j;
                        break;
                    }
                }
            }
        }
    }
    for (int i = end; i < mem_size; i++) {
        if (C1[i] == '1') {  // look for missing gene
            for (int j = jprev; j < mem_size; j++) {
                if (C2[j] != '1') {
                    if (C1.find(C2[j]) == string::npos) {  // gene in 2 not in 1
                        C1[i] = C2[j];
                        jprev = j;
                        break;
                    }
                }
            }
        }
    }
}

void population::crossOver(string &P1, string &P2, string &C1, string &C2) {
    double random = getRandom(1);
    if (random >= cross_prob) {  // no crossover
        C1 = P1;
        C2 = P2;
    } else {
        // cout << "crossed" << endl;
        int strt, end;
        int t1 = (int)getRandom(member_size);
        int t2 = (int)getRandom(member_size);
        while (t1 == t2) {
            t2 = (int)getRandom(member_size);
        }
        strt = min(t1, t2);
        end = max(t1, t2);
        string swap1 = P2.substr(strt, end - strt);
        string swap2 = P1.substr(strt, end - strt);
        C1 = P1.substr(0, strt) + swap1 + P1.substr(end, member_size - end);
        C2 = P2.substr(0, strt) + swap2 + P2.substr(end, member_size - end);
        // weed out duplicates
        weed(C1, C2, swap1, swap2, member_size, strt, end);
        // repair the chromosome
        repair(C1, C2, member_size, strt, end);
        repair(C2, C1, member_size, strt, end);
    }
}

void population::mutate(string &chrm) {
    double random = getRandom(1);
    if (random < mut_prob) {  // mutate
        int r1 =
            (int)(getRandom(member_size) * member_size / (member_size + 1));
        int r2 =
            (int)(getRandom(member_size) * member_size / (member_size + 1));
        while (r2 == r1) {
            r2 =
                (int)(getRandom(member_size) * member_size / (member_size + 1));
        }
        char temp = chrm[r1];
        chrm[r1] = chrm[r2];
        chrm[r2] = temp;
    }
}

void population::nextGen() {
    vector<string> new_gen;
    new_gen.resize(pop_size);
    // get fittest chromosomes and add to new pop
    getElites(new_gen);
    // select parents
    for (int i = 2; i < pop_size; i += 2) {
        string P1 = selParent();
        string P2 = selParent();
        while (P1 == P2) {
            P2 = selParent();
        }
        string C1, C2;
        crossOver(P1, P2, C1, C2);
        mutate(C1);
        mutate(C2);
        new_gen[i] = C1;
        new_gen[i + 1] = C2;
    }
    // new gen becomes current population
    chromosomes_pop = new_gen;
    getDistances();
    getFitSum();
}

void population::getBest(string &best, double &bestFit) {
    string bestt = chromosomes_pop[0];
    double besttFit = chrm_fitnesses[0];
    for (int i = 1; i < pop_size; i++) {
        if (chrm_fitnesses[i] > besttFit) {
            bestt = chromosomes_pop[i];
            besttFit = chrm_fitnesses[i];
        }
    }
    best = bestt;
    bestFit = besttFit;
}
