#include "includes.h"
#include "global.h"

extern Net *net;
extern float *pred;
extern int num_train_images;
extern int num_classes;
extern float *train_label_ptr;

void test_problem(float *base_address, float **xreal, float **gene, float *obj)
{
    net->initWeights((DataType *)base_address);

    net->classify((DataType *)pred);

    obj[0] = 0;
    int correct = 0, wrong = 0;
    for (int i = 0; i < num_train_images; i++)
    {
        float train_max = 0, truth_max = 0;
        int train_digit = -1, truth_digit = -2;
        for (int j = 0; j < num_classes; j++)
        {
            float train_value = pred[i*num_classes+j];
            float truth_value = train_label_ptr[i*num_classes+j];
            obj[0] += (truth_value-train_value)*(truth_value-train_value);

            if (train_value > train_max)
            {
                train_max = train_value;
                train_digit = j;
            }
            if (truth_value > truth_max)
            {
                truth_max = truth_value;
                truth_digit = j;
            }
        }
        if (truth_digit == train_digit)
        {
            correct++;
        }
        else
        {
            wrong++;
        }
    }

    obj[1] = ((float)wrong) / (correct + wrong);
    printf("Correct: %d,  Wrong: %d,  loss: %f, Acc: %f\n", correct, wrong, obj[0], 1-obj[1]);
}