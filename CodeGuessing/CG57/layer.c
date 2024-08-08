#include "layer.h"
#include "matrix.h"
#include "optimizer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

Layer* layer_init(Layer* prev, Layer* next, Activation* activation, LayerType type)
{
    Layer* l = malloc(sizeof(Layer));
    layer_set_params(l, prev, next, activation, type);;
    return l;
}

void layer_free(Layer *layer)
{
    layer->optimizer->free(layer->optimizer);
    free(layer);
}

void layer_set_params(Layer* l, Layer* prev, Layer* next, Activation* activation, LayerType type)
{
    // assert(activation != NULL);
    l->prev = prev;
    l->next = next;
    l->activation = activation;
    l->layer_type = type;

    l->forward_pass = layer_forward;
    l->backward_pass = layer_backward;
    l->init_layer_neurons = layer_init_neurons;
    l->print_info = layer_print_info;
    l->get_neuron_matrix = layer_get_neuron_matrix;
    l->get_weight_matrix = layer_get_weight_matrix;
    l->get_bias_matrix = layer_get_bias_matrix;
    l->free = layer_free;
    l->get_before_activation_matrix = layer_get_before_activation_marix;
}

void layer_init_neurons(Layer* l, size_t batch_size, int init_weights)
{
    assert(0);
}

Matrix* layer_forward(Layer* layer)
{
    assert(0);
}

void layer_backward(Layer* layer, Matrix* multipliers)
{
    assert(0);
}

size_t layer_print_info(Layer* l, FILE* stream)
{
    fprintf(stream, "This method should not be called!\n");
    return 0;
}

void layer_set_optimizer(Layer* l, Optimizer* o)
{
    Optimizer* l_opt = l->optimizer = optimizer_copy(o);
    l_opt->set_layer(l_opt, l);
}

Matrix* layer_get_neuron_matrix(Layer* layer)
{
    assert(0);
    return NULL;
}

Matrix* layer_get_weight_matrix(Layer* later)
{
    assert(0);
    return NULL;
}

Matrix* layer_get_bias_matrix(Layer* later)
{
    assert(0);
    return NULL;
}

Matrix* layer_get_before_activation_marix(Layer* layer)
{
    assert(0);
    return NULL;
}

Layer* layer_get_next_with_type(Layer* l, LayerType type)
{
    Layer* current = l->next;
    while(current)
    {
        if(current->layer_type == type)
        {
            return current;
        }

        current = current->next;
    }
    return NULL;
}

Layer* layer_get_prev_with_type(Layer* l, LayerType type)
{
    Layer* current = l->prev;
    while(current)
    {
        if(current->layer_type == type)
        {
            return current;
        }

        current = current->prev;
    }
    return NULL;
}

