#include <math.h>

/* Switch to the first version once expf is more accurate */

#if 0

/**
 * @remarks Minimum ulp:
 * ulp of +22  at -0x1.fbd4c8p-6 with ideal expf
 * ulp of +117 at -0x1.02c81cp-5 with current expf
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
 * @remarks Minimum ulp:
 * ulp of -30 at +0x1.46c494p-5 with ideal expf
 * ulp of -62 at +0x1.adbe02p-5 with current expf (|x| < 64.0f)
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
