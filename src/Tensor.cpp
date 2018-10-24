#include "includes.h"

/* constructor */
Tensor::Tensor(std::vector<int> shape, bool allocate)
{
    int num_elems = 1;
    for (int i = 0; i < shape.size(); i++)
    {
        num_elems *= shape[i];
    }

    if (allocate)
    {
        own_ = true;
        data_ = new DataType[num_elems];
    }
    else
    {
        own_ = false;
        data_ = NULL;
    }

    shape_ = shape;

    size_ = num_elems;

    if (shape_.size() == 2)
    {
        from_shape_1 = shape_[1];
    }
    else if (shape_.size() == 3)
    {
        from_shape_1 = shape_[1] * shape_[2];
        from_shape_2 = shape_[2];
    }
    else if (shape_.size() == 4)
    {
        from_shape_1 = shape_[1] * shape_[2] * shape_[3];
        from_shape_2 = shape_[2] * shape_[3];
        from_shape_3 = shape_[3];
    }
}

/* destructor */
Tensor::~Tensor()
{
    if (own_)
    {
        delete [] data_;
    }
}

/* get data element */
DataType &Tensor::data(int i)
{
    return data_[i];
}
DataType &Tensor::data(int i, int j)
{
    Assert(shape_.size() >= 2, "Tensor must has a dimension size larger then 2");
    return data_[i*from_shape_1+j];
}
DataType &Tensor::data(int i, int j, int k)
{
    Assert(shape_.size() >= 3, "Tensor must has a dimension size larger then 3");
    return data_[i*shape_[1]*from_shape_2+j*from_shape_2+k];
}
DataType &Tensor::data(int i, int j, int k, int l)
{
    Assert(shape_.size() >= 4, "Tensor must has a dimension size larger then 4");
    return data_[i*shape_[1]*shape_[2]*from_shape_3+j*shape_[2]*from_shape_3+k*from_shape_3+l];
}

/* set data pointer */
void Tensor::setPointer(DataType *data)
{
    if (own_)
    {
        delete [] data_;
    }
    own_ = false;
    data_ = data;
}

/* get data pointer */
DataType *Tensor::getPointer()
{
    return data_;
}

/* get shape */
std::vector<int> Tensor::shape()
{
    return shape_;
}

/* get total number of elements */
int Tensor::size()
{
    return size_;
}
