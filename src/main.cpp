#include "includes.h"
#include "global.h"

// data and network
Net *net;
int num_train_images = 0;
int num_test_images = 0;
int num_image_rows = 0;
int num_image_cols = 0;
int num_image_chls = 0;
int num_classes = 0;
DataType *train_data_ptr;
DataType *train_label_ptr;
DataType *test_data_ptr;
DataType *test_label_ptr;
Params *layers;
int num_layers;
DataType *weights_ptr;
DataType *pred;

// nsga-ii
int popsize;
int ngen;
int nobj;
int nreal;
int *nvalues;
float min_realvar;
float max_realvar;
float pcross_real;
float pmut_real;
int nbin;
int *nbits;
float pcross_bin;
float pmut_bin;

/* implemented in cpp files in data folder */
void load_mnist();
void load_yiliao();
void load_scope();

/* load network parameters from config file */
void build_network(const char *config)
{
    std::ifstream cfg(config, std::ifstream::in);

    std::string line;

    std::getline(cfg, line);
    num_layers = std::stoi(line);

    layers = new Params[num_layers];
    int layer_index = 0;

    while (std::getline(cfg, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        else if (line[0] == '[')
        {
            int pos = line.find(']');
            std::string layername = line.substr(1, pos-1);

            if (layername == "input")
            {
                layers[layer_index].addString("type", "input");

                while (true)
                {
                    std::getline(cfg, line);
                    if (line[0] == '#')
                    {
                        continue;
                    }
                    int pos = line.find('=');
                    if (pos == string::npos)
                    {
                        break;
                    }

                    std::string key = line.substr(0, pos-1);
                    if (key == "shape")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int n, h, w, c;
                        valuestream >> n;
                        valuestream >> h;
                        valuestream >> w;
                        valuestream >> c;
                        layers[layer_index].addVectori("shape", std::vector<int> {n, h, w, c});
                    }
                }
            }
            else if(layername == "convolution")
            {
                layers[layer_index].addString("type", "convolution");

                while (true)
                {
                    std::getline(cfg, line);
                    if (line[0] == '#')
                    {
                        continue;
                    }
                    int pos = line.find('=');
                    if (pos == string::npos)
                    {
                        break;
                    }

                    std::string key = line.substr(0, pos-1);
                    if (key == "filterSize")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int f, h, w;
                        valuestream >> f;
                        valuestream >> h;
                        valuestream >> w;
                        layers[layer_index].addVectori("filterSize", std::vector<int> {f, h, w});
                    }
                    else if (key == "stride")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int h, w;
                        valuestream >> h;
                        valuestream >> w;
                        layers[layer_index].addVectori("stride", std::vector<int> {h, w});
                    }
                    else if (key == "padding")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int h, w;
                        valuestream >> h;
                        valuestream >> w;
                        layers[layer_index].addVectori("padding", std::vector<int> {h, w});
                    }
                    else if (key == "biases")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        std::string biases;
                        valuestream >> biases;
                        layers[layer_index].addString("biases", biases.c_str());
                    }
                }
            }
            else if(layername == "pool")
            {
                layers[layer_index].addString("type", "pool");

                while (true)
                {
                    std::getline(cfg, line);
                    if (line[0] == '#')
                    {
                        continue;
                    }
                    int pos = line.find('=');
                    if (pos == string::npos)
                    {
                        break;
                    }

                    std::string key = line.substr(0, pos-1);
                    if (key == "poolType")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        std::string pooltype;
                        valuestream >> pooltype;
                        layers[layer_index].addString("poolType", pooltype.c_str());
                    }
                    else if (key == "filterSize")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int h, w;
                        valuestream >> h;
                        valuestream >> w;
                        layers[layer_index].addVectori("filterSize", std::vector<int> {h, w});
                    }
                    else if (key == "stride")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int h, w;
                        valuestream >> h;
                        valuestream >> w;
                        layers[layer_index].addVectori("stride", std::vector<int> {h, w});
                    }
                    else if (key == "padding")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int h, w;
                        valuestream >> h;
                        valuestream >> w;
                        layers[layer_index].addVectori("padding", std::vector<int> {h, w});
                    }
                }
            }
            else if(layername == "full")
            {
                layers[layer_index].addString("type", "full");

                while (true)
                {
                    std::getline(cfg, line);
                    if (line[0] == '#')
                    {
                        continue;
                    }
                    int pos = line.find('=');
                    if (pos == string::npos)
                    {
                        break;
                    }

                    std::string key = line.substr(0, pos-1);
                    if (key == "length")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int length;
                        valuestream >> length;
                        layers[layer_index].addScalari("length", length);
                    }
                    else if (key == "biases")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        std::string biases;
                        valuestream >> biases;
                        layers[layer_index].addString("biases", biases.c_str());
                    }
                }
            }
            else if(layername == "activation")
            {
                layers[layer_index].addString("type", "activation");

                while (true)
                {
                    std::getline(cfg, line);
                    if (line[0] == '#')
                    {
                        continue;
                    }
                    int pos = line.find('=');
                    if (pos == string::npos)
                    {
                        break;
                    }

                    std::string key = line.substr(0, pos-1);
                    if (key == "nonlinear")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        std::string nonlinear;
                        valuestream >> nonlinear;
                        layers[layer_index].addString("nonlinear", nonlinear.c_str());
                    }
                }
            }
            else if(layername == "batchnormalization")
            {
                layers[layer_index].addString("type", "batchnormalization");

                while (true)
                {
                    std::getline(cfg, line);
                    if (line[0] == '#')
                    {
                        continue;
                    }
                    int pos = line.find('=');
                    if (pos == string::npos)
                    {
                        break;
                    }

                    std::string key = line.substr(0, pos-1);
                    if (key == "channels")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int channels;
                        valuestream >> channels;
                        layers[layer_index].addScalari("channels", channels);
                    }
                }
            }
            else if(layername == "residual")
            {
                layers[layer_index].addString("type", "residual");

                while (true)
                {
                    std::getline(cfg, line);
                    if (line[0] == '#')
                    {
                        continue;
                    }
                    int pos = line.find('=');
                    if (pos == string::npos)
                    {
                        break;
                    }

                    std::string key = line.substr(0, pos-1);
                    if (key == "layer")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int layer;
                        valuestream >> layer;
                        layers[layer_index].addScalari("layer", layer);
                    }
                }
            }
            else if(layername == "concatenate")
            {
                layers[layer_index].addString("type", "concatenate");

                while (true)
                {
                    std::getline(cfg, line);
                    if (line[0] == '#')
                    {
                        continue;
                    }
                    int pos = line.find('=');
                    if (pos == string::npos)
                    {
                        break;
                    }

                    std::string key = line.substr(0, pos-1);
                    if (key == "layer")
                    {
                        std::string value = line.substr(pos+1, std::string::npos);
                        std::stringstream valuestream(value);
                        int layer;
                        valuestream >> layer;
                        layers[layer_index].addScalari("layer", layer);
                    }
                }
            }
            layer_index++;
            if (layer_index >= num_layers)
            {
                break;
            }
        }
    }

    cfg.close();
}

