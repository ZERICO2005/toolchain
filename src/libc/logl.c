#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

//   Copyright Naoki Shibata and contributors 2010 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define L2U 0.69314718055966295651160180568695068359375L
#define L2L 0.28235290563031577122588448175013436025525412068e-12L
#define R_LN2 1.442695040888963407359924681001892137426645954152985934135449406931L
#define LOG_LDBL_MAX 709.78271289338399678773454114191L /* 0x1.62e42fefa39efp+9 */

#define C2V(x) (x)

#define MLA fmal
#define mla fmal

#define POLY2(x, c1, c0) MLA(x, C2V(c1), C2V(c0))
#define POLY3(x, x2, c2, c1, c0) MLA(x2, C2V(c2), MLA(x, C2V(c1), C2V(c0)))
#define POLY4(x, x2, c3, c2, c1, c0) MLA(x2, MLA(x, C2V(c3), C2V(c2)), MLA(x, C2V(c1), C2V(c0)))
#define POLY5(x, x2, x4, c4, c3, c2, c1, c0) MLA(x4, C2V(c4), POLY4(x, x2, c3, c2, c1, c0))
#define POLY6(x, x2, x4, c5, c4, c3, c2, c1, c0) MLA(x4, POLY2(x, c5, c4), POLY4(x, x2, c3, c2, c1, c0))
#define POLY7(x, x2, x4, c6, c5, c4, c3, c2, c1, c0) MLA(x4, POLY3(x, x2, c6, c5, c4), POLY4(x, x2, c3, c2, c1, c0))
#define POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0) MLA(x4, POLY4(x, x2, c7, c6, c5, c4), POLY4(x, x2, c3, c2, c1, c0))
#define POLY9(x, x2, x4, x8, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x8, C2V(c8), POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY10(x, x2, x4, x8, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x8, POLY2(x, c9, c8), POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY11(x, x2, x4, x8, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x8, POLY3(x, x2, ca, c9, c8), POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY12(x, x2, x4, x8, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x8, POLY4(x, x2, cb, ca, c9, c8), POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY13(x, x2, x4, x8, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x8, POLY5(x, x2, x4, cc, cb, ca, c9, c8), POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY14(x, x2, x4, x8, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x8, POLY6(x, x2, x4, cd, cc, cb, ca, c9, c8), POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY15(x, x2, x4, x8, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x8, POLY7(x, x2, x4, ce, cd, cc, cb, ca, c9, c8), POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY16(x, x2, x4, x8, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x8, POLY8(x, x2, x4, cf, ce, cd, cc, cb, ca, c9, c8), POLY8(x, x2, x4, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY17(x, x2, x4, x8, x16, d0, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x16, C2V(d0), POLY16(x, x2, x4, x8, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY18(x, x2, x4, x8, x16, d1, d0, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x16, POLY2(x, d1, d0), POLY16(x, x2, x4, x8, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY19(x, x2, x4, x8, x16, d2, d1, d0, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x16, POLY3(x, x2, d2, d1, d0), POLY16(x, x2, x4, x8, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY20(x, x2, x4, x8, x16, d3, d2, d1, d0, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x16, POLY4(x, x2, d3, d2, d1, d0), POLY16(x, x2, x4, x8, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0))
#define POLY21(x, x2, x4, x8, x16, d4, d3, d2, d1, d0, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0)\
    MLA(x16, POLY5(x, x2, x4, d4, d3, d2, d1, d0), POLY16(x, x2, x4, x8, cf, ce, cd, cc, cb, ca, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0))

typedef union F64_pun {
    long double flt;
    uint64_t bin;
} F64_pun;

#define Float64_pos_qNaN UINT64_C(0x7FF8000000000000)

/**
 * @brief intended as a size optimization when x is normalized, as the value
 * of 2.0L does not need to be pushed onto the stack
 */
long double _f64_fast_mul_two(long double x);

/**
 * @remarks ULP of 3.5
 */
long double logl(long double d) {
    if (iszero(d)) {
        errno = ERANGE;
        feraiseexcept(FE_DIVBYZERO);
        return -HUGE_VALL;
    }
    if (signbit(d)) {
        errno = EDOM;
        feraiseexcept(FE_INVALID);
        F64_pun ret;
        ret.bin = Float64_pos_qNaN;
        return ret.flt;
    }
    /* positive inf/NaN */
    if (!isfinite(d)) {
        feraiseexcept(FE_INVALID);
        return d;
    }
    long double x, x2, t, m;
    int e;

    bool d_subnormal = d < LDBL_MIN;
    // if (d_subnormal) d *= (long double)(INT64_C(1) << 32) * (long double)(INT64_C(1) << 32);
    if (d_subnormal) {
        // 2^64
        d *= 18446744073709551616.0L;
    }
    
    /* ilogb2k */
    e = ilogbl(d * (1.0L / 0.75L));
    /**
     * ldexp3k
     * @remarks support for subnormal values is not required, so a faster
     * alternative to ldexpl may be used.
     */
    m = ldexpl(d, -e);

    if (d_subnormal) {
        e -= 64;
    }
    
    x = (m - 1.0L) / (m + 1.0L);
    x2 = x * x;

    double x4 = x2 * x2, x8 = x4 * x4;

    t = POLY7(x2, x4, x8,
            0.153487338491425068243146L,
            0.152519917006351951593857L,
            0.181863266251982985677316L,
            0.222221366518767365905163L,
            0.285714294746548025383248L,
            0.399999999950799600689777L,
            0.6666666666667778740063L);

    x = x * 2.0L + 0.693147180559945286226764L * e + x * x2 * t;

    return x;
}
