#ifndef _LAYER_CAT_H__
#define _LAYER_CAT_H__

#include "includes.h"

/* concatenate layer */
class LayerCat : public Layer
{
public:
    /* constructor */
    LayerCat(Params *params, Layer *prev_layer);

    /* destructor */
    ~LayerCat();

    /* forward propagation */
    void forward();

    /* initialize weights */
    void initWeights(DataType *weights, int &offset);

    /* get number of weights in this layer */
    int getNumWeights();

private:
    Layer *layer_;  // the layer to concatenate
};

#endif