/* load weights weights file */
void load_weights(const char *weights)
{
    std::ifstream wghts(weights, std::ifstream::in);
    int num_weights;
    wghts >> num_weights;

#if ENCRYPT == 0
    weights_ptr = new DataType[num_weights];
    for (int i = 0; i < num_weights; i++)
    {
        wghts >> weights_ptr[i];
    }
#elif ENCRYPT == 1
    weights_ptr = new DataType[num_weights];
    for (int i = 0; i < num_weights; i++)
    {
        float temp;
        wghts >> temp;
        weights_ptr[i].set_value(temp);
        weights_ptr[i].set_use_plain(true);
    }
#endif

    wghts.close();
}

/* initialize NSGA-II */
void nsga_ii(std::vector<std::vector<int>> total_weights)
{
    printf("\n Enter the population size (a multiple of 4) : ");
    scanf("%d", &popsize);
    if (popsize < 4 || (popsize%4) != 0)
    {
        printf("\n population size read is : %d", popsize);
        printf("\n Wrong population size entered, hence exiting \n");
        exit(1);
    }

    printf("\n Enter the number of generations : ");
    scanf("%d", &ngen);
    if (ngen < 1)
    {
        printf("\n number of generations read is : %d", ngen);
        printf("\n Wrong nuber of generations entered, hence exiting \n");
        exit(1);
    }

    printf("\n Enter the number of objectives : ");
    scanf("%d", &nobj);
    if (nobj < 1)
    {
        printf("\n number of objectives entered is : %d", nobj);
        printf("\n Wrong number of objectives entered, hence exiting \n");
        exit(1);
    }

    nreal = 0;
    nbin = 0;
    for (int i = 0; i < total_weights.size(); i++)
    {
        if (total_weights[i][0] == 0)
        {
            nreal++;
        }
        else
        {
            nbin++;
        }
    }
    nvalues = (int *)malloc(nreal*sizeof(int));
    nbits = (int *)malloc(nbin*sizeof(int));
    for (int i = 0, nvalues_index = 0, nbits_index = 0; i < total_weights.size(); i++)
    {
        if (total_weights[i][0] == 0)
        {
            nvalues[nvalues_index++] = total_weights[i][1];
        }
        else
        {
            nbits[nbits_index++] = total_weights[i][1];
        }
    }

    printf("\n Enter the lower limit of real variable : ");
    scanf("%f", &min_realvar);
    printf("\n Enter the upper limit of real variable : ");
    scanf("%f", &max_realvar);
    if (max_realvar <= min_realvar)
    {
        printf("\n Wrong limits entered for the min and max bounds of real variable, hence exiting \n");
        exit(1);
    }

    printf("\n Enter the probability of crossover of real variable (0.0-1.0) : ");
    scanf("%f", &pcross_real);
    if (pcross_real < 0.0 || pcross_real > 1.0)
    {
        printf("\n Probability of crossover entered is : %e", pcross_real);
        printf("\n Entered value of probability of crossover of real variables is out of bounds, hence exiting \n");
        exit(1);
    }
    printf("\n Enter the probablity of mutation of real variables  (0.0-1.0) : ");
    scanf("%f", &pmut_real);
    if (pmut_real < 0.0 || pmut_real > 1.0)
    {
        printf("\n Probability of mutation entered is : %e", pmut_real);
        printf("\n Entered value of probability of mutation of real variables is out of bounds, hence exiting \n");
        exit(1);
    }

    printf("\n Enter the probability of crossover of binary variable (0.0-1.0): ");
    scanf("%f", &pcross_bin);
    if (pcross_bin < 0.0 || pcross_bin > 1.0)
    {
        printf("\n Probability of crossover entered is : %e", pcross_bin);
        printf("\n Entered value of probability of crossover of binary variables is out of bounds, hence exiting \n");
        exit(1);
    }
    printf("\n Enter the probability of mutation of binary variables (0.0-1.0): ");
    scanf("%f", &pmut_bin);
    if (pmut_bin < 0.0 || pmut_bin > 1.0)
    {
        printf("\n Probability of mutation entered is : %e", pmut_bin);
        printf("\n Entered value of probability  of mutation of binary variables is out of bounds, hence exiting \n");
        exit(1);
    }

    if (nreal == 0 && nbin == 0)
    {
        printf("\n Number of real as well as binary variables, both are zero, hence exiting \n");
        exit(1);
    }

    printf("\n NSGA-II initialization Done \n");
}

