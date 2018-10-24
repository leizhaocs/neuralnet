#include "global.h"

/* Function to initialize a population randomly */
void initialize_pop(population *pop)
{
    for (int i = 0; i < popsize; i++)
    {
        initialize_ind(&(pop->ind[i]));
    }
}

/* Function to initialize an individual randomly */
void initialize_ind(individual *ind)
{
    if (nreal != 0)
    {
        for (int j = 0; j < nreal; j++)
        {
            for (int k = 0; k < nvalues[j]; k++)
            {
                ind->xreal[j][k] = rndnorm(0.0, 0.1);
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
    if (nbin != 0)
    {
        for (int j=0; j < nbin; j++)
        {
            for (int k = 0; k < nbits[j]; k++)
            {
                if (randomperc() <= 0.5)
                {
                    ind->gene[j][k] = 1.0;
                }
                else
                {
                    ind->gene[j][k] = -1.0;
                }
            }
        }
    }
}
