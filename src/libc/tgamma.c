/*
 * gamma.c  - public domain implementation of function tgamma(3m)
 * reference - Haruhiko Okumura: C-gengo niyoru saishin algorithm jiten
 *  (New Algorithm handbook in C language) (Gijyutsu hyouronsha, Tokyo, 1991) [in Japanese]
 *  http://oku.edu.mie-u.ac.jp/~okumura/algo/
*/

#include <math.h>
#include <errno.h>

/**
 * @remarks Minimum relative precision of:
 * 2^-15.34 at +3.226818848e+01 with ideal sinf expf and lgammaf (x > 0.0f)
 * 2^-17    at +2.940585591e-39 with ideal sinf expf and lgammaf (x > 0.0f && x < 10.0f)
 * 2^-17.91 at +9.224035263e+00 with ideal sinf expf and lgammaf (x > 0.1f && x < 10.0f)
 */
float tgammaf(float x) { /* Gamma function */
    if (x == 0.0) { /* Pole Error */
        errno = ERANGE;
        return signbit(x) ? -HUGE_VAL : HUGE_VAL;
    }
    if (x < 0) {
        int sign;
    static float zero = 0.0;
        float i, f;
        f = modff(-x, &i);
        if (f == 0.0) { /* Domain Error */
            errno = EDOM;
            return zero/zero;
        }
        sign = (fmodf(i, 2.0) != 0.0) ? 1 : -1;
        return sign * (float)M_PI / (sinf((float)M_PI * f) * expf(lgammaf(1 - x)));
    }
    return expf(lgammaf(x));
}

double tgamma(double) __attribute__((alias("tgammaf")));
