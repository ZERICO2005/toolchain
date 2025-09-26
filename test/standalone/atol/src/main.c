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
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <ti/sprintf.h>

//------------------------------------------------------------------------------
// Config
//------------------------------------------------------------------------------

long my_strtol(const char* nptr, char** endptr, int base);
long _strtol_c(const char* nptr, char** endptr, int base);
unsigned long _strtoul_c(const char* nptr, char** endptr, int base);
#if 0
#define strtol my_strtol
#else
#define strtol _strtol_c
#endif
#define strtoul _strtoul_c

// define to 0 or 1
#define DEBUG_DIAGNOSTICS 1

// define to 0 or 1
#define EXTRA_TESTS 0

//------------------------------------------------------------------------------
// Utility
//------------------------------------------------------------------------------

#ifndef DEBUG_DIAGNOSTICS
#error "DEBUG_DIAGNOSTICS needs to be defined to 0 or 1"
#endif

#ifndef EXTRA_TESTS
#error "EXTRA_TESTS needs to be defined to 0 or 1"
#endif

#define NO_ERR 0

#if DEBUG_DIAGNOSTICS
#define test_printf printf

__attribute__((__unused__))
static void write_letter(char c) {
    if (isgraph(c)) {
        fputc(c, stdout);
        return;
    }
    fputc('\\', stdout);
    switch (c) {
        case '\0': fputc('0', stdout); return;
        case ' ' : fputc('s', stdout); return;
        case '\n': fputc('n', stdout); return;
        case '\t': fputc('t', stdout); return;
        case '\v': fputc('v', stdout); return;
        case '\r': fputc('r', stdout); return;
        case '\f': fputc('f', stdout); return;
        case '\b': fputc('b', stdout); return;
        default: printf("x%02X", (unsigned int)c); return;
    }
}

#else
#define test_printf(...)
#define write_letter(...)
#endif

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

#define C(expr) if (!(expr)) { return __LINE__; }

#define T(val, expr) do { \
    long long temp = (long long)(expr); \
    if ((long long)(val) != temp) { \
        test_printf("E: %llX\n", temp); \
        return __LINE__; \
    } \
} while (0);

#define TEST(test) { ret = test; if (ret != 0) { return ret; }}

#define TEST_NAME(test, name) { ret = test; if (ret != 0) { fputs(name "\n", stdout); return ret; }}

#define PRINT_ENDPTR(x) printf("%p - %p = %td\n", endptr, (x), endptr - (x))

//------------------------------------------------------------------------------
// Strings
//------------------------------------------------------------------------------

char const * const test_str[] = {
    ""             ,
    "+"            ,
    "-"            ,
    "0"            ,
    "+0"           ,
    "-0"           ,
    "1"            ,
    "+1"           ,
    "-1"           ,
    "+-84"         ,
    "--84"         ,
    "-+84"         ,
    "++84"         ,
    "+ 84"         ,
    "- 84"         ,
    "100"          ,
    "+100"         ,
    "-100"         ,
    " -123junk"    ,
    " +321dust"    ,
    " \f\n\r\t\v99",
    "0042"         ,
    "0x2A"         ,
    "junk"         ,
    "a701"         ,
};

//------------------------------------------------------------------------------
// Wrapper functions
//------------------------------------------------------------------------------

typedef long long (*atoint_func)(const char* nptr);

long long atoint_atoi(const char* nptr) {
    return (long long)atoi(nptr);
}

long long atoint_atol(const char* nptr) {
    return (long long)atol(nptr);
}

long long atoint_atoll(const char* nptr) {
    return (long long)atoll(nptr);
}

long long atoint_strtol(const char* nptr) {
    return (long long)strtol(nptr, NULL, 10);
}

long long atoint_strtoul(const char* nptr) {
    return (long long)(long)strtoul(nptr, NULL, 10);
}

long long atoint_strtoll(const char* nptr) {
    return strtoll(nptr, NULL, 10);
}

long long atoint_strtoull(const char* nptr) {
    return (long long)strtoull(nptr, NULL, 10);
}

typedef long long (*strtoint_func)(const char* nptr, char** endptr, int base);

