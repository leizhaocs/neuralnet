#include "includes.h"

/* constructor */
LayerBN::LayerBN(Params *params, Layer *prev_layer)
{
    prev_layer_ = prev_layer;

    type_ = "batchnormalization";

    Assert(params->hasField("channels"), "Batch Normalization Layer must have channels specified.");
    channels_ = params->getScalari("channels");

    shape_ = prev_layer->shape_;

    forwardTensor_ = new Tensor(shape_, true);

    std::vector<int> bn_shape = std::vector<int> {channels_};
    mean_ = new Tensor(bn_shape, false);
    beta_ = new Tensor(bn_shape, false);
    gamma_std_ = new Tensor(bn_shape, false);
}

/* destructor */
LayerBN::~LayerBN()
{
    delete forwardTensor_;
    delete mean_;
    delete beta_;
    delete gamma_std_;
}

/* forward propagation */
void LayerBN::forward()
{
    if (shape_.size() == 2)
    {
        #pragma omp parallel for num_threads(OPENMP_THREADS) collapse(2)
        for (int n = 0; n < shape_[0]; n++)
        {
            for (int l = 0; l < shape_[1]; l++)
            {
                forwardTensor_->data(n, l) = prev_layer_->forwardTensor_->data(n, l);
                forwardTensor_->data(n, l) -= mean_->data(l);
                forwardTensor_->data(n, l) *= gamma_std_->data(l);
                forwardTensor_->data(n, l) += beta_->data(l);
            }
        }
    }
    else if (shape_.size() == 4)
    {
        #pragma omp parallel for num_threads(OPENMP_THREADS) collapse(4)
        for (int n = 0; n < shape_[0]; n++)
        {
            for (int h = 0; h < shape_[1]; h++)
            {
                for (int w = 0; w < shape_[2]; w++)
                {
                    for (int c = 0; c < shape_[3]; c++)
                    {
                        forwardTensor_->data(n, h, w, c) = prev_layer_->forwardTensor_->data(n, h, w, c);
                        forwardTensor_->data(n, h, w, c) -= mean_->data(c);
                        forwardTensor_->data(n, h, w, c) *= gamma_std_->data(c);
                        forwardTensor_->data(n, h, w, c) += beta_->data(c);
                    }
                }
            }
        }
    }
    else
    {
        Assert(false, "Wrong size in Batch Normalization.");
    }
}

/* initialize weights */
void LayerBN::initWeights(DataType *weights, int &offset)
{
    mean_->setPointer(weights+offset);
    offset += mean_->size();

    beta_->setPointer(weights+offset);
    offset += beta_->size();

    gamma_std_->setPointer(weights+offset);
    offset += gamma_std_->size();
}

/* get number of weights in this layer */
int LayerBN::getNumWeights()
{
    int num_weights = mean_->size() + beta_->size() + gamma_std_->size();
    return num_weights;
}
