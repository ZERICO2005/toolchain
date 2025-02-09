/************************************************************************/
/*									*/
/*			Copyright (C)1987-2008 by				*/
/*		            Zilog, Inc.           			*/
/*									*/
/*		        San Jose, California     			*/
/*									*/
/************************************************************************/
/*
    tanh(arg) computes the hyperbolic tangent of its floating
    point argument.

    sinh and cosh are called except for large arguments, which
    would cause overflow improperly.
*/

#include <math.h>

/**
 * @remarks Minimum relative precision of:
 * 2^-22    at +9.155276930e-04 with ideal sinhf coshf
 * 2^-20.67 at +3.083078936e-02 with current sinhf coshf and ideal expf
 * 2^-20.19 at +5.236846209e-01 with current sinhf coshf and expf
 */
float _tanhf_c(float arg) {
    float x = fabsf(arg);

    if(x > 21.0f) {
        x = 1.0f;
    } else {
        x = sinhf(x) / coshf(x);
    }

    return copysignf(x, arg);
}

double _tanh_c(double) __attribute__((alias("_tanhf_c")));