/* main function */
int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout<<"Usage:"<<std::endl;
        std::cout<<"    Train mode: ./nn train <dataset name> <network cfg file> < <nsga-ii cfg file>"<<std::endl;
        std::cout<<"    Test  mode: ./nn test  <dataset name> <network cfg file> <weights file>"<<std::endl;
        exit(0);
    }

    net = new Net();

    std::cout<<"Loading data..."<<std::endl;

    if (strcmp(argv[2], "mnist") == 0)
    {
        load_mnist();
    }
    else if (strcmp(argv[2], "yiliao") == 0)
    {
        load_yiliao();
    }
    else if (strcmp(argv[2], "scope") == 0)
    {
        load_scope();
    }
    else
    {
        Assert(false, "Unsupported dataset.");
    }

    std::cout<<"Building network..."<<std::endl;

    build_network(argv[3]);
    net->initLayers(layers, num_layers);

    if (strcmp(argv[1], "train") == 0)
    {
        Assert(ENCRYPT == 0, "Currently only support non-encrypted training.");

        std::cout<<"Setting data..."<<std::endl;

        net->readData(train_data_ptr, train_label_ptr, num_train_images, num_image_rows, num_image_cols, num_image_chls, num_classes);
        pred = new DataType[num_train_images*num_classes];

        std::cout<<"Reading NSGA-II parameters..."<<std::endl;

        std::vector<std::vector<int>> total_weights = net->getNumWeights();
        nsga_ii(total_weights);

        std::cout<<"Initializing NSGA-II..."<<std::endl;

        population *parent_pop = (population *)malloc(sizeof(population));
        population *child_pop = (population *)malloc(sizeof(population));
        population *mixed_pop = (population *)malloc(sizeof(population));

        allocate_memory_pop(parent_pop, popsize, total_weights);
        allocate_memory_pop(child_pop, popsize, total_weights);
        allocate_memory_pop(mixed_pop, 2*popsize, total_weights);
        init_rand(0);
        initialize_pop(parent_pop);

        std::cout<<"Training..."<<std::endl;

        evaluate_pop(parent_pop);
        printf("\n gen = 1\n");
        for (int i = 2; i <= ngen; i++)
        {
            selection(parent_pop, child_pop);
            mutation_pop(child_pop);
            evaluate_pop(child_pop);
            merge(parent_pop, child_pop, mixed_pop);
            evolve(mixed_pop, parent_pop);
            printf("\n gen = %d\n", i);
        }

        deallocate_memory_pop(parent_pop, popsize);
        deallocate_memory_pop(child_pop, popsize);
        deallocate_memory_pop(mixed_pop, 2*popsize);
        free(parent_pop);
        free(child_pop);
        free(mixed_pop);

        if (nreal != 0)
        {
            free(nvalues);
        }
        if (nbin != 0)
        {
            free(nbits);
        }
    }
    else if (strcmp(argv[1], "test") == 0)
    {
        Assert((BINARIZE == 0) || (ENCRYPT == 0 && BINARIZE == 1), "Currently binarized testing on encrypted data is not supported.");

        std::cout<<"Setting data..."<<std::endl;

        net->readData(test_data_ptr, test_label_ptr, num_test_images, num_image_rows, num_image_cols, num_image_chls, num_classes);
        pred = new DataType[num_test_images*num_classes];

        std::cout<<"Loading weights..."<<std::endl;

        load_weights(argv[4]);
        net->initWeights(weights_ptr);

        std::cout<<"Classifying..."<<std::endl;

        float startTime = omp_get_wtime();
        net->classify(pred);
        float endTime = omp_get_wtime();
        printf("Total time: %f s\n", endTime - startTime);

        std::cout<<"Prediction result:"<<std::endl;

        int correct = 0, wrong = 0;
        for (int i = 0; i < num_test_images; i++)
        {
            DataType test_max = 0, max = 0;
            int test_digit = -1, digit = -2;
            for (int j = 0; j < 10; j++)
            {
                if (pred[i*10+j] > test_max)
                {
                    test_max = pred[i*10+j];
                    test_digit = j;
                }
                if (test_label_ptr[i*10+j] > max)
                {
                    max = test_label_ptr[i*10+j];
                    digit = j;
                }
            }
            if (digit == test_digit)
            {
                correct++;
            }
            else
            {
                wrong++;
            }
        }
        std::cout<<"Correct: "<<correct<<" Wrong: "<<wrong<<std::endl;
    }
    else
    {
        Assert(false, "Neither train nor test");
    }

    delete [] layers;
    delete [] weights_ptr;
    delete [] pred;
    delete [] train_data_ptr;
    delete [] train_label_ptr;
    delete [] test_data_ptr;
    delete [] test_label_ptr;
    delete net;

    return 0;
}
