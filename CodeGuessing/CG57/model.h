#ifndef __MODEL_H__
#define __MODEL_H__
#include "layer.h"
#include <stdio.h>

typedef void(*lossfunc_t)(Matrix*, Matrix*, Matrix*);

typedef struct Model {
    Layer* last;
    Layer* first;
    size_t layer_count;
    size_t batch_size;
    lossfunc_t loss_func;
    lossfunc_t loss_func_derivative;
} Model;

Model* model_init();
void model_free(Model* p);
void model_insert_layer(Model* m, Layer* l);
void model_add_dense(Model* m, size_t neuron_count, Activation* activation, cell_t dropout_probability);
void model_add_dropout(Model* m, cell_t dropout_probability);
void model_print_info(Model* m, FILE* stream);
void model_compile(Model* m, Optimizer* optimizer, lossfunc_t loss_func, lossfunc_t loss_derivative, size_t batch_size);
Matrix* __model_predict(Model* m, Matrix* in);
void model_predict(Model* m, Matrix* in, Matrix* out);
void model_fit(Model* m, Matrix* inputs, Matrix* outputs, size_t epochs);
void model_save(Model* m, const char* const filename);
void model_update_before_predict(Model* m, int is_predicting);

#endif
