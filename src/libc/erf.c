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

/**
 * @remarks Minimum relative precision of:
 * 2^-43.01 at +8.651415701e-04 with ideal erfcl (x > 0.0L)
 * 2^-42.99 at -9.766324656e-04 with ideal erfcl (x < 0.0L)
 */
long double erfl(long double x) {
    /* arg < 0x1.0p-10L */
    if (fabsl(x) < 0.0009765625L) {
        /* M_2_SQRTPI */
        return 1.1283791670955125738961589031215L * (x - x * x * x * 0.333333333333333333333L);
    }
    return 1.0L - erfcl(x);
}