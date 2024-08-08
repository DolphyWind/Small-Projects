#include "matrix.h"
#include "util.h"
#include <stdio.h>
#include <assert.h>

Matrix* matrix_init(size_t rows, size_t cols)
{
    Matrix* matrix = malloc(sizeof(Matrix));
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(sizeof(cell_t) * rows * cols);
    return matrix;
}

Matrix* matrix_initn(size_t rows, size_t cols, cell_t n)
{
    Matrix* matrix = matrix_init(rows, cols);
    for(size_t y = 0; y < rows; ++y)
    {
        for(size_t x = 0; x < cols; ++x)
        {
            matrix_set(matrix, y, x, n);
        }
    }
    return matrix;
}

Matrix* matrix_initr(size_t rows, size_t cols)
{
    Matrix* matrix = matrix_init(rows, cols);
    for(size_t y = 0; y < rows; ++y)
    {
        for(size_t x = 0; x < cols; ++x)
        {
            matrix_set(matrix, y, x, get_random());
        }
    }

    return matrix;
}

Matrix* matrix_initrf(size_t rows, size_t cols, cell_t f)
{
    Matrix* matrix = matrix_init(rows, cols);
    for(size_t y = 0; y < rows; ++y)
    {
        for(size_t x = 0; x < cols; ++x)
        {
            matrix_set(matrix, y, x, get_random() * f);
        }
    }

    return matrix;
}

void matrix_copy(Matrix* dest, Matrix* src)
{
    assert(dest->cols == src->cols && dest->rows == src->rows);
    if(dest == src) return;

    for(size_t y = 0; y < src->rows; ++y)
    {
        for(size_t x = 0; x < src->cols; ++x)
        {
            matrix_set(dest, y, x, matrix_at(src, y, x));
        }
    }
}

void matrix_free(Matrix* ptr)
{
    if(!ptr) return;

    free(ptr->data);
    free(ptr);
}

cell_t matrix_at(Matrix* m, size_t row, size_t col)
{
    assert(row < m->rows);
    assert(col < m->cols);

    return m->data[row * m->cols + col];
}

cell_t* matrix_atp(Matrix* m, size_t row, size_t col)
{
    return m->data + (row * m->cols + col);
}

void matrix_set(Matrix* m, size_t row, size_t col, cell_t val)
{
    *matrix_atp(m, row, col) = val;
}

void matrix_print(FILE* stream, Matrix* m)
{
    size_t y = 0;
    size_t x = 0;
    for(x = 0; y < m->rows;)
    {
        fprintf(stream, "%f ", matrix_at(m, y, x));

        if(x + 1 >= m->cols)
        {
            printf("\n");
            x = 0;
            ++y;
        } else ++x;
    }
}

void matrix_mul(Matrix* m1, Matrix* m2, Matrix* dest)
{
    assert(m1->cols == m2->rows);
    for(size_t y = 0; y < dest->rows; ++y)
    {
        for(size_t x = 0; x < dest->cols; ++x)
        {
            cell_t sum = 0;
            for(size_t i = 0; i < m1->cols; ++i)
            {
                sum += matrix_at(m1, y, i) * matrix_at(m2, i, x);
            }
            matrix_set(dest, y, x, sum);
        }
    }
}

void matrix_cwise_mul(Matrix* m1, Matrix* m2, Matrix* dest)
{
    assert(m1->rows == m2->rows && m1->cols == m2->cols);
    assert(m1->rows == dest->rows && m1->cols == dest->cols);

    for(size_t y = 0; y < dest->rows; ++y)
    {
        for(size_t x = 0; x < dest->cols; ++x)
        {
            matrix_set(dest, y, x, matrix_at(m1, y, x) * matrix_at(m2, y, x));
        }
    }
}

void matrix_scale(Matrix* m, cell_t scalar)
{
    for(size_t y = 0; y < m->rows; ++y)
    {
        for(size_t x = 0; x < m->cols; ++x)
        {
            *matrix_atp(m, y, x) *= scalar;
        }
    }
}

