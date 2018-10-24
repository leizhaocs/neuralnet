#ifndef _NET_H__
#define _NET_H__

#include "includes.h"

/* neural network */
class Net
{
public:
    /* constructor */
    Net();

    /* destructor */
    ~Net();

    /* read data */
    void readData(DataType *data, DataType *labels, int n, int h, int w, int c, int classes);

    /* initialize all the layers in the network */
    void initLayers(Params *layer_params, int layers_num);

    /* initialize weights for all layers */
    void initWeights(DataType *weights);

    /* get number of weights per layer, only return layers with weights, 0: real 1; binary */
    std::vector<std::vector<int>> getNumWeights();

    /* inference */
    void classify(DataType *pred);

private:
    /* called before every forward propagation */
    void initForward(DataType *input, DataType *pred);

    /* forward propagate through all layers */
    void forward();

    std::vector<Layer *> layers_;  // all the layers
    DataType *data_;               // (n,h,w,c)
    DataType *labels_;             // one hot vectors
    int n_;                        // number of samples in the dataset
    int h_;                        // height of input
    int w_;                        // width of input
    int c_;                        // channels of input
    int classes_;                  // number of classes
    int batchSize_;                // batch size
};

#endif
