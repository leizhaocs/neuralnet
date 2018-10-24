#include "includes.h"

/* constructor */
LayerFull::LayerFull(Params *params, Layer *prev_layer)
{
    prev_layer_ = prev_layer;

    type_ = "full";

    Assert(params->hasField("length"), "Full Layer must have length specified.");
    length_ = params->getScalari("length");

    int n = prev_layer_->shape_[0];
    shape_ = std::vector<int> {n, length_};

    forwardTensor_ = new Tensor(shape_, true);

    Assert(params->hasField("biases"), "Full Layer must have biases specified.");
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

    int prev_num_neurons = prev_layer_->forwardTensor_->size() / prev_layer_->shape_[0];
    std::vector<int> weights_shape = std::vector<int> {length_*prev_num_neurons};
    std::vector<int> biases_shape = std::vector<int> {length_};
    weights_ = new Tensor(weights_shape, false);
    if (has_biases_)
    {
        biases_ = new Tensor(biases_shape, false);
    }
}

/* destructor */
LayerFull::~LayerFull()
{
    delete forwardTensor_;
    delete weights_;
    if (has_biases_)
    {
        delete biases_;
    }
}

/* forward propagation */
void LayerFull::forward()
{
    int prev_num_neurons = prev_layer_->forwardTensor_->size() / prev_layer_->shape_[0];

    #pragma omp parallel for num_threads(OPENMP_THREADS) collapse(2)
    for (int n = 0; n < shape_[0]; n++)
    {
        for (int l = 0; l < shape_[1]; l++)
        {
#if ENCRYPT == 0
            if (has_biases_)
            {
                forwardTensor_->data(n, l) = biases_->data(l);
            }
            else
            {
                forwardTensor_->data(n, l) = 0;
            }
            for (int prev_l = 0; prev_l < prev_num_neurons; prev_l++)
            {
#if BINARIZE == 0
                forwardTensor_->data(n, l) += prev_layer_->forwardTensor_->data(n, prev_l) * weights_->data(prev_l*shape_[1]+l);
#elif BINARIZE == 1
                if (weights_->data(prev_l*shape_[1]+l) == 1)
                {
                    forwardTensor_->data(n, l) += prev_layer_->forwardTensor_->data(n, prev_l);
                }
                else if (weights_->data(prev_l*shape_[1]+l) == -1)
                {
                    forwardTensor_->data(n, l) -= prev_layer_->forwardTensor_->data(n, prev_l);
                }
                else
                {
                    Assert(false, "Wrong binary weights in fully connected layer.");
                }
#endif
            }
#elif ENCRYPT == 1
            std::vector<DataType> window;
            if (has_biases_)
            {
                window.emplace_back(biases_->data(l));
            }
            for (int prev_l = 0; prev_l < prev_num_neurons; prev_l++)
            {
                window.emplace_back(prev_layer_->forwardTensor_->data(n, prev_l) * weights_->data(prev_l*shape_[1]+l));
            }
            forwardTensor_->data(n, l).add_many(window);
#endif
        }
    }
}

/* initialize weights */
void LayerFull::initWeights(DataType *weights, int &offset)
{
    weights_->setPointer(weights+offset);
    offset += weights_->size();

    if (has_biases_)
    {
        biases_->setPointer(weights+offset);
        offset += biases_->size();
    }
}

/* get number of weights in this layer */
int LayerFull::getNumWeights()
{
    int num_weights = weights_->size();
    if (has_biases_)
    {
        num_weights += biases_->size();
    }
    return num_weights;
}
