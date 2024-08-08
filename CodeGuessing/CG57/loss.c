#include "loss.h"
#include "matrix.h"
#include "util.h"
#include <assert.h>
#include <math.h>

void mse(Matrix* preds, Matrix* target, Matrix* out)
{
    assert(preds->cols == target->cols && preds->rows == target->rows);
    size_t cols = preds->cols;
    size_t rows = preds->rows;

    Matrix* differences = matrix_init(rows, cols);
    matrix_sub_matrix(target, preds, differences);

    for(size_t y = 0; y < rows; ++y)
    {
        cell_t sum = 0;
        for(size_t x = 0; x < cols; ++x)
        {
            cell_t current = matrix_at(differences, y, x);
            sum += current * current;
        }

        matrix_set(out, y, 0, sum / (cols * 2));
    }

    matrix_free(differences);
}

void mse_derivative(Matrix* preds, Matrix* target, Matrix* out)
{
    assert(preds->cols == out->cols && preds->rows == out->rows);
    
    matrix_sub_matrix(target, preds, out);
    matrix_scale(out, -2.0 / preds->cols);
}

void cross_entropy(Matrix* preds, Matrix* target, Matrix* out)
{
    assert(preds->cols == target->cols && preds->rows == target->rows);

    size_t len = preds->cols;
    size_t batch_size = out->rows;
    const cell_t epsilon = 1e-7;

    for(size_t y = 0; y < batch_size; ++y)
    {
        cell_t loss = 0;
        for(size_t x = 0; x < len; ++x)
        {
            cell_t pred = matrix_at(preds, y, x);
            cell_t true_lbl = matrix_at(target, y, x);
            loss += true_lbl * log(pred + epsilon);
        }
        matrix_set(out, y, 0, -loss / batch_size);
    }
}

void cross_entropy_derivative(Matrix* preds, Matrix* targets, Matrix* out)
{
    assert(preds->cols == out->cols && preds->rows == out->rows);

    size_t batch_size = out->rows;
    size_t len = preds->cols;
    const cell_t epsilon = 1e-7;

    for(size_t y = 0; y < batch_size; ++y)
    {
        for(size_t x = 0; x < len; ++x)
        {
            cell_t pred = matrix_at(preds, y, x);
            cell_t true_lbl = matrix_at(targets, y, x);
            pred = clamp(pred, epsilon, 1 - epsilon);
            matrix_set(out, y, x, -1.0 * (true_lbl / pred) );
        }
    }
}

