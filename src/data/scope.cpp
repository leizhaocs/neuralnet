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

/* read data */
DataType *readData(const char *filename, int &number_of_images, int &n_rows, int &n_cols, int &n_chls)
{
    std::ifstream file (filename, std::ios::binary);
    if (file.is_open())
    {
        int magic_number = 0;

        file.read((char*)&magic_number, sizeof(magic_number));
        file.read((char*)&number_of_images, sizeof(number_of_images));
        file.read((char*)&n_rows, sizeof(n_rows));
        file.read((char*)&n_cols, sizeof(n_cols));
        file.read((char*)&n_chls, sizeof(n_chls));

        DataType *ptr = new DataType[number_of_images*n_rows*n_cols*n_chls];
        for (int i = 0; i < number_of_images; i++)
        {
            for (int r = 0; r < n_rows; r++)
            {
                for (int c = 0; c < n_cols; c++)
                {
                    for (int h = 0; h < n_chls; h++)
                    {
                        unsigned char temp = 0;
                        file.read((char*)&temp, sizeof(temp));
                        ptr[i*n_rows*n_cols*n_chls+r*n_cols*n_chls+c*n_chls+h] = (float)temp/256;
                    }
                }
            }
        }
        return ptr;
    }
    return NULL;
}

/* read labels */
DataType *readLabels(const char *filename, int &number_of_labels, int &n_classes)
{
    std::ifstream file (filename, std::ios::binary);
    if (file.is_open())
    {
        int magic_number = 0;

        file.read((char*)&magic_number, sizeof(magic_number));
        file.read((char*)&number_of_labels, sizeof(number_of_labels));
        file.read((char*)&n_classes, sizeof(n_classes));

        DataType *ptr = new DataType[number_of_labels*n_classes];
        for (int i = 0; i < number_of_labels; i++)
        {
            unsigned char temp = 0;
            file.read((char*)&temp, sizeof(temp));
            for (int j = 0; j < n_classes; j++)
            {
                if (j == temp)
                {
                    ptr[i*n_classes+j] = 1;
                }
                else
                {
                    ptr[i*n_classes+j] = 0;
                }
            }
        }
        return ptr;
    }
    return NULL;
}

/* load training and test data */
void load_scope()
{
    train_data_ptr = readData("data/scope/data", num_train_images, num_image_rows, num_image_cols, num_image_chls);
    train_label_ptr = readLabels("data/scope/labels", num_train_images, num_classes);
    test_data_ptr = readData("data/scope/data", num_test_images, num_image_rows, num_image_cols, num_image_chls);
    test_label_ptr = readLabels("data/scope/labels", num_test_images, num_classes);
}