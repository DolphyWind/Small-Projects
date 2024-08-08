#ifndef __UTIL_H__
#define __UTIL_H__
#include "matrix.h"

cell_t get_random(); // Random between -1 and 1
cell_t get_random_bounded(cell_t a, cell_t b);
void choose_n_indices(size_t n, size_t max_index, size_t* out);
cell_t clamp(cell_t in, cell_t min, cell_t max);
cell_t max(cell_t a, cell_t b);
cell_t absc(cell_t a);

#endif
