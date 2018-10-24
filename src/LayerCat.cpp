#include "includes.h"

/* constructor */
LayerCat::LayerCat(Params *params, Layer *prev_layer)
{
    prev_layer_ = prev_layer;

    type_ = "concatenate";

    Assert(params->hasField("layer"), "concatenate Layer must have layer specified.");
    int offset = params->getScalari("layer");
    layer_ = this;
    for (int i = offset; i >= offset; i++)
    {
        layer_ = layer_->prev_layer_;
    }

    shape_ = prev_layer->shape_;
    Assert(shape_.size() == 2 || shape_.size() == 4, "concatenate Layer has wrong shape.");
    if (shape_.size() == 2)
    {
        Assert(layer_->shape_.size() == 2, "concatenate Layer has wrong shape.");
        shape_[1] += layer_->shape_[1];
    }
    else if (shape_.size() == 4)
    {
        Assert(layer_->shape_.size() == 4, "concatenate Layer has wrong shape.");
        shape_[3] += layer_->shape_[3];
    }

    forwardTensor_ = new Tensor(shape_, true);
}

/* destructor */
LayerCat::~LayerCat()
{
    delete forwardTensor_;
}

/* forward propagation */
void LayerCat::forward()
{
    if (shape_.size() == 2)
    {
        for (int i = 0; i < shape_[0]; i++)
        {
            int index = 0;
            for (int j = 0; j < prev_layer_->shape_[1]; j++, index++)
            {
                forwardTensor_->data(i, index) = prev_layer_->forwardTensor_->data(i, j);
            }
            for (int j = 0; j < layer_->shape_[1]; j++, index++)
            {
                forwardTensor_->data(i, index) = layer_->forwardTensor_->data(i, j);
            }
        }
    }
    else if (shape_.size() == 4)
    {
        for (int i = 0; i < shape_[0]; i++)
        {
            for (int j = 0; j < shape_[1]; j++)
            {
                for (int k = 0; k < shape_[2]; k++)
                {
                    int index = 0;
                    for (int l = 0; l < prev_layer_->shape_[3]; l++, index++)
                    {
                        forwardTensor_->data(i, j, k, index) = prev_layer_->forwardTensor_->data(i, j, k, l);
                    }
                    for (int l = 0; l < layer_->shape_[3]; l++, index++)
                    {
                        forwardTensor_->data(i, j, k, index) = layer_->forwardTensor_->data(i, j, k, l);
                    }
                }
            }
        }
    }
}

/* initialize weights */
void LayerCat::initWeights(DataType *weights, int &offset)
{
}

/* get number of weights in this layer */
int LayerCat::getNumWeights()
{
    return 0;
}
