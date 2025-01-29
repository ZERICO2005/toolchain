#include <math.h>
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

/**
 * @remarks ULP of 3.5
 */
long double expl(long double d) {
    if (iszero(d)) {
        /**
         * @todo check if this function returns 1.0 on inputs +0.0 and -0.0
         * without having a special case to handle it
         */
        return 1.0L;
    }
    long double q_f64 = roundl(d * R_LN2);
    int q = (int)q_f64;
    long double s, u;

    s = mla(q_f64, -L2U, d);
    s = mla(q_f64, -L2L, s);

    long double s2 = s * s, s4 = s2 * s2, s8 = s4 * s4;
    u = POLY10(s, s2, s4, s8,
            2.08860621107283687536341e-09L,
            2.51112930892876518610661e-08L,
            2.75573911234900471893338e-07L,
            2.75572362911928827629423e-06L,
            2.4801587159235472998791e-05L,
            0.000198412698960509205564975L,
            0.00138888888889774492207962L,
            0.00833333333331652721664984L,
            0.0416666666666665047591422L,
            0.166666666666666851703837L);
    u = fmal(u, s, 0.5L);

    u = s * s * u + s + 1.0L;
    /* ldexp2k */
    u = ldexpl(u, q);

    // ldexpl should handle these cases
    #if 0
    if (d > LOG_LDBL_MAX) u = HUGE_VALL;
    if (d < -1000.0L) u = 0;
    #endif

    return u;
}
