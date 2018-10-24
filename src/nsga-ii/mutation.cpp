#include "global.h"

/* Function to perform mutation in a population */
void mutation_pop(population *pop)
{
    for (int i = 0; i < popsize; i++)
    {
        mutation_ind(&(pop->ind[i]));
    }
}

/* Function to perform mutation of an individual */
void mutation_ind(individual *ind)
{
    if (nreal != 0)
    {
        real_mutate_ind(ind);
    }
    if (nbin != 0)
    {
        bin_mutate_ind(ind);
    }
}

/* Routine for binary mutation of an individual */
void bin_mutate_ind(individual *ind)
{
    for (int j = 0; j < nbin; j++)
    {
        for (int k = 0; k < nbits[j]; k++)
        {
            float prob = randomperc();
            if (prob <= pmut_bin)
            {
                if (ind->gene[j][k] == 1.0)
                {
                    ind->gene[j][k] = -1.0;
                }
                else
                {
                    ind->gene[j][k] = 1.0;
                }
            }
        }
    }
}

/* Routine for real polynomial mutation of an individual */
void real_mutate_ind(individual *ind)
{
    for (int j = 0; j < nreal; j++)
    {
        for (int k = 0; k < nvalues[j]; k++)
        {
            float prob = randomperc();
            if (prob <= pmut_real)
            {
                ind->xreal[j][k] += rndnorm(0.0, 0.1);
                if (ind->xreal[j][k] > max_realvar)
                {
                    ind->xreal[j][k] = max_realvar;
                }
                else if (ind->xreal[j][k] < min_realvar)
                {
                    ind->xreal[j][k] = min_realvar;
                }
            }
        }
    }
}
