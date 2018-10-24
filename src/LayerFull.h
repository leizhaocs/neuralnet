#ifndef _LAYER_FULL_H__
#define _LAYER_FULL_H__

#include "includes.h"

/* fully connected layer */
class LayerFull : public Layer
{
public:
    /* constructor */
    LayerFull(Params *params, Layer *prev_layer);

    /* destructor */
    ~LayerFull();

    /* forward propagation */
    void forward();

    /* initialize weights */
    void initWeights(DataType *weights, int &offset);

    /* get number of weights in this layer */
    int getNumWeights();

private:
    int length_;        // number of neurons
    bool has_biases_;   // whether this layer has biases

    Tensor *weights_;   // weights (prev_layer_->size() * length_)
    Tensor *biases_;    // biases (length_)
};

#endif
