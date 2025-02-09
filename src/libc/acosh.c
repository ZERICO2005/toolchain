#include <math.h>

/**
 * @remarks Relative precision of 2^-22 at +1.000120640e+00 assuming ideal
 * log1pf. Relative precision of 2^-19.83 at +1.006979227e+00 using the
 * toolchain's implementation of log1pf
 */
float _acoshf_c(float x) {
    /* x < 0x1.0p+63f */
    if (x < 9223372036854775808.0f) {
        float t = x - 1.0f;
        return log1pf(t + sqrtf(2.0f * t + t * t));
    }
    return logf(x) + (float)M_LN2;
}

double acosh(double) __attribute__((alias("acoshf")));

/**
 * @remarks Relative precision of 2^-42.37 at +1.000000358e+00
 */
long double acoshl(long double x) {
    /* x < 0x1.0p+511L */
    if (x < 6.703903964971298549787e+153L) {
        long double t = x - 1.0L;
        return log1pl(t + sqrtl(2.0L * t + t * t));
    }
    /* ln(x) + ln(2) */
    return logl(x) + 0.693147180559945309417L;
}
