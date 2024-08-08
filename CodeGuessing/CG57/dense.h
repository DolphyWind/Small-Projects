#ifndef __DENSE_H__
#define __DENSE_H__
#include "layer.h"

// N: Batch size
// C: Neuron count of the current layer
// D: Neuron count of the next layer
typedef struct {
    Layer base;
    size_t neuron_count;
    Matrix* weights; // Transposed. CxD matrix. 
    Matrix* neurons; // NxC matrix
    Matrix* before_activation; // NxC matrix
    Matrix* biasses; // 1xD vector
    cell_t dropout_probability;
} DenseLayer;

// input: NxD matrix
// result: NxC matrix
void dense_init_layer_neurons(Layer* l, size_t batch_size, int init_weights);
DenseLayer* dense_init(Layer* prev, Layer* next, size_t neuron_count, Activation* activation, cell_t dropout_probability);
void dense_free(Layer* dense);
Matrix* dense_forward(Layer* dense);
void dense_backward(Layer* dense, Matrix* gradient);
size_t dense_print_info(Layer* dense, FILE* stream);
Matrix* dense_get_neuron_matrix(Layer* layer);
Matrix* dense_get_weight_matrix(Layer* layer);
Matrix* dense_get_bias_matrix(Layer* layer);
Matrix* dense_get_before_activation_matrix(Layer* layer);

#endif
