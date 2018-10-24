#include "global.h"

std::default_random_engine generator;

/* Get seed number for random and start it up */
void init_rand(int seed)
{
    srand(seed);
}

/* Fetch a single random number between 0.0 and 1.0 */
float randomperc()
{
    float r = ((float)rand()) / RAND_MAX;
    return r;
}

/* Fetch a single random integer between low and high including the bounds */
int rnd(int low, int high)
{
    int r = low + rand() % (high - low + 1);
    return r;
}

/* Fetch a single random real number between low and high including the bounds */
float rndreal(float low, float high)
{
    return low + (high-low)*randomperc();
}

/* Fetch a single random real number from a normal distribution */
float rndnorm (float mean, float stddev)
{
    std::normal_distribution<float> distribution(mean, stddev);
    return distribution(generator);
}
