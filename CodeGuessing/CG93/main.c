#include <math.h>
#include <stdio.h>
#include "not_so_fast_inv_sqrt.c"


int main(int argc, char** argv)
{
    printf("NUM\tREAL\t\tAPPR\t\tABSE\t\tRELE\n");
    for(int i = 0; i < 100; ++i) {
        double sqrt_real = 1.0 / sqrt(i);
        double sqrt_approx = 1.0 / _sqrt(i);

        double abs_error = fabs(sqrt_approx - sqrt_real);
        double rel_error = abs_error / sqrt_real;

        printf("%d\t%lf\t%lf\t%lf\t%lf\n", i, sqrt_real, sqrt_approx, abs_error, rel_error);
    }
    return 0;
}
