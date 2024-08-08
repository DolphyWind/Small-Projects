#include "activation.h"
#include "matrix.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Activation* activation_init(char* name, activation_t func, derivative_t derivative)
{
    Activation* p = malloc(sizeof(Activation));
    size_t len = strlen(name);
    p->name = malloc(sizeof(char) * (len + 1));
    strcpy(p->name, name);
    p->func = func;
    p->derivative = derivative;

    return p;
}

void activation_free(Activation* p)
{
    if(!p) return;

    free(p->name);
    free(p);
}

void activation_apply(Activation* a, Matrix* in, Matrix* out)
{
    a->func(in, out);
}


void identity(Matrix* in, Matrix* out)
{
    assert(in->rows == out->rows && in->cols == out->cols);
    matrix_copy(out, in);
}

void one(Matrix* in, Matrix* out)
{
    assert(in->rows == out->rows && in->cols == out->cols);

    for(size_t y = 0; y < out->rows; ++y)
    {
        for(size_t x = 0; x < out->cols; ++x)
        {
            matrix_set(out, y, x, 1);
        }
    }
}

void relu(Matrix* in, Matrix* out)
{
    assert(in->rows == out->rows && in->cols == out->cols);

    for(size_t y = 0; y < in->rows; ++y)
    {
        for(size_t x = 0; x < in->cols; ++x)
        {
            cell_t current = matrix_at(in, y, x);
            matrix_set(out, y, x, (current < 0 ? 0 : current));
        }
    }
}

void relu_derivative(Matrix* in, Matrix* out)
{
    assert(in->rows == out->rows && in->cols == out->cols);

    for(size_t y = 0; y < in->rows; ++y)
    {
        for(size_t x = 0; x < in->cols; ++x)
        {
            cell_t current = matrix_at(in, y, x);
            matrix_set(out, y, x, (current <= 0 ? 0 : 1));
        }
    }
}

void softmax(Matrix* in, Matrix* out)
{
    assert(in->rows == out->rows && in->cols == out->cols);

    size_t rows = in->rows;
    size_t cols = in->cols;
    for(size_t y = 0; y < rows; ++y)
    {
        cell_t exp_sum = 0;
        for(size_t x = 0; x < cols; ++x)
        {
            exp_sum += exp(matrix_at(in, y, x));
        }

        for(size_t x = 0; x < cols; ++x)
        {
            matrix_set(out, y, x, exp(matrix_at(in, y, x)) / exp_sum);
        }
    }
}

void softmax_derivative(Matrix* in, Matrix* out)
{
    assert(in->rows == out->rows && in->cols == out->cols);

    size_t rows = in->rows;
    size_t cols = in->cols;
    softmax(in, out);

    for(size_t y = 0; y < rows; ++y)
    {
        for(size_t x = 0; x < cols; ++x)
        {
            cell_t current = matrix_at(out, y, x);
            matrix_set(out, y, x, current * (1 - current));
        }
    }
}

void sigmoid(Matrix *in, Matrix *out)
{
    assert(in->rows == out->rows && in->cols == out->cols);

    for(size_t y = 0; y < in->rows; ++y)
    {
        for(size_t x = 0; x < in->cols; ++x)
        {
            cell_t current = matrix_at(in, y, x);
            cell_t sigm = 1.0 / (1.0 + exp(-current));
            matrix_set(out, y, x, sigm);
        }
    }
}

void sigmoid_derivative(Matrix *in, Matrix *out)
{
    assert(in->rows == out->rows && in->cols == out->cols);

    for(size_t y = 0; y < in->rows; ++y)
    {
        for(size_t x = 0; x < in->cols; ++x)
        {
            cell_t current = matrix_at(in, y, x);
            cell_t sigm_d = exp(-current) / (pow(1+exp(-current), 2));
            matrix_set(out, y, x, sigm_d);
        }
    }
}

