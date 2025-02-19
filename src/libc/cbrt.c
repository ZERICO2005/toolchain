#include <math.h>

/**
 * @remarks Minimum ulp:
 * ulp of -1  at +0x1.000002p-4   with ideal powf (1/16 < fabsf(x) < 16)
 * ulp of -17 at +0x1.000000p-147 with ideal powf (fabsf(x) < 1.0e-30f)
 * ulp of +15 at +0x1.ea7c60p+122 with ideal powf (fabsf(x) > 1.0e+30f)
 * ulp of +20 at +0x1.62680ap+26  with current powf, expf, and logf
 */
float cbrtf(float x)
{
    return copysignf(powf(fabsf(x), 1.0f/3.0f), x);
}

double cbrt(double) __attribute__((alias("cbrtf")));
