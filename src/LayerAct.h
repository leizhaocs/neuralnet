#ifndef _LAYER_ACT_H__
#define _LAYER_ACT_H__

#include "includes.h"

/* activation layer */
class LayerAct : public Layer
{
public:
    /* constructor */
    LayerAct(Params *params, Layer *prev_layer);

    /* destructor */
    ~LayerAct();

    /* forward propagation */
    void forward();

    /* initialize weights */
    void initWeights(DataType *weights, int &offset);

    /* get number of weights in this layer */
    int getNumWeights();

private:
    std::string nonlinear_;  // activation type
};

#endif
