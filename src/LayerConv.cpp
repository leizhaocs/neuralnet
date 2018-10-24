#include "includes.h"

/* constructor */
LayerConv::LayerConv(Params *params, Layer *prev_layer)
{
    prev_layer_ = prev_layer;

    type_ = "convolution";

    Assert(params->hasField("filterSize"), "Convolutional Layer must have filter size specified.");
    filterSize_ = params->getVectori("filterSize");

    Assert(params->hasField("stride"), "Convolutional Layer must have stride specified.");
    stride_ = params->getVectori("stride");

    Assert(params->hasField("padding"), "Convolutional Layer must have padding specified.");
    padding_ = params->getVectori("padding");

    int n = prev_layer_->shape_[0];
    int h = (prev_layer_->shape_[1]-filterSize_[1]+2*padding_[0])/stride_[0] + 1;
    int w = (prev_layer_->shape_[2]-filterSize_[2]+2*padding_[1])/stride_[1] + 1;
    int c = filterSize_[0];
    shape_ = std::vector<int> {n, h, w, c};

    forwardTensor_ = new Tensor(shape_, true);

    Assert(params->hasField("biases"), "Convolutional Layer must have biases specified.");
    std::string biases = params->getString("biases");
    if (biases == "true")
    {
        has_biases_ = true;
    }
    else if (biases == "false")
    {
        has_biases_ = false;
    }
    else
    {
        Assert(false, "Wrong biases parameter in config.");
    }

    std::vector<int> filter_shape = std::vector<int> {filterSize_[0], filterSize_[1], filterSize_[2], prev_layer_->shape_[3]};
    std::vector<int> biases_shape = std::vector<int> {filterSize_[0]};
    filters_ = new Tensor(filter_shape, false);
    if (has_biases_)
    {
        biases_ = new Tensor(biases_shape, false);
    }
}

/* destructor */
LayerConv::~LayerConv()
{
    delete forwardTensor_;
    delete filters_;
    if (has_biases_)
    {
        delete biases_;
    }
}

/* forward propagation */
void LayerConv::forward()
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
                    int start_h = h * stride_[0] - padding_[0];
                    int start_w = w * stride_[1] - padding_[1];
                    int start_c = 0;
                    int end_h = start_h + filterSize_[1];
                    int end_w = start_w + filterSize_[2];
                    int end_c = start_c + prev_layer_->shape_[3];

#if ENCRYPT == 0
                    if (has_biases_)
                    {
                        forwardTensor_->data(n, h, w, c) = biases_->data(c);
                    }
                    else
                    {
                        forwardTensor_->data(n, h, w, c) = 0;
                    }
                    for (int cur_h = start_h, filter_h = 0; cur_h < end_h; cur_h++, filter_h++)
                    {
                        for (int cur_w = start_w, filter_w = 0; cur_w < end_w; cur_w++, filter_w++)
                        {
                            for (int cur_c = start_c; cur_c < end_c; cur_c++)
                            {
                                if (cur_w < 0 || cur_h < 0)
                                {
                                }
                                else
                                {
#if BINARIZE == 0
                                    forwardTensor_->data(n, h, w, c) += filters_->data(c, filter_h, filter_w, cur_c) * prev_layer_->forwardTensor_->data(n, cur_h, cur_w, cur_c);
#elif BINARIZE == 1
                                    if (filters_->data(c, filter_h, filter_w, cur_c) == 1)
                                    {
                                        forwardTensor_->data(n, h, w, c) += prev_layer_->forwardTensor_->data(n, cur_h, cur_w, cur_c);
                                    }
                                    else if (filters_->data(c, filter_h, filter_w, cur_c) == -1)
                                    {
                                        forwardTensor_->data(n, h, w, c) -= prev_layer_->forwardTensor_->data(n, cur_h, cur_w, cur_c);
                                    }
                                    else
                                    {
                                        Assert(false, "Wrong binary weights in convolutional layer.");
                                    }
#endif
                                }
                            }
                        }
                    }
#elif ENCRYPT == 1
                    std::vector<DataType> window;
                    if (has_biases_)
                    {
                        window.emplace_back(biases_->data(c));
                    }
                    for (int cur_h = start_h, filter_h = 0; cur_h < end_h; cur_h++, filter_h++)
                    {
                        for (int cur_w = start_w, filter_w = 0; cur_w < end_w; cur_w++, filter_w++)
                        {
                            for (int cur_c = start_c; cur_c < end_c; cur_c++)
                            {
                                if (cur_w < 0 || cur_h < 0)
                                {
                                }
                                else
                                {
                                    window.emplace_back(filters_->data(c, filter_h, filter_w, cur_c) * prev_layer_->forwardTensor_->data(n, cur_h, cur_w, cur_c));
                                }
                            }
                        }
                    }
                    forwardTensor_->data(n, h, w, c).add_many(window);
#endif
                }
            }
        }
    }
}

/* initialize weights */
void LayerConv::initWeights(DataType *weights, int &offset)
{
    filters_->setPointer(weights+offset);
    offset += filters_->size();

    if (has_biases_)
    {
        biases_->setPointer(weights+offset);
        offset += biases_->size();
    }
}

/* get number of weights in this layer */
int LayerConv::getNumWeights()
{
    int num_weights = filters_->size();
    if (has_biases_)
    {
        num_weights += biases_->size();
    }
    return num_weights;
}
