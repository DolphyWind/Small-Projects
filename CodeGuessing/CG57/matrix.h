#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdio.h>
#include <stdlib.h>


typedef double cell_t;

typedef struct {
    cell_t* data;
    size_t rows;
    size_t cols;
} Matrix;

Matrix* matrix_init(size_t rows, size_t cols);
Matrix* matrix_initn(size_t rows, size_t cols, cell_t n);
Matrix* matrix_initr(size_t rows, size_t cols);
Matrix* matrix_initrf(size_t rows, size_t cols, cell_t f);
void matrix_copy(Matrix* dest, Matrix* src);
void matrix_free(Matrix* ptr);
cell_t matrix_at(Matrix* m, size_t row, size_t col);
cell_t* matrix_atp(Matrix* m, size_t row, size_t col);
void matrix_set(Matrix* m, size_t row, size_t col, cell_t val);
void matrix_print(FILE* stream, Matrix* m);
void matrix_mul(Matrix* m1, Matrix* m2, Matrix* dest);
void matrix_cwise_mul(Matrix* m1, Matrix* m2, Matrix* dest);
void matrix_scale(Matrix* m, cell_t scalar);
void matrix_add(Matrix* m, cell_t scalar);
void matrix_sub(Matrix* m, cell_t scalar);
void matrix_add_matrix(Matrix* m1, Matrix* m2, Matrix* out);
void matrix_sub_matrix(Matrix* m1, Matrix* m2, Matrix* out);
void matrix_copy_row(Matrix* dest, Matrix* src, size_t row_dest, size_t row_stc);
cell_t matrix_col_mean(Matrix* m, size_t col);
void matrix_clip_norm(Matrix* m, cell_t clip_norm);
void matrix_clip_value(Matrix* m, cell_t clip_val);
void matrix_batch_mul(Matrix* m1, Matrix* m2, Matrix* dest);

#endif
