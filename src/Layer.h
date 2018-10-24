#ifndef _LAYER_H__
#define _LAYER_H__

#include "includes.h"

/* layer base class */
class Layer
{
public:
    /* constructor */
    Layer();

    /* destructor */
    virtual ~Layer();

    /* forward propagation */
    virtual void forward() = 0;

    /* initialize weights */
    virtual void initWeights(DataType *weights, int &offset) = 0;

    /* get number of weights in this layer */
    virtual int getNumWeights() = 0;

    Layer *prev_layer_;           // previous layer
    std::string type_;            // layer type
    std::vector<int> shape_;      // shape of forwardTensor_ (n,h,w,c)
    Tensor *forwardTensor_;       // (n,h,w,c)
};

#endif
