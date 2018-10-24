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

/* read data and labels */
void ReadDataLabels(const char *filename)
{
    std::string s;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        printf("\n Error open file\n");
        exit(1);
    }

    train_data_ptr = new DataType[num_train_images*num_image_rows*num_image_cols];
    train_label_ptr = new DataType[num_train_images*num_classes];
    test_data_ptr = new DataType[num_test_images*num_image_rows*num_image_cols];
    test_label_ptr = new DataType[num_test_images*num_classes];

    for (int i = 0; i < 58; i++)
    {
        getline(file, s, ',');
    }
    getline(file, s, '\n');

    for (int i = 0; i < num_train_images; i++)
    {
        for (int j = 0; j < num_image_rows*num_image_cols; j++)
        {
            if (j < 58)
            {
                getline(file, s, ',');
                train_data_ptr[i*num_image_rows*num_image_cols+j] = (float)atof(s.c_str());
            }
            else
            {
                train_data_ptr[i*num_image_rows*num_image_cols+j] = 0;
            }
        }
        getline(file, s, '\n');
        int label = atoi(s.c_str());
        if (label == 0)
        {
            train_label_ptr[i*num_classes] = 1;
            train_label_ptr[i*num_classes+1] = 0;
        }
        else
        {
            train_label_ptr[i*num_classes] = 0;
            train_label_ptr[i*num_classes+1] = 1;
        }
    }
    for (int i = 0; i < num_test_images; i++)
    {
        for (int j = 0; j < num_image_rows*num_image_cols; j++)
        {
            if (j < 58)
            {
                getline(file, s, ',');
                test_data_ptr[i*num_image_rows*num_image_cols+j] = (float)atof(s.c_str());
            }
            else
            {
                test_data_ptr[i*num_image_rows*num_image_cols+j] = 0;
            }
        }
        getline(file, s, '\n');
        int label = atoi(s.c_str());
        if (label == 0)
        {
            test_label_ptr[i*num_classes] = 1;
            test_label_ptr[i*num_classes+1] = 0;
        }
        else
        {
            test_label_ptr[i*num_classes] = 0;
            test_label_ptr[i*num_classes+1] = 1;
        }
    }
}

/* load training and test data */
void load_yiliao()
{
    num_train_images = 6838;
    num_test_images = 1710;
    num_image_rows = 8;
    num_image_cols = 8; 
    num_image_chls = 1;
    num_classes = 2;

    ReadDataLabels("data/yiliao/data.csv");
}