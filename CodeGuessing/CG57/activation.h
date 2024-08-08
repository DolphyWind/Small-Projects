#ifndef __ACTIVATION_H__
#define __ACTIVATION_H__
#include "matrix.h"

typedef void(*activation_t)(Matrix* in, Matrix* out);
typedef void(*derivative_t)(Matrix* in, Matrix* out);

typedef struct {
    char* name;
    activation_t func;
    derivative_t derivative;
} Activation;

Activation* activation_init(char* name, activation_t func, derivative_t derivative);
void activation_free(Activation* p);
void activation_apply(Activation* a, Matrix* in, Matrix* out);

// Some pre-defined activation functions and their derivatives
void identity(Matrix* in, Matrix* out);
void one(Matrix* in, Matrix* out);
void relu(Matrix* in, Matrix* out);
void relu_derivative(Matrix* in, Matrix* out);
void softmax(Matrix* in, Matrix* out);
void softmax_derivative(Matrix* in, Matrix* out);
void sigmoid(Matrix* in, Matrix* out);
void sigmoid_derivative(Matrix* in, Matrix* out);

#endif
