#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>
#include <vector>

#define INF 1.0e14
#define EPS 1.0e-14
#define E  2.71828182845905
#define PI 3.14159265358979

typedef struct
{
    float *base_address;
    float **xreal;
    float **gene;
    float *obj;
}
individual;

typedef struct
{
    individual *ind;
}
population;

extern int popsize;
extern int ngen;
extern int nobj;
extern int nreal;
extern int *nvalues;
extern float min_realvar;
extern float max_realvar;
extern float pcross_real;
extern float pmut_real;
extern int nbin;
extern int *nbits;
extern float pcross_bin;
extern float pmut_bin;

void allocate_memory_pop(population *pop, int size, std::vector<std::vector<int>> total_weights);
void allocate_memory_ind(individual *ind, std::vector<std::vector<int>> total_weights);
void deallocate_memory_pop(population *pop, int size);
void deallocate_memory_ind(individual *ind);

void crossover(individual *parent1, individual *parent2, individual *child1, individual *child2);
void realcross(individual *parent1, individual *parent2, individual *child1, individual *child2);
void bincross(individual *parent1, individual *parent2, individual *child1, individual *child2);

int check_dominance(individual *a, individual *b);

void evaluate_pop(population *pop);
void evaluate_ind(individual *ind);

void initialize_pop(population *pop);
void initialize_ind(individual *ind);

void merge(population *pop1, population *pop2, population *pop3);
void copy_ind(individual *ind1, individual *ind2);

void mutation_pop(population *pop);
void mutation_ind(individual *ind);
void bin_mutate_ind(individual *ind);
void real_mutate_ind(individual *ind);

void test_problem(float *base_address, float **xreal, float **gene, float *obj);

void selection(population *old_pop, population *new_pop);
individual* tournament(individual *ind1, individual *ind2);

void evolve(population *mixed_pop, population *parent_pop);
void quickSort(int index_arr[], float value_arr[], int low, int high);
int partition(int index_arr[], float value_arr[], int low, int high);

void init_rand(int seed);
float randomperc(void);
int rnd(int low, int high);
float rndreal(float low, float high);
float rndnorm (float mean, float stddev);

#endif
