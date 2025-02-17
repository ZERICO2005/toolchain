#include <math.h>

/**
 * @remarks Minimum ulp:
 * ulp of -20 at +0x1.ff2afcp-5
 */
 float asinhf(float arg) {
    float x = fabsf(arg);
    if (x < 0.0703125f) {
        x = x - (x * x * x) * 0.166666666666666666667f;
    } else if (x < 9223372036854775808.0f) {
        /* x < 0x1.0p+63 */
        x = logf(x + sqrtf(x * x + 1.0f));
    } else {
        x = logf(x) + (float)M_LN2;
    }
    return copysignf(x, arg);
}

double asinh(double) __attribute__((alias("asinhf")));

/**
 * @remarks Relative precision of:
 * 2^-42.41 at +9.768006857e-04 with ideal logl
 */
long double asinhl(long double arg) {
    long double x = fabsl(arg);
    if (x < 0.0009765625L) {
        /* x < 0x1.0p-10L */
        x = x - (x * x * x) * 0.166666666666666666667L;
    } else if (x < 6.703903964971298549787e+153L) {
        /* x < 0x1.0p+511L */
        x = logl(x + sqrtl(x * x + 1.0L));
    } else {
        /* ln(x) + ln(2) */
        x = logl(x) + 0.693147180559945309417L;
    }
    return copysignl(x, arg);
}
