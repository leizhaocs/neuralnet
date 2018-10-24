#include "global.h"

/* Function to allocate memory to a population */
void allocate_memory_pop(population *pop, int size, std::vector<std::vector<int>> total_weights)
{
    pop->ind = (individual *)malloc(size*sizeof(individual));
    for (int i = 0; i < size; i++)
    {
        allocate_memory_ind(&(pop->ind[i]), total_weights);
    }
}

/* Function to allocate memory to an individual */
void allocate_memory_ind(individual *ind, std::vector<std::vector<int>> total_weights)
{
    int total = 0;
    for (int i = 0; i < nreal; i++)
    {
        total += nvalues[i];
    }
    for (int i = 0; i < nbin; i++)
    {
        total += nbits[i];
    }
    ind->base_address = (float *)malloc(total*sizeof(float));
    ind->xreal = (float **)malloc(nreal*sizeof(float *));
    ind->gene = (float **)malloc(nbin*sizeof(float *));

    float *ptr = ind->base_address;
    for (int i = 0, xreal_index = 0, gene_index = 0; i < total_weights.size(); i++)
    {
        if (total_weights[i][0] == 0)
        {
            ind->xreal[xreal_index++] = ptr;
            ptr += total_weights[i][1];
        }
        else
        {
            ind->gene[gene_index++] = ptr;
            ptr += total_weights[i][1];
        }
    }

    ind->obj = (float *)malloc(nobj*sizeof(float));
}

/* Function to deallocate memory to a population */
void deallocate_memory_pop(population *pop, int size)
{
    for (int i = 0; i < size; i++)
    {
        deallocate_memory_ind(&(pop->ind[i]));
    }
    free(pop->ind);
}

/* Function to deallocate memory to an individual */
void deallocate_memory_ind(individual *ind)
{
    free(ind->base_address);
    free(ind->xreal);
    free(ind->gene);
    free(ind->obj);
}
