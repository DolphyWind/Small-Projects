#include "sgd.h"
#include "matrix.h"
#include "optimizer.h"
#include "layer.h"
#include "util.h"
#include <assert.h>

SGD* sgd_init(const char* name, cell_t lr, cell_t momentum, cell_t clip_norm)
{
    SGD* sgd = malloc(sizeof(SGD));
    optimizer_set_params((Optimizer*)sgd, name);
    sgd->base.object_size = sizeof(SGD);
    sgd->base.free = sgd_free;
    sgd->base.update = sgd_update;
    sgd->base.set_layer = sgd_set_layer;
    sgd->learning_rate = lr;
    sgd->momentum = momentum;
    sgd->clip_norm = clip_norm;

    return sgd;
}

void sgd_free(Optimizer* o)
{
    if(!o) return;

    SGD* sgd = (SGD*)o;
    free(o->name);
    matrix_free(sgd->weight_velocities);
    matrix_free(sgd->bias_velocities);
    free(sgd);
}

void sgd_update(Optimizer* o, Matrix* gradients, Matrix* activation_derivatives)
{
    // TODO: Add velocity
    SGD* sgd = (SGD*)o;
    cell_t learning_rate = sgd->learning_rate;
    cell_t clip_norm = sgd->clip_norm;
    size_t batch_size = gradients->rows;
    Layer* layer = o->layer;
    Matrix* weights = layer->get_weight_matrix(layer);
    Matrix* biasses = layer->get_bias_matrix(layer);
    Matrix* neurons = layer->get_neuron_matrix(layer);
    double max_gradient = 0.99;
    double max_delta = 0.98;

    matrix_clip_norm(gradients, clip_norm);

    for(size_t y = 0; y < batch_size; ++y)
    {
        for(size_t x = 0; x < gradients->cols; ++x)
        {
            cell_t gradient = matrix_at(gradients, y, x);
            gradient = clamp(gradient, -max_gradient, max_gradient);

            for(size_t wy = 0; wy < weights->rows; ++wy)
            {
                cell_t activation_derivative = matrix_at(activation_derivatives, y, wy);
                cell_t current_weight = matrix_at(weights, wy, x);
                cell_t current_neuron = matrix_at(neurons, y, wy);
                cell_t current_bias = matrix_at(biasses, 0, x);
                cell_t delta_weight = gradient * current_neuron * learning_rate * activation_derivative;
                // delta_weight = clamp(delta_weight, -max_delta, max_delta);
                
                current_weight -= delta_weight;
                matrix_set(weights, wy, x, current_weight);
            }
            cell_t bias = matrix_at(biasses, 0, x);
            cell_t delta_bias = gradient * learning_rate;
            // delta_bias = clamp(delta_bias, -max_delta, max_delta);
            bias -= delta_bias;
            matrix_set(biasses, 0, x, bias);
        }
    }
}

void sgd_set_layer(Optimizer* o, Layer* l)
{
    optimizer_set_layer(o, l);
    SGD* sgd = (SGD*)o;

    Matrix* weights = l->get_weight_matrix(l);
    if(weights)
    {
        sgd->weight_velocities = matrix_initn(weights->rows, weights->cols, 0);
    }
    Matrix* biasses = l->get_bias_matrix(l);
    if(biasses)
    {
        sgd->bias_velocities = matrix_initn(biasses->rows, biasses->cols, 0);
    }
}






