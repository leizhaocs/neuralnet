#ifndef _TENSOR_H_
#define _TENSOR_H_

#include "includes.h"

/* tensor */
class Tensor
{
public:
    /* constructor */
    Tensor(std::vector<int> shape, bool allocate);

    /* destructor */
    ~Tensor();

    /* get data element */
    DataType &data(int i);
    DataType &data(int i, int j);
    DataType &data(int i, int j, int k);
    DataType &data(int i, int j, int k, int l);

    /* set data pointer */
    void setPointer(DataType *data);

    /* get data pointer */
    DataType *getPointer();

    /* get shape */
    std::vector<int> shape();

    /* get total number of elements */
    int size();

private:
    bool own_;                // true: data_ is allocated when creating this tensor, so need to free data_
    DataType *data_;          // raw data
    std::vector<int> shape_;  // shape of the tensor
    int size_;                // total number of elements
    int from_shape_1;         // shape_[1]*...
    int from_shape_2;         // shape_[2]*...
    int from_shape_3;         // shape_[3]*...
};

#endif
