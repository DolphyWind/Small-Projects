#include "optimizer.h"
#include "layer.h"
#include <assert.h>
#include <string.h>

void optimizer_set_params(Optimizer* o, const char* name)
{
    o->object_size = sizeof(Optimizer);
    o->update = optimizer_update;
    o->free = optimizer_free;
    o->set_layer = optimizer_set_layer;
    size_t len = strlen(name);
    o->name = malloc(sizeof(char) * (len + 1));
    strcpy(o->name, name);
}

void optimizer_update(Optimizer* o, Matrix* gradients, Matrix* activation_derivatives)
{
    assert(0);
}

void optimizer_free(Optimizer* o)
{
    free(o->name);
    free(o);
}

void optimizer_set_layer(Optimizer* o, Layer* l)
{
    o->layer = l;
    l->optimizer = o;
}

Optimizer* optimizer_copy(Optimizer* o)
{
    Optimizer* new_optimizer = malloc(o->object_size);
    memcpy(new_optimizer, o, o->object_size);
    return new_optimizer;
}

