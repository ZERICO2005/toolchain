/*
 * gamma.c  - public domain implementation of function tgamma(3m)
 * reference - Haruhiko Okumura: C-gengo niyoru saishin algorithm jiten
 *  (New Algorithm handbook in C language) (Gijyutsu hyouronsha, Tokyo, 1991) [in Japanese]
 *  http://oku.edu.mie-u.ac.jp/~okumura/algo/
*/

#include <math.h>
#include <errno.h>

#define N       8.0

#define B0  1.0                 /* Bernoulli numbers */
#define B1  (-1.0 / 2.0)
#define B2  ( 1.0 / 6.0)
#define B4  (-1.0 / 30.0)
#define B6  ( 1.0 / 42.0)
#define B8  (-1.0 / 30.0)
#define B10 ( 5.0 / 66.0)
#define B12 (-691.0 / 2730.0)
#define B14 ( 7.0 / 6.0)
#define B16 (-3617.0 / 510.0)

/**
 * @remarks Minimum relative precision of:
 * 2^-16.97 at +3.205794811e+00 with ideal logf (x > 3.0f)
 * 2^-16.71 at +2.940585591e-39 with ideal logf (x > 0.0f && x < 0.5f)
 * 2^-13.68 at +1.591955781e+00 with ideal logf (x > 1.25f && x < 1.75f)
 * @note input values 0.5f - 3.0f have very low precision
 */
float lgammaf(float x) { /* the natural logarithm of the Gamma function. */
    float v, w;

    v = 1;
    while (x < N && isfinite(v)) {
		v *= x;
		x++;
	}
    w = 1 / (x * x);
    return ((((((((B16 / (16 * 15))  * w + (B14 / (14 * 13))) * w
                + (B12 / (12 * 11))) * w + (B10 / (10 *  9))) * w
                + (B8  / ( 8 *  7))) * w + (B6  / ( 6 *  5))) * w
                + (B4  / ( 4 *  3))) * w + (B2  / ( 2 *  1))) / x
                + 0.5 * (float)M_LOG_2M_PI - logf(v) - x + (x - 0.5) * logf(x);
}

double lgamma(double) __attribute__((alias("lgammaf")));
