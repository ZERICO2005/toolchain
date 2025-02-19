#include <math.h>

/**
 * @remarks Minimum ulp:
 * ulp of -513  at -0x1.35260d8034ac3p-10 with ideal erfcl
 * ulp of +6702 at -0x1.4ef5ac6f23690p-10 with current erfcl
 */
long double erfl(long double x) {
    /* arg < 0x1.0p-10L */
    if (fabsl(x) < 0.0009765625L) {
        /* M_2_SQRTPI */
        return 1.1283791670955125738961589031215L * (x - x * x * x * 0.333333333333333333333L);
    }
    return 1.0L - erfcl(x);
}
