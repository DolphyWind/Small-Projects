#include <math.h>


double _sqrt(double x)
{
    double start = 0.0;
    double end = x;
    double mid;
    const double THRESHOLD = 1e-7;
    const int _ITER_LIMIT = 20;
    for(int i = 0; i < _ITER_LIMIT; i++) {
        mid = (start + end) / 2.0;
        if(fabs(mid * mid - x) < THRESHOLD) return mid;

        if(mid * mid > x) {
            end = mid;
        } else {
            start = mid;
        }
    }
    return mid;
}


double xa0c8a621(double x)
{
    return 1.0 / _sqrt(x);
}
