// Unused because they won't work with the current design. Insted, I've added dropout_probability parameter to dense layers
#include "dropout.h"
#include "matrix.h"
#include "util.h"
#include <assert.h>

void dropout_init_layer_neurons(Layer* l, size_t batch_size, int init_weights)
{
}

Dropout* dropout_init(Layer* prev, Layer* next, cell_t dropout_probability)
{
    Dropout* d = malloc(sizeof(Dropout));

    layer_set_params(&d->base, prev, next, NULL, DROPOUT);
    d->dropout_probability = dropout_probability;
    d->base.forward_pass = dropout_forward;
    d->base.backward_pass = dropout_backward;
    d->base.init_layer_neurons = dropout_init_layer_neurons;
    d->base.print_info = dropout_print_info;
    d->base.get_neuron_matrix = dropout_get_neuron_matrix;
    d->base.get_weight_matrix = dropout_get_weight_matrix;
    d->base.get_bias_matrix = dropout_get_bias_matrix;
    d->base.get_before_activation_matrix = dropout_get_before_activation_matrix;
    d->base.free = dropout_free;

    return d;
}

void dropout_free(Layer* dropout)
{
    Dropout* d = (Dropout*)dropout;
    free(d);
}

Matrix* dropout_forward(Layer* dropout)
{
    Dropout* d = (Dropout*)dropout;
    Matrix* next_before_activaton = dropout->next->get_before_activation_matrix(dropout->next);

    if(d->is_predicting) return next_before_activaton;
    
    for(size_t y = 0; y < next_before_activaton->rows; ++y)
    {
        for(size_t x = 0; x < next_before_activaton->cols; ++x)
        {
            if(get_random_bounded(0.0, 1.0) < d->dropout_probability)
            {
                matrix_set(next_before_activaton, y, x, 0);
            }
        }
    }
    return next_before_activaton;
}

void dropout_backward(Layer* dropout, Matrix* gradient)
{
    return dropout->prev->backward_pass(dropout->prev, gradient);
}

size_t dropout_print_info(Layer* dropout, FILE* stream)
{
    Dropout* d = (Dropout*)dropout;
    fprintf(stream, "A dropout layer with dropout probability of %lf\n\n", d->dropout_probability);
    return 0;
}

Matrix* dropout_get_neuron_matrix(Layer* layer)
{
    return NULL;
}

Matrix* dropout_get_weight_matrix(Layer* layer)
{
    return NULL;
}

Matrix* dropout_get_bias_matrix(Layer* layer)
{
    return NULL;
}

Matrix* dropout_get_before_activation_matrix(Layer* layer)
{
    return NULL;
}
