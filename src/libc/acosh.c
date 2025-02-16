#include <math.h>

/**
 * @remarks Minimum relative precision of:
 * 2^-22    at +1.000120640e+00 with ideal log1pf
 * 2^-19.83 at +1.006979227e+00 with current log1pf
 */
float acoshf(float x) {
    /* x < 0x1.0p+63f */
    if (x < 9223372036854775808.0f) {
        float t = x - 1.0f;
        return log1pf(t + sqrtf(2.0f * t + t * t));
    }
    return logf(x) + (float)M_LN2;
}

double acosh(double) __attribute__((alias("acoshf")));

/**
 * @remarks Minimum relative precision of:
 * 2^-42.37 at +1.000000358e+00 with ideal log1pl
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
