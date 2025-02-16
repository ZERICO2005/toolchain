#include <math.h>

/**
 * @remarks Relative precision of:
 * 2^-22    at +3.030303493e-02 with ideal log1pf
 * 2^-19.83 at +5.882683396e-02 with current log1pf and ideal logf
 * 2^-19.54 at +6.091820449e-02 with current log1pf and logf
 */
float atanhf(float arg) {
    float x = fabsf(arg);
    x = 0.5f * log1pf((2.0f * x) / (1.0f - x));
    return copysignf(x, arg);
}

double atanh(double) __attribute__((alias("atanhf")));

/**
 * @remarks Relative precision of:
 * 2^-51 at +1.277396381e-01 with ideal log1pl
 */
long double atanhl(long double arg) {
    long double x = fabsl(arg);
    x = 0.5L * log1pl((2.0L * x) / (1.0L - x));
    return copysignl(x, arg);
}
