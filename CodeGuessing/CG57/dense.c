#include "dense.h"
#include "activation.h"
#include "layer.h"
#include "matrix.h"
#include "util.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

void dense_init_layer_neurons(Layer* l, size_t batch_size, int init_weights)
{
    if(!l) return;
    DenseLayer* this = (DenseLayer*)l;
    size_t N = batch_size;
    size_t C = this->neuron_count;
    this->neurons = matrix_initn(N, C, 0);
    this->before_activation = matrix_initn(N, C, 0);

    if(!init_weights) return;

    DenseLayer* next = (DenseLayer*)layer_get_next_with_type(l, DENSE);
    if(!next) return;

    size_t D = next->neuron_count;

    cell_t factor = sqrt(3.0 / C);
    this->weights = matrix_initrf(C, D, 1);
    this->biasses = matrix_initn(1, D, 0);
}

DenseLayer* dense_init(Layer* prev, Layer* next, size_t neuron_count, Activation* activation, cell_t dropout_probability)
{
    assert(activation != NULL);

    DenseLayer* dense = malloc(sizeof(DenseLayer));
    
    dense->neuron_count = neuron_count;
    dense->dropout_probability = dropout_probability;
    dense->weights = NULL;
    dense->neurons = NULL;
    dense->before_activation = NULL;
    dense->biasses = NULL;
    layer_set_params(&dense->base, prev, next, activation, DENSE);

    dense->base.forward_pass = dense_forward;
    dense->base.backward_pass = dense_backward;
    dense->base.init_layer_neurons = dense_init_layer_neurons;
    dense->base.print_info = dense_print_info;
    dense->base.get_neuron_matrix = dense_get_neuron_matrix;
    dense->base.get_weight_matrix = dense_get_weight_matrix;
    dense->base.get_bias_matrix = dense_get_bias_matrix;
    dense->base.free = dense_free;
    dense->base.get_before_activation_matrix = dense_get_before_activation_matrix;

    return dense;
}

void dense_free(Layer* layer)
{
    if(!layer) return;
    DenseLayer* dense = (DenseLayer*)layer;

    matrix_free(dense->weights);
    matrix_free(dense->biasses);
    matrix_free(dense->neurons);
    matrix_free(dense->before_activation);
    free(dense);
}

Matrix* dense_forward(Layer* dense)
{
    DenseLayer* dl = (DenseLayer*)dense;
    if(!dense->next)
    {
        activation_apply(dl->base.activation, dl->before_activation, dl->neurons);
        return dl->neurons;
    }

    // DenseLayer* next = (DenseLayer*)layer_get_next_with_type(dense, DENSE);
    DenseLayer* next = (DenseLayer*)dense->next;

    Matrix* next_bef_act = next->base.get_before_activation_matrix((Layer*)next);
    Matrix* next_neurons = next->base.get_neuron_matrix((Layer*)next);
    if(!next_bef_act || !next_neurons)
    {
        return NULL; // Should be unreachable but I added it just in case
    }
    
    for(size_t y = 0; y < dl->neurons->rows; ++y)
    {
        for(size_t x = 0; x < dl->neurons->cols; ++x)
        {
            if(get_random_bounded(0, 1) < dl->dropout_probability)
            {
                matrix_set(dl->neurons, y, x, 0);
            }
        }
    }
    
    matrix_mul(dl->neurons, dl->weights, next_bef_act);
    matrix_add_matrix(next_bef_act, dl->biasses, next_bef_act);
    activation_apply(dl->base.activation, next_bef_act, next_neurons);

    return next_neurons;
}

void dense_backward(Layer* dense, Matrix* gradients)
{
    DenseLayer* d = (DenseLayer*)(dense);
    Matrix* neurons = d->neurons;
    Matrix* nba = d->before_activation;
    Matrix* weights = d->weights;
    Matrix* biasses = d->biasses;
    size_t batch_size = gradients->rows;

    Matrix* new_gradients = gradients;
    if(!dense->next)
    {
        Matrix* activation_derivatives = matrix_init(neurons->rows, neurons->cols);
        dense->activation->derivative(nba, activation_derivatives);
        matrix_cwise_mul(gradients, activation_derivatives, gradients);
        matrix_free(activation_derivatives);
        goto end;
    }

    Matrix* activation_derivatives = matrix_init(neurons->rows, neurons->cols);
    dense->activation->derivative(nba, activation_derivatives);

    new_gradients = matrix_init(batch_size, neurons->cols);
    matrix_batch_mul(weights, gradients, new_gradients);
    matrix_cwise_mul(new_gradients, activation_derivatives, new_gradients);

    dense->optimizer->update(dense->optimizer, gradients, activation_derivatives);
    matrix_free(activation_derivatives);
end:
    if(dense->prev) dense->prev->backward_pass(dense->prev, new_gradients);
    if(new_gradients != gradients) matrix_free(new_gradients);
}

size_t dense_print_info(Layer* dense, FILE* stream)
{
    DenseLayer* dl = (DenseLayer*)dense;
    fprintf(stream, "A dense layer with %zu neurons.\n", dl->neuron_count);

    size_t rows = 0;
    size_t cols = 0;
    size_t biasses = 0;
    if(dl->weights)
    {
        rows = dl->weights->rows;
        cols = dl->weights->cols;
        biasses = dl->biasses->cols;
        
        fprintf(stream, "Parameter count: %zu*%zu+%zu=%zu\n",
                rows, cols, biasses, rows*cols + biasses);
    }
    fprintf(stream, "With activation function named \"%s\"\n\n", dl->base.activation->name);
    return (rows*cols + biasses);
}

Matrix* dense_get_neuron_matrix(Layer* layer)
{
    return ((DenseLayer*)layer)->neurons;
}

Matrix* dense_get_weight_matrix(Layer* dense)
{
    return ((DenseLayer*)dense)->weights;
}

Matrix* dense_get_bias_matrix(Layer* dense)
{
    return ((DenseLayer*)dense)->biasses;
}

Matrix* dense_get_before_activation_matrix(Layer* layer)
{
    return ((DenseLayer*)layer)->before_activation;
}


