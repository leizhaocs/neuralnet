#include "global.h"

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop(population *pop)
{
    for (int i = 0; i < popsize; i++)
    {
        evaluate_ind(&(pop->ind[i]));
    }
}

/* Routine to evaluate objective function values and constraints for an individual */
void evaluate_ind(individual *ind)
{
    test_problem(ind->base_address, ind->xreal, ind->gene, ind->obj);
}
