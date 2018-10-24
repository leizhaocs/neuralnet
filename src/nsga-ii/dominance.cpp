#include "global.h"

/* Routine for usual non-domination checking
   It will return the following values
   1 if a dominates b
   -1 if b dominates a
   0 if both a and b are non-dominated */
int check_dominance(individual *a, individual *b)
{
    if (a->obj[1] < b->obj[1])
    {
        return 1;
    }
    if (a->obj[1] > b->obj[1])
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
