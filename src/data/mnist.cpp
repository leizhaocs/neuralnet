#include "includes.h"

extern int num_train_images;
extern int num_test_images;
extern int num_image_rows;
extern int num_image_cols;
extern int num_image_chls;
extern int num_classes;
extern DataType *train_data_ptr;
extern DataType *train_label_ptr;
extern DataType *test_data_ptr;
extern DataType *test_label_ptr;

/* change from little endian to big endian */
int reverseInt (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 0xff;
    ch2 = (i>>8) & 0xff;
    ch3 = (i>>16) & 0xff;
    ch4 = (i>>24) & 0xff;
    return ((int)ch1<<24) + ((int)ch2<<16) + ((int)ch3<<8) + ch4;
}

/* read data */
DataType *readData(const char *filename, int &number_of_images, int &n_rows, int &n_cols)
{
    std::ifstream file (filename, std::ios::binary);
    if (file.is_open())
    {
        int magic_number = 0;

        file.read((char*)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);
        file.read((char*)&number_of_images, sizeof(number_of_images));
        number_of_images = 1;//reverseInt(number_of_images);
        file.read((char*)&n_rows, sizeof(n_rows));
        n_rows = reverseInt(n_rows);
        file.read((char*)&n_cols, sizeof(n_cols));
        n_cols = reverseInt(n_cols);

        DataType *ptr = new DataType[number_of_images*n_rows*n_cols];
        for (int i = 0; i < number_of_images; i++)
        {
            for (int r = 0; r < n_rows; r++)
            {
                for (int c = 0; c < n_cols; c++)
                {
                    unsigned char temp = 0;
                    file.read((char*)&temp, sizeof(temp));
                    ptr[i*n_rows*n_cols+r*n_cols+c] = (float)temp/256;
                }
            }
        }
        return ptr;
    }
    return NULL;
}

/* read labels */
DataType *readLabels(const char *filename, int &number_of_labels)
{
    std::ifstream file (filename, std::ios::binary);
    if (file.is_open())
    {
        int magic_number = 0;

        file.read((char*)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);
        file.read((char*)&number_of_labels, sizeof(number_of_labels));
        number_of_labels = 1;//reverseInt(number_of_labels);

        DataType *ptr = new DataType[number_of_labels*10];
        for (int i = 0; i < number_of_labels; i++)
        {
            unsigned char temp = 0;
            file.read((char*)&temp, sizeof(temp));
            for (int j = 0; j < 10; j++)
            {
                if (j == temp)
                {
                    ptr[i*10+j] = 1;
                }
                else
                {
                    ptr[i*10+j] = 0;
                }
            }
        }
        return ptr;
    }
    return NULL;
}

/* load training and test data */
void load_mnist()
{
    train_data_ptr = readData("data/mnist/train-images.idx3-ubyte", num_train_images, num_image_rows, num_image_cols);
    train_label_ptr = readLabels("data/mnist/train-labels.idx1-ubyte", num_train_images);
    test_data_ptr = readData("data/mnist/t10k-images.idx3-ubyte", num_test_images, num_image_rows, num_image_cols);
    test_label_ptr = readLabels("data/mnist/t10k-labels.idx1-ubyte", num_test_images);

    num_image_chls = 1;
    num_classes = 10;
}