void matrix_add(Matrix* m, cell_t scalar)
{
    for(size_t y = 0; y < m->rows; ++y)
    {
        for(size_t x = 0; x < m->cols; ++x)
        {
            *matrix_atp(m, y, x) += scalar;
        }
    }
}

void matrix_sub(Matrix* m, cell_t scalar)
{
    matrix_add(m, -scalar);
}

void matrix_add_matrix(Matrix* m1, Matrix* m2, Matrix* out)
{
    assert(m1->rows == out->rows || m1->cols == out->cols);

    // Broadcast if the src is a vector
    // I know, the code below is horrible and very error prone,
    // but if you be a nice person it won't bite you
    size_t mul_y = (m1->rows == m2->rows);
    size_t mul_x = (m1->cols == m2->cols);

    for(size_t y = 0; y < m1->rows; ++y)
    {
        for(size_t x = 0; x < m1->cols; ++x)
        {
            *matrix_atp(out, y, x) = matrix_at(m1, y, x) + matrix_at(m2, mul_y * y, mul_x * x);
        }
    }
}

void matrix_sub_matrix(Matrix* m1, Matrix* m2, Matrix* out)
{
    // assert(m1->rows == m2->rows && m1->cols == m2->cols);
    // assert(m1->rows == out->rows && m1->cols == out->cols);

    size_t mul_y = (m1->rows == m2->rows);
    size_t mul_x = (m1->cols == m2->cols);

    for(size_t y = 0; y < m1->rows; ++y)
    {
        for(size_t x = 0; x < m1->cols; ++x)
        {
            *matrix_atp(out, y, x) = matrix_at(m1, y, x) - matrix_at(m2, mul_y * y, mul_x * x);
        }
    }
}

void matrix_copy_row(Matrix* dest, Matrix* src, size_t row_dest, size_t row_src)
{
    assert(dest->cols == src->cols);

    for(size_t x = 0; x < dest->cols; ++x)
    {
        matrix_set(dest, row_dest, x, matrix_at(src, row_src, x));
    }
}

cell_t matrix_col_mean(Matrix* m, size_t col)
{
    cell_t sum = 0;
    for(size_t i = 0; i < m->rows; ++i)
    {
        sum += matrix_at(m, i, col);
    }

    return sum / m->rows;
}

void matrix_clip_norm(Matrix* m, cell_t clip_norm)
{
    for(size_t y = 0; y < m->rows; ++y)
    {
        cell_t norm = 0;
        for(size_t x = 0; x < m->cols; ++x)
        {
            cell_t current = matrix_at(m, y, x);
            norm += current * current;
        }

        if(norm > clip_norm)
        {
            for(size_t x = 0; x < m->cols; ++x)
            {
                cell_t current = matrix_at(m, y, x);
                matrix_set(m, y, x, current / norm);
            }
        }
    }
}

void matrix_clip_value(Matrix* m, cell_t clip_value)
{
    for(size_t y = 0; y < m->rows; ++y)
    {
        for(size_t x = 0; x < m->cols; ++x)
        {
            cell_t current = matrix_at(m, y, x);
            cell_t abs = current > 0 ? current : -current;
            if(abs > clip_value)
            {
                matrix_set(m, y, x, clip_value);
            }
        }
    }
}

void matrix_batch_mul(Matrix* m1, Matrix* m2, Matrix* dest)
{
    assert(m1->cols == m2->cols);
    size_t batch_size = m2->rows;
    size_t cols = m1->cols;
    assert(dest->rows == batch_size);

    for(size_t y = 0; y < batch_size; ++y)
    {
        for(size_t cy = 0; cy < m1->rows; ++cy)
        {
            cell_t sum = 0;
            for(size_t cx = 0; cx < cols; ++cx)
            {
                sum += matrix_at(m1, cy, cx) * matrix_at(m2, y, cx);
            }
            matrix_set(dest, y, cy, sum);
        }
    }
}

