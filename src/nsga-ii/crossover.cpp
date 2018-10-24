#include "global.h"

/* Function to cross two individuals */
void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    if (nreal != 0)
    {
        realcross(parent1, parent2, child1, child2);
    }
    if (nbin != 0)
    {
        bincross(parent1, parent2, child1, child2);
    }
}

/* Routine for real variable SBX crossover */
void realcross(individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    for (int i = 0; i < nreal; i++)
    {
        float rand = randomperc();
        if (rand <= pcross_real)
        {
            int site1 = rnd(0, nvalues[i]-1);
            int site2 = rnd(0, nvalues[i]-1);
            if (site1 > site2)
            {
                int temp = site1;
                site1 = site2;
                site2 = temp;
            }
            for (int j = 0; j < site1; j++)
            {
                child1->xreal[i][j] = parent1->xreal[i][j];
                child2->xreal[i][j] = parent2->xreal[i][j];
            }
            for (int j = site1; j < site2; j++)
            {
                child1->xreal[i][j] = parent2->xreal[i][j];
                child2->xreal[i][j] = parent1->xreal[i][j];
            }
            for (int j = site2; j < nbits[i]; j++)
            {
                child1->xreal[i][j] = parent1->xreal[i][j];
                child2->xreal[i][j] = parent2->xreal[i][j];
            }
        }
        else
        {
            for (int j = 0; j < nvalues[i]; j++)
            {
                child1->xreal[i][j] = parent1->xreal[i][j];
                child2->xreal[i][j] = parent2->xreal[i][j];
            }
        }
    }
}

/* Routine for two point binary crossover */
void bincross(individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    for (int i = 0; i < nbin; i++)
    {
        float rand = randomperc();
        if (rand <= pcross_bin)
        {
            int site1 = rnd(0, nbits[i]-1);
            int site2 = rnd(0, nbits[i]-1);
            if (site1 > site2)
            {
                int temp = site1;
                site1 = site2;
                site2 = temp;
            }
            for (int j = 0; j < site1; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
            for (int j = site1; j < site2; j++)
            {
                child1->gene[i][j] = parent2->gene[i][j];
                child2->gene[i][j] = parent1->gene[i][j];
            }
            for (int j = site2; j < nbits[i]; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
        }
        else
        {
            for (int j = 0; j < nbits[i]; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
        }
    }
}
