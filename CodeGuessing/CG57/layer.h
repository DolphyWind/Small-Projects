#ifndef __LAYER_H__
#define __LAYER_H__
#include "matrix.h"
#include "activation.h"
#include "optimizer.h"
#include <stdio.h>

typedef enum
{
    DENSE,
    DROPOUT,
    COUNT
} LayerType;

typedef struct Layer {
    struct Layer* prev;
    struct Layer* next;
    Activation* activation;
    LayerType layer_type;
    Optimizer* optimizer;

    Matrix*(*forward_pass)(struct Layer*);
    void(*backward_pass)(struct Layer*, Matrix*);
    void(*init_layer_neurons)(struct Layer*, size_t, int);
    size_t(*print_info)(struct Layer*, FILE*);
    Matrix*(*get_neuron_matrix)(struct Layer*);
    Matrix*(*get_weight_matrix)(struct Layer*);
    Matrix*(*get_bias_matrix)(struct Layer*);
    Matrix*(*get_before_activation_matrix)(struct Layer*);
    void(*free)(struct Layer*);
} Layer;

Layer* layer_init(Layer* prev, Layer* next, Activation* activation, LayerType type);
void layer_free(Layer* layer);
void layer_set_params(Layer* l, Layer* prev, Layer* next, Activation* activation, LayerType type);
void layer_init_neurons(Layer* l, size_t batch_size, int init_weights);
Matrix* layer_forward(Layer* layer);
void layer_backward(Layer* layer, Matrix* multipliers);
size_t layer_print_info(Layer* l, FILE* stream);
void layer_set_optimizer(Layer* l, Optimizer* o);
Matrix* layer_get_neuron_matrix(Layer* layer);
Matrix* layer_get_weight_matrix(Layer* layer);
Matrix* layer_get_bias_matrix(Layer* layer);
Matrix* layer_get_before_activation_marix(Layer* layer);
Layer* layer_get_next_with_type(Layer* l, LayerType type);
Layer* layer_get_prev_with_type(Layer* l, LayerType type);

#endif