long long strtoint_strtol(const char* nptr, char** endptr, int base) {
    return (long long)strtol(nptr, endptr, base);
}

long long strtoint_strtoul(const char* nptr, char** endptr, int base) {
    return (long long)(long)strtoul(nptr, endptr, base);
}

long long strtoint_strtoll(const char* nptr, char** endptr, int base) {
    return strtoll(nptr, endptr, base);
}

long long strtoint_strtoull(const char* nptr, char** endptr, int base) {
    return (long long)strtoull(nptr, endptr, base);
}

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------

bool atoint_verify(atoint_func func, long long truth, const char* nptr) {
    long long guess = (func)(nptr);
    if (guess == truth) {
        return true;
    }
    test_printf("E: %llX\n", guess);
    return false;
}

int atoint_test(atoint_func const func) {
    C(atoint_verify(func,        0, ""             ));
    C(atoint_verify(func,        0, "+"            ));
    C(atoint_verify(func,        0, "-"            ));
    C(atoint_verify(func,        0, "0"            ));
    C(atoint_verify(func,        0, "+0"           ));
    C(atoint_verify(func,        0, "-0"           ));
    C(atoint_verify(func,        1, "1"            ));
    C(atoint_verify(func,        1, "+1"           ));
    C(atoint_verify(func,       -1, "-1"           ));
    C(atoint_verify(func,        0, "+-84"         ));
    C(atoint_verify(func,        0, "--84"         ));
    C(atoint_verify(func,        0, "-+84"         ));
    C(atoint_verify(func,        0, "++84"         ));
    C(atoint_verify(func,        0, "+ 84"         ));
    C(atoint_verify(func,        0, "- 84"         ));
    C(atoint_verify(func,      100, "100"          ));
    C(atoint_verify(func,      100, "+100"         ));
    C(atoint_verify(func,     -100, "-100"         ));
    C(atoint_verify(func,     -123, " -123junk"    ));
    C(atoint_verify(func,      321, " +321dust"    ));
    C(atoint_verify(func,       99, " \f\n\r\t\v99"));
    C(atoint_verify(func,       42, "0042"         ));
    C(atoint_verify(func,        0, "0x2A"         ));
    C(atoint_verify(func,        0, "junk"         ));
    C(atoint_verify(func,        0, "a701"         ));
    C(atoint_verify(func,  8388607, "8388607"      ));
    C(atoint_verify(func, -8388608, "-8388608"     ));
    C(atoint_verify(func, -8388607, "\t-8388607"   ));
    return 0;
}

bool strtoint_verify(
    long long truth,
    const char* nptr,
    int endptr_offset,
    strtoint_func func,
    int base,
    int errno_state
) {
    char* endptr;
    long long guess;
    errno = 0;
    guess = (func)(nptr, &endptr, base);
    if (guess != truth) {
        test_printf("E: %llX\n", guess);
        return false;
    }
    if (endptr != nptr + endptr_offset) {
        test_printf("%p - %p = %td\n", endptr, nptr, endptr - nptr);
        return false;
    }
    if (errno != errno_state) {
        test_printf("errno: G %d != T %d\n", errno, errno_state);
        return false;
    }
    guess = (func)(nptr, NULL, base);
    if (guess != truth) {
        test_printf("NULL error: %llX\n", guess);
        return false;
    }
    return true;
}

