#include <errno.h>
#include <math.h>

/*
; only works for normalized values
; CC: 9F + 12R + 9W + 1 | 8 bytes
__f32_fast_div4:
	pop	bc, hl, de
	dec	e	; subtracts 2 from the exponent
	push	de, hl, bc
	ret
*/
float _f32_fast_div4(float x);

/**
 * @remarks Minimum ulp of -1
 */
float _sqrtf_c(float x)
{
    float f, y;
    int n;

    if (x==0.0) return x;
    else if (x==1.0) return 1.0;
    else if (x<0.0)
    {
        errno=EDOM;
        return 0.0;
    }
    f=frexpf(x, &n);
    y=0.41731+0.59016*f; /*Educated guess*/
    /*For a 24 bit mantisa (float), two iterations are sufficient*/
    y+=f/y;
    y = _f32_fast_div4(y) + f/y; /*Faster version of 0.25 * y + f/y*/

    if (n&1)
    {
        y*=0.7071067812;
        ++n;
    }
    /* n will be [-148, 128] prior to division */
    return ldexpf(y, n/2);
}

double _sqrt_c(double) __attribute__((alias("_sqrtf_c")));
