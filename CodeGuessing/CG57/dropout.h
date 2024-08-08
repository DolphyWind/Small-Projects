// Unused because they won't work with the current design. Insted, I've added dropout_probability parameter to dense layers
#ifndef __DROPOUT_H__
#define __DROPOUT_H__

#include "layer.h"
typedef struct {
    Layer base;
    cell_t dropout_probability;
    int is_predicting;
} Dropout;

void dropout_init_layer_neurons(Layer* l, size_t batch_size, int init_weights);
Dropout* dropout_init(Layer* prev, Layer* next, cell_t dropout_probability);
void dropout_free(Layer* dropout);
Matrix* dropout_forward(Layer* dropout);
void dropout_backward(Layer* dropout, Matrix* gradient);
size_t dropout_print_info(Layer* dropout, FILE* stream);
Matrix* dropout_get_neuron_matrix(Layer* layer);
Matrix* dropout_get_weight_matrix(Layer* layer);
Matrix* dropout_get_bias_matrix(Layer* layer);
Matrix* dropout_get_before_activation_matrix(Layer* layer);


#endif
