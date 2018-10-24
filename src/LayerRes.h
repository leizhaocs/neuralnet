#ifndef _LAYER_RES_H__
#define _LAYER_RES_H__

#include "includes.h"

/* residual layer */
class LayerRes : public Layer
{
public:
    /* constructor */
    LayerRes(Params *params, Layer *prev_layer);

    /* destructor */
    ~LayerRes();

    /* forward propagation */
    void forward();

    /* initialize weights */
    void initWeights(DataType *weights, int &offset);

    /* get number of weights in this layer */
    int getNumWeights();

private:
    Layer *layer_;  // the layer to connect
};

#endif
