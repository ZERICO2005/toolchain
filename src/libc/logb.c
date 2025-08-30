#include <fenv.h>
#include <math.h>

float old_logbf(float x) {
    if (isfinite(x)) {
        if (iszero(x)) {
            feraiseexcept(FE_DIVBYZERO);
            return -HUGE_VALF;
        }
        return (float)ilogbf(x);
    }
    // infinity and NaN
    return fabsf(x);
}

double old_logb(double) __attribute__((alias("old_logbf")));

long double old_logbl(long double x) {
    if (isfinite(x)) {
        if (iszero(x)) {
            feraiseexcept(FE_DIVBYZERO);
            return -HUGE_VALL;
        }
        return (long double)ilogbl(x);
    }
    // infinity and NaN
    return fabsl(x);
}
