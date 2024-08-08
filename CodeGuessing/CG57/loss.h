#ifndef __LOSS_H__
#define __LOSS_H__
#include "matrix.h"

// It's loss...
// |/|ı/||/|_

void mse(Matrix* preds, Matrix* target, Matrix* out);
void mse_derivative(Matrix* preds, Matrix* target, Matrix* out);
void cross_entropy(Matrix* preds, Matrix* target, Matrix* out);
void cross_entropy_derivative(Matrix* preds, Matrix* targets, Matrix* out);

#endif
