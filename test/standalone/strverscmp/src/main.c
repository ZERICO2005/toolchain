#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <ti/sprintf.h>

//------------------------------------------------------------------------------
// Config
//------------------------------------------------------------------------------

// define to 0 or 1
#define DEBUG_DIAGNOSTICS 1

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------

#define C(expr) if (!(expr)) { return __LINE__; }

#define TEST(test) { ret = test; if (ret != 0) { return ret; }}

#ifndef DEBUG_DIAGNOSTICS
#error "DEBUG_DIAGNOSTICS needs to be defined to 0 or 1"
#endif

#if DEBUG_DIAGNOSTICS
#define test_printf(...) sprintf((char*)0xFB0000, __VA_ARGS__)
#else
#define test_printf(...)
#endif

int strverscmp(const char *s1, const char *s2);

/* states: S_N: normal, S_I: comparing integral part, S_F: comparing
    fractionnal parts, S_Z: idem but with leading Zeroes only
*/
#define  S_N    0x0
#define  S_I    0x3
#define  S_F    0x6
#define  S_Z    0x9
/* result_type: CMP: return diff; LEN: compare using len_diff/diff */
#define  CMP    2
#define  LEN    3
/* Compare S1 and S2 as strings holding indices/version numbers,
    returning less than, equal to or greater than zero if S1 is less than,
    equal to or greater than S2 (for more info, see the texinfo doc).
*/

int __strverscmp (const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    /* Symbol(s)    0       [1-9]   others
    Transition   (10) 0  (01) d  (00) x   */
    static const uint8_t next_state[] = {
        /* state    x    d    0  */
        /* S_N */  S_N, S_I, S_Z,
        /* S_I */  S_N, S_I, S_I,
        /* S_F */  S_N, S_F, S_F,
        /* S_Z */  S_N, S_F, S_Z
    };
    static const int8_t result_type[] = {
        /* state   x/x  x/d  x/0  d/x  d/d  d/0  0/x  0/d  0/0  */
        /* S_N */  CMP, CMP, CMP, CMP, LEN, CMP, CMP, CMP, CMP,
        /* S_I */  CMP, -1,  -1,  +1,  LEN, LEN, +1,  LEN, LEN,
        /* S_F */  CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP,
        /* S_Z */  CMP, +1,  +1,  -1,  CMP, CMP, -1,  CMP, CMP
    };
    if (p1 == p2) {
        return 0;
    }
    unsigned char c1 = *p1++;
    unsigned char c2 = *p2++;
    /* Hint: '0' is a digit too.  */
    int state = S_N + ((c1 == '0') + (isdigit (c1) != 0));
    int diff;
    while ((diff = c1 - c2) == 0) {
        if (c1 == '\0') {
            return diff;
        }
        state = next_state[state];
        c1 = *p1++;
        c2 = *p2++;
        state += (c1 == '0') + (isdigit (c1) != 0);
    }
    state = result_type[state * 3 + (((c2 == '0') + (isdigit (c2) != 0)))];
    switch (state) {
        case CMP:
            return diff;
        case LEN:
            while (isdigit(*p1++)) {
                if (!isdigit(*p2++)) {
                    return 1;
                }
            }
            return isdigit(*p2) ? -1 : diff;
        default:
            return state;
    }
}


int strcmp_1( const void* a, const void* b )
{
    return __strverscmp( *(const char**)a, *(const char**)b );
}

int strcmp_2( const void* a, const void* b )
{
    return strverscmp( *(const char**)a, *(const char**)b );
}

int strcmp_3( const void* a, const void* b )
{
    return strcmp( *(const char**)a, *(const char**)b );
}

int shuffle( const void* a, const void* b )
{
    return strcmp( *(const char**)b, *(const char**)a );
}

int run_tests()
{
    char const * xs[] = {
        // "Korra",
        // "Zhu Li",
        // "Asami",
        // "Mako",
        // "Bolin",
        // "Tenzin",
        // "Varrick",
        "0",
        "1",
        "9",
        "00",
        "01",
        "09",
        "10",
        "11",
        "19",
        "90",
        "91",
        "99",
        "000",
        "001",
        "009",
        "010",
        "011",
        "019",
        "090",
        "091",
        "099",
        "000",
        "010",
        "090",
        "100",
        "110",
        "190",
        "900",
        "910",
        "990",
        // "jan0",
        // "jan1",
        // "jan9",
        // "jan00",
        // "jan01",
        // "jan09",
        // "jan10",
        // "jan11",
        // "jan19",
        // "jan90",
        // "jan91",
        // "jan99",
        // "jan000",
        // "jan001",
        // "jan009",
        // "jan010",
        // "jan011",
        // "jan019",
        // "jan090",
        // "jan091",
        // "jan099",
        // "jan000",
        // "jan010",
        // "jan090",
        // "jan100",
        // "jan110",
        // "jan190",
        // "jan900",
        // "jan910",
        // "jan990",
        // "0feb",
        // "1feb",
        // "9feb",
        // "00feb",
        // "01feb",
        // "09feb",
        // "10feb",
        // "11feb",
        // "19feb",
        // "90feb",
        // "91feb",
        // "99feb",
        // "000feb",
        // "001feb",
        // "009feb",
        // "010feb",
        // "011feb",
        // "019feb",
        // "090feb",
        // "091feb",
        // "099feb",
        // "000feb",
        // "010feb",
        // "090feb",
        // "100feb",
        // "110feb",
        // "190feb",
        // "900feb",
        // "910feb",
        // "990feb",
    };
    const size_t N = ARRAY_LENGTH(xs);

    qsort(xs, N, sizeof(xs[0]), shuffle);
    qsort(xs, N, sizeof(xs[0]), strcmp_1);
    test_printf("\n\n"); for (size_t n = 0; n < N; n++) { test_printf("%s, ", xs[n]); }

    // qsort(xs, N, sizeof(xs[0]), shuffle);
    qsort(xs, N, sizeof(xs[0]), strcmp_2);
    test_printf("\n\n"); for (size_t n = 0; n < N; n++) { test_printf("%s, ", xs[n]); }

    // qsort(xs, N, sizeof(xs[0]), shuffle);
    qsort(xs, N, sizeof(xs[0]), strcmp_3);
    test_printf("\n\n"); for (size_t n = 0; n < N; n++) { test_printf("%s, ", xs[n]); }

    return 0;
}

int main(void) {
    os_ClrHome();
    int failed_test = run_tests();
    if (failed_test != 0) {
        char buf[sizeof("Failed test L-8388608\n")];
        boot_sprintf(buf, "Failed test L%d\n", failed_test);
        fputs(buf, stdout);
    } else {
        fputs("All tests passed", stdout);
    }

    while (!os_GetCSC());

    return 0;
}
