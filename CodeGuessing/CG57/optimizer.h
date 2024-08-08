#ifndef __OPTIMIZER_H__
#define __OPTIMIZER_H__
#include "matrix.h"

struct Layer;

typedef struct Optimizer {
    struct Layer* layer;
    char* name;
    size_t object_size;
    void(*set_layer)(struct Optimizer* o, struct Layer* l);
    void(*update)(struct Optimizer* o, Matrix* gradients, Matrix* activation_derivatives);
    void(*free)(struct Optimizer*);
} Optimizer;

void optimizer_set_params(Optimizer* o, const char* name);
void optimizer_update(struct Optimizer* o, Matrix* gradients, Matrix* activation_derivatives);
void optimizer_free(Optimizer* o);
void optimizer_set_layer(Optimizer* o, struct Layer* l);
Optimizer* optimizer_copy(Optimizer* o);

#endif
