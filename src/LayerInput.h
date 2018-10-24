#ifndef _LAYER_INPUT_H__
#define _LAYER_INPUT_H__

#include "includes.h"

/* input layer */
class LayerInput : public Layer
{
public:
    /* constructor */
    LayerInput(Params *params, Layer *prev_layer);

    /* destructor */
    ~LayerInput();

    /* forward propagation */
    void forward();

    /* initialize weights */
    void initWeights(DataType *weights, int &offset);

    /* get number of weights in this layer */
    int getNumWeights();
};

#endif
