#ifndef _LAYER_BN_H__
#define _LAYER_BN_H__

#include "includes.h"

/* batch normalization layer */
class LayerBN : public Layer
{
public:
    /* constructor */
    LayerBN(Params *params, Layer *prev_layer);

    /* destructor */
    ~LayerBN();

    /* forward propagation */
    void forward();

    /* initialize weights */
    void initWeights(DataType *weights, int &offset);

    /* get number of weights in this layer */
    int getNumWeights();

private:
    int channels_;      // the second dimension of input

    Tensor *mean_;      // mean
    Tensor *beta_;      // beta
    Tensor *gamma_std_; // gamma / std
};

#endif
