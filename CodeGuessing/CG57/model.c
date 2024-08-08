#include "model.h"
#include "activation.h"
#include "dense.h"
#include "dropout.h"
#include "layer.h"
#include "matrix.h"
#include "optimizer.h"
#include "util.h"
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Model* model_init()
{
    Model* n = malloc(sizeof(Model));
    n->last = n->first = NULL;
    n->layer_count = 0;
    n->batch_size = 0;
    return n;
}

void model_free(Model* p)
{
    if(!p) return;

    Layer* current = p->last;
    while(current)
    {
        Layer* prev = current->prev;
        current->free(current);
        current = prev;
    }
    free(p);
}

void model_insert_layer(Model* m, Layer* l)
{
    ++m->layer_count;
    if(!m->first)
    {
        m->first = m->last = l;
        m->first->next = m->first->prev = NULL;
        return;
    }

    m->last->next = l;
    l->prev = m->last;
    l->next = NULL;
    m->last = m->last->next;
}

void model_add_dense(Model* m, size_t neuron_count, Activation* activation, cell_t dropout_probability)
{
    DenseLayer* dl = dense_init(NULL, NULL, neuron_count, activation, dropout_probability);
    model_insert_layer(m, (Layer*)dl);
}

void model_add_dropout(Model* m, cell_t dropout_probability)
{
    Dropout* d = dropout_init(NULL, NULL, dropout_probability);
    model_insert_layer(m, (Layer*)d);
}

void model_print_info(Model* m, FILE* stream)
{
    fprintf(stream, "A model with %zu layers.\n\n", m->layer_count);

    size_t i = 0;
    size_t total_params = 0;
    Layer* current = m->first;
    while(current)
    {
        total_params += current->print_info(current, stdout);

        current = current->next;
        ++i;
    }

    fprintf(stream, "Total parameters: %zu\n\n", total_params);
}

void model_compile(Model* m, Optimizer* optimizer, lossfunc_t loss_func, lossfunc_t loss_derivative, size_t batch_size)
{
    Layer* current = m->first;
    while(current)
    {
        current->init_layer_neurons(current, batch_size, 1);
        layer_set_optimizer(current, optimizer);
        current = current->next;
    }
    m->batch_size = batch_size;
    m->loss_func = loss_func;
    m->loss_func_derivative = loss_derivative;
}

Matrix* __model_predict(Model* m, Matrix* in)
{
    Layer* current = m->first;
    Matrix* neurons = current->get_neuron_matrix(current);
    
    matrix_copy(neurons, in);
    Matrix* out = in;

    while(current)
    {
        Matrix* temp = current->forward_pass(current);
        if(temp)
        {
            out = temp;
        }

        current = current->next;
    }
    return out;
}

void model_predict(Model* m, Matrix* in, Matrix* out)
{
    model_update_before_predict(m, 1);

    Layer* current = m->first;
    while(current)
    {
        current->init_layer_neurons(current, in->rows, 0);
        current = current->next;
    }

    Matrix* result = __model_predict(m, in);
    matrix_copy(out, result);
}

void model_fit(Model* m, Matrix* inputs, Matrix* outputs, size_t epochs)
{
    assert(inputs->rows == outputs->rows);

    model_update_before_predict(m, 0);
    int has_allocated = 0;
    size_t predictions_cols = m->last->get_neuron_matrix(m->last)->cols;

    if(outputs->cols == 1 && predictions_cols != 1)
    {
        Matrix* new_outputs = matrix_initn(outputs->rows, predictions_cols, 0);
        for(size_t y = 0; y < outputs->rows; ++y)
        {
            matrix_set(new_outputs, y, matrix_at(outputs, y, 0), 1);
        }
        outputs = new_outputs;
        has_allocated = 1;
    }

    size_t batch_size = m->batch_size;
    Matrix* batch = matrix_init(batch_size, inputs->cols);
    Matrix* batch_outputs = matrix_init(batch_size, outputs->cols);
    Matrix* loss_matrix = matrix_init(batch_size, 1);
    Matrix* gradients = matrix_initn(batch_size, outputs->cols, 1);
    size_t* indices = malloc(sizeof(size_t) * batch_size);

    for(size_t i = 0; i < epochs; ++i)
    {
        printf("Epoch: %zu ", i + 1);
        choose_n_indices(batch_size, inputs->rows, indices);

        for(size_t i = 0; i < batch_size; ++i)
        {
            size_t row = indices[i];
            
            matrix_copy_row(batch, inputs, i, row);
            matrix_copy_row(batch_outputs, outputs, i, row);
        }

        Matrix* predictions = __model_predict(m, batch);

        m->loss_func(predictions, batch_outputs, loss_matrix);
        cell_t mean_loss = matrix_col_mean(loss_matrix, 0);
        printf("Average loss: %lf\n", mean_loss);

        m->loss_func_derivative(predictions, batch_outputs, gradients);
        m->last->backward_pass(m->last, gradients);
    }
    matrix_free(batch);
    matrix_free(batch_outputs);
    matrix_free(gradients);
    matrix_free(loss_matrix);
    if(has_allocated)
    {
        matrix_free(outputs);
    }
    free(indices);
}

void model_save(Model* m, const char* const filename)
{
    // I was going to implement this but I am running out of time
    FILE* stream = fopen(filename, "wb");
    if(!stream)
    {
        fprintf(stderr, "Couldn't open file \"%s\"\n", filename);
        return;
    }

    fclose(stream);
}

void model_update_before_predict(Model* m, int is_predicting)
{
    Layer* current = m->first;
    while(current)
    {
        if(current->layer_type == DROPOUT)
        {
            Dropout* d = (Dropout*)current;
            d->is_predicting = is_predicting;
        }
        current = current->next;
    }
}

