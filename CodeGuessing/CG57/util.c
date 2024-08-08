#include "util.h"
#include <stdlib.h>

cell_t get_random()
{
    return 2.0 * ((cell_t)rand() - (cell_t)RAND_MAX / 2.0) / (cell_t)RAND_MAX;
}

cell_t get_random_bounded(cell_t a, cell_t b)
{
    return a + (get_random() / 2.0 + 0.5) * (b - a);
}

void choose_n_indices(size_t n, size_t max_index, size_t* out)
{
    for(size_t i = 0; i < n; ++i)
    {
        out[i] = rand() % max_index;
    }
}

cell_t clamp(cell_t in, cell_t min, cell_t max)
{
    if(in > max) return max;
    if(in < min) return min;
    return in;
}

cell_t max(cell_t a, cell_t b)
{
    if(a > b) return a;
    return b;
}

cell_t absc(cell_t a)
{
    if(a < 0) return -a;
    return a;
}

