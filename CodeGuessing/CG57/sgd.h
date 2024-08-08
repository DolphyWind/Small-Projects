#ifndef __SGD_H__
#define __SGD_H__
#include "optimizer.h"

typedef struct {
    Optimizer base;
    cell_t learning_rate;
    cell_t momentum;
    cell_t clip_norm;
    Matrix* weight_velocities;
    Matrix* bias_velocities;
} SGD;

SGD* sgd_init(const char* name, cell_t lr, cell_t momentum, cell_t clip_norm);
void sgd_free(Optimizer* sgd);
void sgd_update(Optimizer* o, Matrix* gradients, Matrix* activation_derivatives);
void sgd_set_layer(Optimizer* o, struct Layer* l);

#endif
