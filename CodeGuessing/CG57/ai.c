#include <stdio.h>
#include <time.h>
#include <math.h>
#include "activation.h"
#include "loss.h"
#include "matrix.h"
#include "model.h"
#include "optimizer.h"
#include "sgd.h"
#include "util.h"

int main(int argc, char** argv)
{
    srand(time(NULL));
    Activation* act_id = activation_init("identity", identity, one);
    Activation* act_relu = activation_init("relu", relu, relu_derivative);
    Activation* act_softmax = activation_init("softmax", softmax, softmax_derivative);
    Activation* act_sigmoid = activation_init("sigmoid", sigmoid, sigmoid_derivative);
    SGD* sgd = sgd_init("SGD", 0.001, 0, 1);
    const size_t BATCH_SIZE = 16;

    // if you are lucky it gets up to 60% accuracy, but it is 45-50% accurate most of the time.
    // which is terrible for a neural network, perhaps ive done something wrong in the implementation.
    // it somehow got 85% accuracy once.
    Model* m = model_init();
    model_add_dense(m, 784, act_relu, 0.33);
    model_add_dense(m, 128, act_relu, 0.33);
    model_add_dense(m, 64, act_softmax, 0);
    model_add_dense(m, 10, act_id, 0);
    model_compile(m, (Optimizer*)sgd, mse, mse_derivative, BATCH_SIZE);
    
    const size_t example_count = 60000;
    const size_t test_count = 10000;
    const size_t pixel_count = 784;
    const size_t epochs = 300;

    Matrix* in = matrix_init(example_count, pixel_count);
    Matrix* test_in = matrix_init(test_count, pixel_count);
    Matrix* out = matrix_init(example_count, 1);
    Matrix* test_out = matrix_init(test_count, 1);

    FILE* mnist = fopen("mnist_train.csv", "r");
    for(size_t y = 0; y < example_count; ++y)
    {
        fscanf(mnist, "%lf", matrix_atp(out, y, 0));
        for(size_t x = 0; x < pixel_count; ++x)
        {
            fscanf(mnist, "%lf", matrix_atp(in, y, x));
            *matrix_atp(in, y, x) /= 255.0;
        }
    }
    fclose(mnist);
    printf("Loaded the MNIST dataset!\n");

    FILE* mnist_test = fopen("mnist_test.csv", "r");
    for(size_t y = 0; y < test_count; ++y)
    {
        fscanf(mnist_test, "%lf", matrix_atp(test_out, y, 0));
        for(size_t x = 0; x < pixel_count; ++x)
        {
            fscanf(mnist, "%lf", matrix_atp(test_in, y, x));
            *matrix_atp(test_in, y, x) /= 255.0;
        }
    }
    fclose(mnist_test);
    printf("Loaded tests!\n");

    model_fit(m, in, out, epochs);
    printf("Fit done!\n");

    Matrix* prediction = matrix_init(test_count, 10);
    size_t accuracy = 0;

    model_predict(m, test_in, prediction);
    for(size_t y = 0; y < test_count; ++y)
    {
        if(matrix_at(prediction, y, matrix_at(test_out, y, 0)) > 0.5)
        {
            ++accuracy;
        }
    }
    matrix_free(prediction);
    printf("Model accuracy: %lf\n", (cell_t)accuracy / (cell_t)test_count);

    model_free(m);
    matrix_free(test_in);
    matrix_free(out);
    matrix_free(test_out);
    activation_free(act_id);
    activation_free(act_relu);
    activation_free(act_softmax);
    activation_free(act_sigmoid);
    sgd->base.free((Optimizer*)sgd);
    return 0;
}
