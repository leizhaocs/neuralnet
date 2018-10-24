#include "includes.h"

/* constructor */
LayerPool::LayerPool(Params *params, Layer *prev_layer)
{
    prev_layer_ = prev_layer;

    type_ = "pool";

    Assert(params->hasField("poolType"), "Pool Layer must have pool type specified.");
    pool_type_ = params->getString("poolType");

    Assert(params->hasField("filterSize"), "Pool Layer must have filter size specified.");
    filterSize_ = params->getVectori("filterSize");

    Assert(params->hasField("stride"), "Pool Layer must have stride specified.");
    stride_ = params->getVectori("stride");

    int n = prev_layer->shape_[0];
    int h = (prev_layer_->shape_[1]-filterSize_[0])/stride_[0] + 1;
    int w = (prev_layer_->shape_[2]-filterSize_[1])/stride_[1] + 1;
    int c = prev_layer->shape_[3];
    shape_ = std::vector<int> {n, h, w, c};

    forwardTensor_ = new Tensor(shape_, true);
}

/* destructor */
LayerPool::~LayerPool()
{
    delete forwardTensor_;
}

/* forward propagation */
void LayerPool::forward()
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
                    if (pool_type_ == "scaledmean")
                    {
                        int start_h = h * stride_[0];
                        int start_w = w * stride_[1];
                        int end_h = start_h + filterSize_[0];
                        int end_w = start_w + filterSize_[1];
#if ENCRYPT == 0
                        DataType sum = 0;
                        for (int cur_h = start_h; cur_h < end_h; cur_h++)
                        {
                            for (int cur_w = start_w; cur_w < end_w; cur_w++)
                            {
                                sum += prev_layer_->forwardTensor_->data(n, cur_h, cur_w, c);
                            }
                        }
                        forwardTensor_->data(n, h, w, c) = sum;
#elif ENCRYPT == 1
                        std::vector<DataType> window;
                        for (int cur_h = start_h; cur_h < end_h; cur_h++)
                        {
                            for (int cur_w = start_w; cur_w < end_w; cur_w++)
                            {
                                window.emplace_back(prev_layer_->forwardTensor_->data(n, cur_h, cur_w, c));
                            }
                        }
                        forwardTensor_->data(n, h, w, c).add_many(window);
#endif
                    }
                    else if (pool_type_ == "max")
                    {
#if ENCRYPT == 0
                        int start_h = h * stride_[0];
                        int start_w = w * stride_[1];
                        int end_h = start_h + filterSize_[0];
                        int end_w = start_w + filterSize_[1];
                        DataType max = prev_layer_->forwardTensor_->data(n, start_h, start_w, c);
                        for (int cur_h = start_h; cur_h < end_h; cur_h++)
                        {
                            for (int cur_w = start_w; cur_w < end_w; cur_w++)
                            {
                                if (prev_layer_->forwardTensor_->data(n, cur_h, cur_w, c) > max)
                                max = prev_layer_->forwardTensor_->data(n, cur_h, cur_w, c);
                            }
                        }
                        forwardTensor_->data(n, h, w, c) = max;
#elif ENCRYPT == 1
                        Assert(false, "Max pooling is not support in encrypted version.");
#endif
                    }
                    else if (pool_type_ == "mean")
                    {
#if ENCRYPT == 0
                        int start_h = h * stride_[0];
                        int start_w = w * stride_[1];
                        int end_h = start_h + filterSize_[0];
                        int end_w = start_w + filterSize_[1];
                        DataType sum = 0;
                        for (int cur_h = start_h; cur_h < end_h; cur_h++)
                        {
                            for (int cur_w = start_w; cur_w < end_w; cur_w++)
                            {
                                sum += prev_layer_->forwardTensor_->data(n, cur_h, cur_w, c);
                            }
                        }
                        forwardTensor_->data(n, h, w, c) = sum / (filterSize_[0] * filterSize_[1]);
#elif ENCRYPT == 1
                        Assert(false, "Mean pooling is not support in encrypted version.");
#endif
                    }
                    else
                    {
                        Assert(false, "Unrecognized pooling function.");
                    }
                }
            }
        }
    }
}

/* initialize weights */
void LayerPool::initWeights(DataType *weights, int &offset)
{
}

/* get number of weights in this layer */
int LayerPool::getNumWeights()
{
    return 0;
}
