#include <math.h>

/**
 * @remarks Minimum relative precision of:
 * 2^-23    at +6.250000745e-02 with ideal powf (1/16 < fabsf(x) < 16)
 * 2^-18.91 at +4.344025239e-44 with ideal powf (fabsf(x) < 1.0e-30f)
 * 2^-19.09 at +1.329224985e+36 with ideal powf (fabsf(x) > 1.0e+30f)
 */
float cbrtf(float x)
{
    return copysignf(powf(fabsf(x), 1.0f/3.0f), x);
}

double cbrt(double) __attribute__((alias("cbrtf")));
