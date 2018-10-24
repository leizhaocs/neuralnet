#include "global.h"

/* Routine for tournament selection, it creates a new_pop from old_pop by performing tournament selection and the crossover */
void selection(population *old_pop, population *new_pop)
{
    int temp;
    int rand;
    individual *parent1, *parent2;
    int *a1 = (int *)malloc(popsize*sizeof(int));
    int *a2 = (int *)malloc(popsize*sizeof(int));
    for (int i = 0; i < popsize; i++)
    {
        a1[i] = a2[i] = i;
    }
    for (int i=0; i < popsize; i++)
    {
        rand = rnd(i, popsize-1);
        temp = a1[rand];
        a1[rand] = a1[i];
        a1[i] = temp;
        rand = rnd(i, popsize-1);
        temp = a2[rand];
        a2[rand] = a2[i];
        a2[i] = temp;
    }
    for (int i = 0; i < popsize; i += 4)
    {
        parent1 = tournament(&old_pop->ind[a1[i]], &old_pop->ind[a1[i+1]]);
        parent2 = tournament(&old_pop->ind[a1[i+2]], &old_pop->ind[a1[i+3]]);
        crossover(parent1, parent2, &new_pop->ind[i], &new_pop->ind[i+1]);
        parent1 = tournament(&old_pop->ind[a2[i]], &old_pop->ind[a2[i+1]]);
        parent2 = tournament(&old_pop->ind[a2[i+2]], &old_pop->ind[a2[i+3]]);
        crossover(parent1, parent2, &new_pop->ind[i+2], &new_pop->ind[i+3]);
    }
    free(a1);
    free(a2);
}

/* Routine for binary tournament */
individual* tournament(individual *ind1, individual *ind2)
{
    int flag = check_dominance(ind1, ind2);
    if (flag == 1)
    {
        return ind1;
    }
    if (flag == -1)
    {
        return ind2;
    }
    if (randomperc() <= 0.5)
    {
        return ind1;
    }
    else
    {
        return ind2;
    }
}
