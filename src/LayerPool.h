#ifndef _LAYER_POOL_H__
#define _LAYER_POOL_H__

#include "includes.h"

class LayerPool : public Layer
{
public:
    /* constructor */
    LayerPool(Params *params, Layer *prev_layer);

    /* destructor */
    ~LayerPool();

    /* forward propagation */
    void forward();

    /* initialize weights */
    void initWeights(DataType *weights, int &offset);

    /* get number of weights in this layer */
    int getNumWeights();

private:
    std::string pool_type_;  // pooling type

    std::vector<int> filterSize_;  // shape of filters_ (h,w)
    std::vector<int> stride_;      // stride (h,w)
};

#endif
