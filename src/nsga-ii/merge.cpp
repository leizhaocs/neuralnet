#include "global.h"

/* Routine to merge two populations into one */
void merge(population *pop1, population *pop2, population *pop3)
{
    for (int i = 0; i < popsize; i++)
    {
        copy_ind(&(pop1->ind[i]), &(pop3->ind[i]));
    }
    for (int i = 0, k = popsize; i < popsize; i++, k++)
    {
        copy_ind(&(pop2->ind[i]), &(pop3->ind[k]));
    }
}

/* Routine to copy an individual 'ind1' into another individual 'ind2' */
void copy_ind(individual *ind1, individual *ind2)
{
    if (nreal != 0)
    {
        for (int i = 0; i < nreal; i++)
        {
            for (int j = 0; j < nvalues[i]; j++)
            {
                ind2->xreal[i][j] = ind1->xreal[i][j];
            }
        }
    }
    if (nbin != 0)
    {
        for (int i = 0; i < nbin; i++)
        {
            for (int j = 0; j < nbits[i]; j++)
            {
                ind2->gene[i][j] = ind1->gene[i][j];
            }
        }
    }
    for (int i = 0; i < nobj; i++)
    {
        ind2->obj[i] = ind1->obj[i];
    }
}
