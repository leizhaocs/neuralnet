#include "global.h"

/* Choose proper individuals for next generation */
void evolve(population *mixed_pop, population *parent_pop)
{
    int index_arr[2*popsize];
    float value_arr[2*popsize];
    for (int i = 0; i < 2*popsize; i++)
    {
        index_arr[i] = i;
        value_arr[i] = mixed_pop->ind[i].obj[0];
    }

    quickSort(index_arr, value_arr, 0, 2*popsize-1);

    for (int i = 0; i < 2*popsize; i++)
    {
        printf("%f  %d   %f\n", value_arr[i], index_arr[i], 1-mixed_pop->ind[index_arr[i]].obj[1]);
    }

    for (int i = 0; i < popsize; i++)
    {
        copy_ind(&(mixed_pop->ind[index_arr[i]]), &(parent_pop->ind[i]));
    }
}

/* Quick sorting */
void quickSort(int index_arr[], float value_arr[], int low, int high)
{
    if (low < high)
    {
        int pivot = partition(index_arr, value_arr, low, high);

        quickSort(index_arr, value_arr, low, pivot-1);
        quickSort(index_arr, value_arr, pivot+1, high);
    }
}

/* Intermidiate step in quick sorting */
int partition(int index_arr[], float value_arr[], int low, int high)
{
    float pivot = value_arr[high];
    int i = low - 1;
 
    for (int j = low; j <= high- 1; j++)
    {
        if (value_arr[j] <= pivot)
        {
            i++;
            float temp1 = value_arr[i];
            value_arr[i] = value_arr[j];
            value_arr[j] = temp1;
            int temp2 = index_arr[i];
            index_arr[i] = index_arr[j];
            index_arr[j] = temp2;
        }
    }
    i++;
    float temp1 = value_arr[i];
    value_arr[i] = value_arr[high];
    value_arr[high] = temp1;
    int temp2 = index_arr[i];
    index_arr[i] = index_arr[high];
    index_arr[high] = temp2;

    return i;
}
