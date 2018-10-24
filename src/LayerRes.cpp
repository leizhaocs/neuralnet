#include "includes.h"

/* constructor */
LayerRes::LayerRes(Params *params, Layer *prev_layer)
{
    prev_layer_ = prev_layer;

    type_ = "residual";

    shape_ = prev_layer->shape_;

    forwardTensor_ = new Tensor(shape_, true);

    Assert(params->hasField("layer"), "Residual Layer must have layer specified.");
    int offset = params->getScalari("layer");
    layer_ = this;
    for (int i = offset; i >= offset; i++)
    {
        layer_ = layer_->prev_layer_;
    }
}

/* destructor */
LayerRes::~LayerRes()
{
    delete forwardTensor_;
}

/* forward propagation */
void LayerRes::forward()
{
    for (int i = 0; i < forwardTensor_->size(); i++)
    {
        forwardTensor_->data(i) = prev_layer_->forwardTensor_->data(i) + layer_->forwardTensor_->data(i);
    }
}

/* initialize weights */
void LayerRes::initWeights(DataType *weights, int &offset)
{
}

/* get number of weights in this layer */
int LayerRes::getNumWeights()
{
    return 0;
}
