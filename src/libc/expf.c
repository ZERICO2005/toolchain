/************************************************************************/
/*									*/
/*			Copyright (C) 2000-2008 Zilog, Inc.		*/
/*									*/
/************************************************************************/
/*
    exp returns the exponential function of its
    floating-point argument.

    The coefficients are #1069 from Hart and Cheney. (22.35D)
*/

#include <errno.h>
#include <math.h>

/**
 * @note These coefficients are for float64
 */
#define	p0 0.20803843466947e7f
#define	p1 0.30286971697440e5f
#define	p2 0.60614853300611e2f
#define	q0 0.60027203602388e7f
#define	q1 0.32772515180829e6f
#define	q2 0.17492876890931e4f

#if 0
#define	exp_max_arg 88.723f
#define exp_min_arg -103.28f
#else
/* mantissa is zero for these constants */
#define	exp_max_arg 128.0f
#define exp_min_arg -128.0f
#endif

/**
 * @remarks Minimum relative precision of:
 * 2^-21    at +6.320388317e-01 (-ln(2) < x < +ln(2))
 * 2^-17.09 at +6.030083084e+01 (-64.0L < x < +64.0L)
 *
 * See the purple line for relative precision (lag warning):
 * https://www.desmos.com/calculator/zlrmxatxkf
 */
float _expf_c(float arg) {
    float fraction;
    float temp1, temp2, xsq;
    int ent;

    if ( arg == 0.0f ){
        return 1.0f;
    }
    if ( arg < exp_min_arg ){
        return 0.0f;
    }
    if ( arg > exp_max_arg ){
        errno = ERANGE;
        return HUGE_VALF;
    }
    arg *= (float)M_LOG2E;
    ent = floorf( arg );
    fraction = arg - ent - 0.5f;
    xsq = fraction * fraction;
    temp1 = ((p2 * xsq + p1) * xsq + p0) * fraction;
    temp2 = ((xsq + q2) * xsq + q1) * xsq + q0;
    return ldexpf( (float)M_SQRT2 * (temp2+temp1) / (temp2-temp1), ent );
}

double _exp_c(double) __attribute__((alias("_expf_c")));
