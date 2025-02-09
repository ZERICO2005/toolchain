/************************************************************************/
/*									*/
/*			Copyright (C)1987-2008 by				*/
/*		            Zilog, Inc.           			*/
/*									*/
/*		        San Jose, California     			*/
/*									*/
/************************************************************************/
/*
    sinh(arg) returns the hyperbolic sine of its floating-
    point argument.

    The exponential function is called for arguments
    greater in magnitude than 0.5.

    A series is used for arguments smaller in magnitude than 0.5.
    The coefficients are #2029 from Hart & Cheney. (20.36D)

    cosh(arg) is computed from the exponential function for
    all arguments.
*/

#include <math.h>

/**
 * @remarks Minimum relative precision of:
 * 2^-23    at +2.991109795e-04 with ideal expf (x < 88.5f)
 * 2^-17    at +7.901648712e+01 with current expf (x < 80.0f)
 * 2^-18.83 at +1.662612534e+01 with current expf (x < 21.0f)
 */
float _coshf_c(float arg) {
    float val;
    arg = fabsf(arg);

    if (arg > 88.5f) {
        /* Handles some overflow cases */
        return expf(arg - (float)M_LN2);
    }

    val = expf(arg);

    if(arg > 21.0f) {
        return val / 2.0f;
    }

    val += expf(-arg);
    val /= 2.0f;
    return val;
}

double _cosh_c(double) __attribute__((alias("_coshf_c")));
