#include <math.h>

/* Switch to the first version once expf is more accurate */

#if 0

/**
 * @remarks Relative precision of:
 * 2^-18.54 at -3.099555522e-02 with ideal expf
 * 2^-16.12 at -3.158956021e-02 with current expf
 */
float expm1f(float x) {
    /* 0x1.0p-5f */
    if (fabsf(x) < 0.03125f) {
        float x_sqr = x * x;
        return (x_sqr * x) * 0.166666666666666666667f + 0.5f * (x_sqr) + x;
    }
    return expf(x) - 1.0f;
}

#else

/**
 * @remarks Relative precision of:
 * 2^-18.09 at +3.988865763e-02 with ideal expf
 * 2^-17.04 at +5.245876685e-02 with current expf
 */
float expm1f(float x) {
    if (fabsf(x) < 0.04f) {
        float x_sqr = x * x;
        return (x_sqr * x) * 0.166666666666666666667f + 0.5f * (x_sqr) + x;
    }
    return expf(x) - 1.0f;
}

#endif

double expm1(double) __attribute__((alias("expm1f")));
