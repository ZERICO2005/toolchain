#include <math.h>

/**
 * @remarks Minimum ulp:
 * ulp of -9    at +0x1.00030ap-5 with ideal erfcf
 * ulp of +5549 at +0x1.c46b04p-5 with current erfcf
 */
float erff(float x) {
    /* arg < 0x1.0p-5f */
    if (fabsf(x) < 0.03125f) {
        return (float)M_2_SQRTPI * (x - x * x * x * 0.333333333333333333333f);
    }
    return 1.0f - erfcf(x);
}

double erf(double) __attribute__((alias("erff")));
