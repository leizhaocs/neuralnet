#include "includes.h"

/* constructor */
Net::Net()
{
    Encrypt::initEncrypt();
}

/* destructor */
Net::~Net()
{
    int layers_num = layers_.size();
    for (int i = 0; i < layers_num; ++i)
    {
        delete layers_[i];
    }
    layers_.clear();
}

/* read test data */
void Net::readData(DataType *data, DataType *labels, int n, int h, int w, int c, int classes)
{
    data_ = data;
    labels_ = labels;
    n_ = n;
    h_ = h;
    w_ = w;
    c_ = c;
    classes_ = classes;
}

/* initialize all the layers in the network */
void Net::initLayers(Params *layer_params, int layers_num)
{
    Params *layer_param = &(layer_params[0]);
    batchSize_ = layer_params[0].getVectori("shape")[0];
    std::string layer_type = layer_param->getString("type");
    layers_.resize(layers_num);
    layers_[0] = new LayerInput(layer_param, NULL);
    for (int i = 1; i < layers_num; i++)
    {
        Layer *prev_layer = layers_[i-1];
        layer_param = &(layer_params[i]);
        layer_type = layer_param->getString("type");
        if (layer_type == "convolution")
        {
            layers_[i] = new LayerConv(layer_param, prev_layer);
        }
        else if (layer_type == "pool")
        {
            layers_[i] = new LayerPool(layer_param, prev_layer);
        }
        else if (layer_type == "full")
        {
            layers_[i] = new LayerFull(layer_param, prev_layer);
        }
        else if (layer_type == "activation")
        {
            layers_[i] = new LayerAct(layer_param, prev_layer);
        }
        else if (layer_type == "batchnormalization")
        {
            layers_[i] = new LayerBN(layer_param, prev_layer);
        }
        else if (layer_type == "residual")
        {
            layers_[i] = new LayerRes(layer_param, prev_layer);
        }
        else if (layer_type == "concatenate")
        {
            layers_[i] = new LayerCat(layer_param, prev_layer);
        }
        else
        {
            Assert(false, layer_type + " - unknown type of the layer");
        }
    }
}

/* initialize weights for all layers */
void Net::initWeights(DataType *weights)
{
    int offset = 0;
    for (int i = 0; i < layers_.size(); i++)
    {
        layers_[i]->initWeights(weights, offset);
    }
}

/* get number of weights per layer, only return layers with weights, 0: real 1; binary */
std::vector<std::vector<int>> Net::getNumWeights()
{
    std::vector<std::vector<int>> total_weights;
    for (int i = 0; i < layers_.size(); i++)
    {
        int num_weights = layers_[i]->getNumWeights();

        if (layers_[i]->type_ == "convolution")
        {
            std::vector<int> temp;
#if BINARIZE == 0
            temp.push_back(0);
#elif BINARIZE == 1
            temp.push_back(1);
#endif
            temp.push_back(num_weights);
            total_weights.push_back(temp);
        }
        else if (layers_[i]->type_ == "full")
        {
            std::vector<int> temp;
#if BINARIZE == 0
            temp.push_back(0);
#elif BINARIZE == 1
            temp.push_back(1);
#endif
            temp.push_back(num_weights);
            total_weights.push_back(temp);
        }
        else if (layers_[i]->type_ == "batchnormalization")
        {
            std::vector<int> temp;
            temp.push_back(0);
            temp.push_back(num_weights);
            total_weights.push_back(temp);
        }
    }
    return total_weights;
}

/* inference */
void Net::classify(DataType *pred)
{
    int offset = 0;
    int numbatches = DIVUP(n_, batchSize_);
    for (int i = 0; i < numbatches; i++)
    {
        int batchsize = MIN(n_ - offset, batchSize_);
        DataType *X = data_ + offset*h_*w_*c_;
        DataType *y = pred + offset*classes_;
        initForward(X, y);
        forward();
        offset += batchsize;
    }
}

/* called before every forward propagation */
void Net::initForward(DataType *input, DataType *pred)
{
    layers_[0]->forwardTensor_->setPointer(input);
    layers_.back()->forwardTensor_->setPointer(pred);
}

/* forward propagate through all layers */
void Net::forward()
{
    for (int i = 0; i < layers_.size(); i++)
    {
        layers_[i]->forward();
    }
}
