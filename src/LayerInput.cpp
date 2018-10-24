#include "includes.h"

/* constructor */
LayerInput::LayerInput(Params *params, Layer *prev_layer)
{
    prev_layer_ = prev_layer;

    type_ = "input";

    Assert(params->hasField("shape"), "Input Layer must have shape specified.");
    shape_ = params->getVectori("shape");

    forwardTensor_ = new Tensor(shape_, false);
}

/* destructor */
LayerInput::~LayerInput()
{
    delete forwardTensor_;
}

/* forward propagation */
void LayerInput::forward()
{
}

/* initialize weights */
void LayerInput::initWeights(DataType *weights, int &offset)
{
}

/* get number of weights in this layer */
int LayerInput::getNumWeights()
{
    return 0;
}