int strtoint_test(strtoint_func const func) {
    C(strtoint_verify(   0, ""             , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "+"            , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "-"            , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "0"            , 1, func, 10, NO_ERR));
    C(strtoint_verify(   0, "+0"           , 2, func, 10, NO_ERR));
    C(strtoint_verify(   0, "-0"           , 2, func, 10, NO_ERR));
    C(strtoint_verify(   1, "1"            , 1, func, 10, NO_ERR));
    C(strtoint_verify(   1, "+1"           , 2, func, 10, NO_ERR));
    C(strtoint_verify(  -1, "-1"           , 2, func, 10, NO_ERR));
    C(strtoint_verify(   0, "+-84"         , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "--84"         , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "-+84"         , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "++84"         , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "+ 84"         , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "- 84"         , 0, func, 10, NO_ERR));
    C(strtoint_verify( 100, "100"          , 3, func, 10, NO_ERR));
    C(strtoint_verify( 100, "+100"         , 4, func, 10, NO_ERR));
    C(strtoint_verify(-100, "-100"         , 4, func, 10, NO_ERR));
    C(strtoint_verify(-123, " -123junk"    , 5, func, 10, NO_ERR));
    C(strtoint_verify( 321, " +321dust"    , 5, func, 10, NO_ERR));
    C(strtoint_verify(  99, " \f\n\r\t\v99", 8, func, 10, NO_ERR));
    C(strtoint_verify(  42, "0042"         , 4, func, 10, NO_ERR));
    C(strtoint_verify(   0, "0x2A"         , 1, func, 10, NO_ERR));
    C(strtoint_verify(   0, "junk"         , 0, func, 10, NO_ERR));
    C(strtoint_verify(   0, "a701"         , 0, func, 10, NO_ERR));
    
    C(strtoint_verify(          10, "1010"        ,  4, func,  2, NO_ERR));
    C(strtoint_verify(          10, "12"          ,  2, func,  8, NO_ERR));
    C(strtoint_verify(          10, "A"           ,  1, func, 16, NO_ERR));
    C(strtoint_verify(           0, "junk"        ,  0, func,  0, NO_ERR));
    C(strtoint_verify(      926192, "junk"        ,  4, func, 36, NO_ERR));
    C(strtoint_verify(      926192, "JuNk"        ,  4, func, 36, NO_ERR));
    C(strtoint_verify(           0, "foobar"      ,  0, func, 37, NO_ERR));
    C(strtoint_verify(           0, "fizzbuzz"    ,  0, func, -1, NO_ERR));
    C(strtoint_verify(           0, "000?"        ,  3, func,  1, NO_ERR));
    C(strtoint_verify(          10, "012"         ,  3, func,  0, NO_ERR));
    C(strtoint_verify(          10, "\f0xA"       ,  4, func,  0, NO_ERR));
    C(strtoint_verify(         -83, "-0123ABC"    ,  5, func,  0, NO_ERR));
    C(strtoint_verify(      0xCAFE, "0XCAFE"      ,  6, func,  0, NO_ERR));
    C(strtoint_verify(0b1011100010, "0b1011100010", 12, func,  2, NO_ERR));
    C(strtoint_verify(0b0100011101, "0B0100011101", 12, func,  0, NO_ERR));

    /**
     * @remarks If the first digit is '0' and the base is 0, then the
     * string should be treated as octal. This also implies that the string
     * contains a number, even if there are no more digits after the '0'.
     * Here we test that the string has been correctly classified as
     * containing a number, meaning that nptr != endptr.
     */
    C(strtoint_verify(           0, "0"      , 1, func,  0, NO_ERR));
    C(strtoint_verify(           0, "\n09"   , 2, func,  0, NO_ERR));

    /**
     * @remarks Make sure endptr is handled correctly when there is a
     * prefix without digits.
     */
    C(strtoint_verify(           0, "0x"   , 0, func,  0, NO_ERR));
    C(strtoint_verify(           0, "0x"   , 0, func, 16, NO_ERR));
    C(strtoint_verify(           0, "\v-0b", 0, func,  0, NO_ERR));
    C(strtoint_verify(           0, "\v-0b", 0, func,  2, NO_ERR));
    return 0;
}

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------

int test_atoi(void) {
    T(   0, atoi(test_str[ 0]));
    T(   0, atoi(test_str[ 1]));
    T(   0, atoi(test_str[ 2]));
    T(   0, atoi(test_str[ 3]));
    T(   0, atoi(test_str[ 4]));
    T(   0, atoi(test_str[ 5]));
    T(   1, atoi(test_str[ 6]));
    T(   1, atoi(test_str[ 7]));
    T(  -1, atoi(test_str[ 8]));
    T(   0, atoi(test_str[ 9]));
    T(   0, atoi(test_str[10]));
    T(   0, atoi(test_str[11]));
    T(   0, atoi(test_str[12]));
    T(   0, atoi(test_str[13]));
    T(   0, atoi(test_str[14]));
    T( 100, atoi(test_str[15]));
    T( 100, atoi(test_str[16]));
    T(-100, atoi(test_str[17]));
    T(-123, atoi(test_str[18]));
    T( 321, atoi(test_str[19]));
    T(  99, atoi(test_str[20]));
    T(  42, atoi(test_str[21]));
    T(   0, atoi(test_str[22]));
    T(   0, atoi(test_str[23]));
    T(   0, atoi(test_str[24]));

    T( INT_MIN, atoi("-8388608" ));
    T( INT_MAX, atoi("8388607"  ));
    T( INT_MAX, atoi("+8388607" ));
    T(-INT_MAX, atoi("-8388607" ));
    return 0;
}

