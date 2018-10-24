#ifndef _LAYER_CONV_H__
#define _LAYER_CONV_H__

#include "includes.h"

/* convolution layer */
class LayerConv : public Layer
{
public:
    /* constructor */
    LayerConv(Params *params, Layer *prev_layer);

    /* destructor */
    ~LayerConv();

    /* forward propagation */
    void forward();

    /* initialize weights */
    void initWeights(DataType *weights, int &offset);

    /* get number of weights in this layer */
    int getNumWeights();

private:
    std::vector<int> filterSize_;  // shape of filters_ (f,h,w)
    std::vector<int> stride_;      // stride (h,w)
    std::vector<int> padding_;     // padding (h,w)
    bool has_biases_;              // whether this layer has biases

    Tensor *filters_;   // filters (f,h,w,c)
    Tensor *biases_;    // biases (f)
};

#endif
