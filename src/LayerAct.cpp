#include "includes.h"

/* constructor */
LayerAct::LayerAct(Params *params, Layer *prev_layer)
{
    prev_layer_ = prev_layer;

    type_ = "activation";

    shape_ = prev_layer->shape_;

    forwardTensor_ = new Tensor(shape_, true);

    Assert(params->hasField("nonlinear"), "Activation Layer must have nonlinear specified.");
    nonlinear_ = params->getString("nonlinear");
}

/* destructor */
LayerAct::~LayerAct()
{
    delete forwardTensor_;
}

/* forward propagation */
void LayerAct::forward()
{
    if (nonlinear_ == "square")
    {
        #pragma omp parallel for num_threads(OPENMP_THREADS)
        for (int i = 0; i < prev_layer_->forwardTensor_->size(); i++)
        {
#if ENCRYPT == 0
            forwardTensor_->data(i) = prev_layer_->forwardTensor_->data(i) * prev_layer_->forwardTensor_->data(i);
#elif ENCRYPT == 1
            forwardTensor_->data(i) = prev_layer_->forwardTensor_->data(i).square();
#endif
        }
    }
    else if (nonlinear_ == "relu")
    {
#if ENCRYPT == 0
        #pragma omp parallel for num_threads(OPENMP_THREADS)
        for (int i = 0; i < prev_layer_->forwardTensor_->size(); i++)
        {
            if (prev_layer_->forwardTensor_->data(i) < 0)
            {
                forwardTensor_->data(i) = 0;
            }
            else
            {
                forwardTensor_->data(i) = prev_layer_->forwardTensor_->data(i);
            }
        }
#elif ENCRYPT == 1
        Assert(false, "ReLU is not support in encrypted version.");
#endif
    }
    else if (nonlinear_ == "leakyrelu")
    {
#if ENCRYPT == 0
        #pragma omp parallel for num_threads(OPENMP_THREADS)
        for (int i = 0; i < prev_layer_->forwardTensor_->size(); i++)
        {
            if (prev_layer_->forwardTensor_->data(i) < 0)
            {
                forwardTensor_->data(i) = prev_layer_->forwardTensor_->data(i) * 0.1;
            }
            else
            {
                forwardTensor_->data(i) = prev_layer_->forwardTensor_->data(i);
            }
        }
#elif ENCRYPT == 1
        Assert(false, "LeakyReLU is not support in encrypted version.");
#endif
    }
    else if (nonlinear_ == "softmax")
    {
        Assert(shape_.size() == 2, "Softmax can only has two dimensions.");
#if ENCRYPT == 0
        #pragma omp parallel for num_threads(OPENMP_THREADS)
        for (int n = 0; n < shape_[0]; n++)
        {
            DataType sum = 0;
            for (int i = 0; i < shape_[1]; i++)
            {
                sum += exp(prev_layer_->forwardTensor_->data(n, i));
            }
            for (int i = 0; i < shape_[1]; i++)
            {
                forwardTensor_->data(n, i) = exp(prev_layer_->forwardTensor_->data(n, i)) / sum;
            }
        }
#elif ENCRYPT == 1
        Assert(false, "Softmax is not support in encrypted version.");
#endif
    }
    else
    {
        Assert(false, "Unsupported nonlinear function.");
    }
}

/* initialize weights */
void LayerAct::initWeights(DataType *weights, int &offset)
{
}

/* get number of weights in this layer */
int LayerAct::getNumWeights()
{
    return 0;
}