int test_atol(void) {
    T(   0, atol(test_str[ 0]));
    T(   0, atol(test_str[ 1]));
    T(   0, atol(test_str[ 2]));
    T(   0, atol(test_str[ 3]));
    T(   0, atol(test_str[ 4]));
    T(   0, atol(test_str[ 5]));
    T(   1, atol(test_str[ 6]));
    T(   1, atol(test_str[ 7]));
    T(  -1, atol(test_str[ 8]));
    T(   0, atol(test_str[ 9]));
    T(   0, atol(test_str[10]));
    T(   0, atol(test_str[11]));
    T(   0, atol(test_str[12]));
    T(   0, atol(test_str[13]));
    T(   0, atol(test_str[14]));
    T( 100, atol(test_str[15]));
    T( 100, atol(test_str[16]));
    T(-100, atol(test_str[17]));
    T(-123, atol(test_str[18]));
    T( 321, atol(test_str[19]));
    T(  99, atol(test_str[20]));
    T(  42, atol(test_str[21]));
    T(   0, atol(test_str[22]));
    T(   0, atol(test_str[23]));
    T(   0, atol(test_str[24]));

    T( LONG_MIN, atol("-2147483648"));
    T( LONG_MAX, atol("2147483647"));
    T( LONG_MAX, atol("+2147483647"));
    T(-LONG_MAX, atol("-2147483647"));
    return 0;
}

int test_atoll(void) {
    T(   0, atoll(test_str[ 0]));
    T(   0, atoll(test_str[ 1]));
    T(   0, atoll(test_str[ 2]));
    T(   0, atoll(test_str[ 3]));
    T(   0, atoll(test_str[ 4]));
    T(   0, atoll(test_str[ 5]));
    T(   1, atoll(test_str[ 6]));
    T(   1, atoll(test_str[ 7]));
    T(  -1, atoll(test_str[ 8]));
    T(   0, atoll(test_str[ 9]));
    T(   0, atoll(test_str[10]));
    T(   0, atoll(test_str[11]));
    T(   0, atoll(test_str[12]));
    T(   0, atoll(test_str[13]));
    T(   0, atoll(test_str[14]));
    T( 100, atoll(test_str[15]));
    T( 100, atoll(test_str[16]));
    T(-100, atoll(test_str[17]));
    T(-123, atoll(test_str[18]));
    T( 321, atoll(test_str[19]));
    T(  99, atoll(test_str[20]));
    T(  42, atoll(test_str[21]));
    T(   0, atoll(test_str[22]));
    T(   0, atoll(test_str[23]));
    T(   0, atoll(test_str[24]));

    T( LLONG_MIN, atoll("-9223372036854775808"));
    T( LLONG_MAX, atoll("9223372036854775807"));
    T( LLONG_MAX, atoll("+9223372036854775807"));
    T(-LLONG_MAX, atoll("-9223372036854775807"));
    return 0;
}

void extra_strtol_test(void) {
    // https://en.cppreference.com/w/c/string/byte/strtol
    // parsing with error handling
    const char* p = "10 200000000000000000000000000000 30 -40 junk";
    printf("Parsing '%s':\n", p);
 
    for (;;) {
        // errno can be set to any non-zero value by a library function call
        // regardless of whether there was an error, so it needs to be cleared
        // in order to check the error set by strtol
        errno = 0;
        char* end = NULL;
        const long i = strtol(p, &end, 10);
        if (p == end) {
            break;
        }
 
        const bool range_error = errno == ERANGE;
        printf("Extracted '%.*s', strtol returned %ld.", (int)(end-p), p, i);
        p = end;
 
        if (range_error)
            printf("\n --> Range error occurred.");
 
        putchar('\n');
        while (!os_GetCSC());
    }
    printf("Unextracted leftover: '%s'\n\n", p);
}

int test_strtol(void) {
    char* endptr;

    T(   0, strtol(test_str[ 0], &endptr, 10)); C(endptr == test_str[ 0] + 0);
    T(   0, strtol(test_str[ 1], &endptr, 10)); C(endptr == test_str[ 1] + 0);
    T(   0, strtol(test_str[ 2], &endptr, 10)); C(endptr == test_str[ 2] + 0);
    T(   0, strtol(test_str[ 3], &endptr, 10)); C(endptr == test_str[ 3] + 1);
    T(   0, strtol(test_str[ 4], &endptr, 10)); C(endptr == test_str[ 4] + 2);
    T(   0, strtol(test_str[ 5], &endptr, 10)); C(endptr == test_str[ 5] + 2);
    T(   1, strtol(test_str[ 6], &endptr, 10)); C(endptr == test_str[ 6] + 1);
    T(   1, strtol(test_str[ 7], &endptr, 10)); C(endptr == test_str[ 7] + 2);
    T(  -1, strtol(test_str[ 8], &endptr, 10)); C(endptr == test_str[ 8] + 2);
    T(   0, strtol(test_str[ 9], &endptr, 10)); C(endptr == test_str[ 9] + 0);
    T(   0, strtol(test_str[10], &endptr, 10)); C(endptr == test_str[10] + 0);
    T(   0, strtol(test_str[11], &endptr, 10)); C(endptr == test_str[11] + 0);
    T(   0, strtol(test_str[12], &endptr, 10)); C(endptr == test_str[12] + 0);
    T(   0, strtol(test_str[13], &endptr, 10)); C(endptr == test_str[13] + 0);
    T(   0, strtol(test_str[14], &endptr, 10)); C(endptr == test_str[14] + 0);
    T( 100, strtol(test_str[15], &endptr, 10)); C(endptr == test_str[15] + 3);
    T( 100, strtol(test_str[16], &endptr, 10)); C(endptr == test_str[16] + 4);
    T(-100, strtol(test_str[17], &endptr, 10)); C(endptr == test_str[17] + 4);
    T(-123, strtol(test_str[18], &endptr, 10)); C(endptr == test_str[18] + 5);
    T( 321, strtol(test_str[19], &endptr, 10)); C(endptr == test_str[19] + 5);
    T(  99, strtol(test_str[20], &endptr, 10)); C(endptr == test_str[20] + 8);
    T(  42, strtol(test_str[21], &endptr, 10)); C(endptr == test_str[21] + 4);
    T(   0, strtol(test_str[22], &endptr, 10)); C(endptr == test_str[22] + 1);
    T(   0, strtol(test_str[23], &endptr, 10)); C(endptr == test_str[23] + 0);
    T(   0, strtol(test_str[24], &endptr, 10)); C(endptr == test_str[24] + 0);

    T(10, strtol("1010", NULL, 2));
    T(10, strtol("12",   NULL, 8));
    T(10, strtol("A",    NULL, 16));
    T(926192, strtol("junk", NULL, 36));
    T(926192, strtol("jUnK", NULL, 36));
    T(0, strtol("foobar", NULL, 37));
    T(0, strtol("fizzbuzz", NULL, -1));
    T(0, strtol("000?", NULL, 1));
    T(10, strtol("012",  NULL, 0));
    T(10, strtol("\f0xA",  NULL, 0));
    T(0xCAFE, strtol("0XCAFE", NULL, 16));
    T(0b1011100010, strtol("0b1011100010", NULL, 2));
    T(0b0100011101, strtol("0B0100011101", NULL, 0));
    T(0, strtol("junk", NULL, 0));

    errno = 0;
    T( LONG_MIN, strtol("-2147483648", NULL, 10));
    T( LONG_MAX, strtol("2147483647" , NULL, 10));
    T( LONG_MAX, strtol("+2147483647", NULL, 10));
    T(-LONG_MAX, strtol("-2147483647", NULL, 10));
    T( LONG_MIN, strtol("-10000000000000000000000000000000", NULL, 2));
    T( LONG_MAX, strtol("0b1111111111111111111111111111111", NULL, 0));
    T(-LONG_MAX, strtol(" -1111111111111111111111111111111", NULL, 2));
    C(errno == 0);

    T( LONG_MIN, strtol("-2147483649", NULL, 10));
    T( LONG_MAX, strtol("2147483648" , NULL, 10));
    T( LONG_MAX, strtol("+2147483648", NULL, 0));
    T( LONG_MIN, strtol("-2147483649", NULL, 0));
    T( LONG_MIN, strtol("-1qaz2WSX3edc4RFV", NULL, 36));
    T( LONG_MAX, strtol("+1qaz2WSX3edc4RFV", NULL, 36));

    {
        /**
         * @remarks If the first digit is '0' and the base is 0, then the
         * string should be treated as octal. This also implies that the string
         * contains a number, even if there are no more digits after the '0'.
         * Here we test that the string has been correctly classified as
         * containing a number, meaning that nptr != endptr.
         */
        char const * const octal_1 = "0";
        char const * const octal_2 = "\n09";
        T(0, strtol(octal_1, &endptr, 0));
        C(endptr == octal_1 + 1);
        T(0, strtol(octal_2, &endptr, 0));
        C(endptr == octal_2 + 2);
    }
    {
        /**
         * @remarks Make sure endptr is handled correctly when there is a
         * prefix without digits.
         */
        char const * const hex_prefix = "0x";
        char const * const bin_prefix = "\v-0b";
        T(0, strtol(hex_prefix, &endptr, 0));
        C(endptr == hex_prefix);
        T(0, strtol(hex_prefix, &endptr, 16));
        C(endptr == hex_prefix);
        T(0, strtol(bin_prefix, &endptr, 0));
        C(endptr == bin_prefix);
        T(0, strtol(bin_prefix, &endptr, 2));
        C(endptr == bin_prefix);
    }

    #if EXTRA_TESTS
    extra_strtol_test();
    #endif

    return 0;
}

/**
 * returns [0, 9] when ['0', '9']
 * returns [10, 35] when ['A', 'Z'] or ['a', 'z']
 * otherwise returns -1
 */
char char_to_digit(char c);

char char_to_digit_truth(char c) {
    if (c >= '0' && c <= '9') {
        return (c - '0');
    }
    if (c >= 'A' && c <= 'Z') {
        return (c - 'A') + 10;
    }
    if (c >= 'a' && c <= 'z') {
        return (c - 'a') + 10;
    }
    return -1;
}

int test_char_to_digit(void) {
    for (int i = 0; i < 255; i++) {
        char c = (char)i;
        char guess = char_to_digit(c);
        char truth = char_to_digit_truth(c);
        if (guess != truth) {
            test_printf("%02X: \'", (uint8_t)c);
            write_letter(c);
            test_printf("\' --> %d != %d\n", guess, truth);
            return __LINE__;
        }
    }
    return 0;
}

int run_tests(void) {
    int ret = 0;

    TEST(test_atoi());
    TEST(test_atol());
    TEST(test_atoll());
    TEST(test_strtol());
    TEST(test_char_to_digit());

    TEST_NAME(atoint_test(atoint_atoi    ), "atoi"    );
    TEST_NAME(atoint_test(atoint_atol    ), "atol"    );
    TEST_NAME(atoint_test(atoint_atoll   ), "atoll"   );
    TEST_NAME(atoint_test(atoint_strtol  ), "strtol"  );
    TEST_NAME(atoint_test(atoint_strtoul ), "strtoul" );
    TEST_NAME(atoint_test(atoint_strtoll ), "strtoll" );
    TEST_NAME(atoint_test(atoint_strtoull), "strtoull");

    TEST_NAME(strtoint_test(strtoint_strtol  ), "strtol"  );
    TEST_NAME(strtoint_test(strtoint_strtoul ), "strtoul" );
    TEST_NAME(strtoint_test(strtoint_strtoll ), "strtoll" );
    TEST_NAME(strtoint_test(strtoint_strtoull), "strtoull");

    return ret;
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
