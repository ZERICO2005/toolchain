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

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------

long my_strtol(const char*, char**, int);
#define strtol my_strtol


int test_atoi(void) {
    T(   0, atoi(""             ));
    T(   0, atoi("+"            ));
    T(   0, atoi("-"            ));
    T(   0, atoi("0"            ));
    T(   0, atoi("+0"           ));
    T(   0, atoi("-0"           ));
    T(   1, atoi("1"            ));
    T(   1, atoi("+1"           ));
    T(  -1, atoi("-1"           ));
    T(   0, atoi("+-84"         ));
    T(   0, atoi("--84"         ));
    T(   0, atoi("-+84"         ));
    T(   0, atoi("++84"         ));
    T(   0, atoi("+ 84"         ));
    T(   0, atoi("- 84"         ));
    T( 100, atoi("100"          ));
    T( 100, atoi("+100"         ));
    T(-100, atoi("-100"         ));
    T(-123, atoi(" -123junk"    ));
    T( 321, atoi(" +321dust"    ));
    T(  99, atoi(" \f\n\r\t\v99"));
    T(  42, atoi("0042"         ));
    T(   0, atoi("0x2A"         ));
    T(   0, atoi("junk"         ));
    T(   0, atoi("a701"         ));

    T( INT_MIN, atoi("-8388608" ));
    T( INT_MAX, atoi("8388607"  ));
    T( INT_MAX, atoi("+8388607" ));
    T(-INT_MAX, atoi("-8388607" ));
    return 0;
}

int test_atol(void) {
    T(   0, atol(""             ));
    T(   0, atol("+"            ));
    T(   0, atol("-"            ));
    T(   0, atol("0"            ));
    T(   0, atol("+0"           ));
    T(   0, atol("-0"           ));
    T(   1, atol("1"            ));
    T(   1, atol("+1"           ));
    T(  -1, atol("-1"           ));
    T(   0, atol("+-84"         ));
    T(   0, atol("--84"         ));
    T(   0, atol("-+84"         ));
    T(   0, atol("++84"         ));
    T(   0, atol("+ 84"         ));
    T(   0, atol("- 84"         ));
    T( 100, atol("100"          ));
    T( 100, atol("+100"         ));
    T(-100, atol("-100"         ));
    T(-123, atol(" -123junk"    ));
    T( 321, atol(" +321dust"    ));
    T(  99, atol(" \f\n\r\t\v99"));
    T(  42, atol("0042"         ));
    T(   0, atol("0x2A"         ));
    T(   0, atol("junk"         ));
    T(   0, atol("a701"         ));

    T( LONG_MIN, atol("-2147483648"));
    T( LONG_MAX, atol("2147483647"));
    T( LONG_MAX, atol("+2147483647"));
    T(-LONG_MAX, atol("-2147483647"));
    return 0;
}

int test_atoll(void) {
    T(   0, atoll(""             ));
    T(   0, atoll("+"            ));
    T(   0, atoll("-"            ));
    T(   0, atoll("0"            ));
    T(   0, atoll("+0"           ));
    T(   0, atoll("-0"           ));
    T(   1, atoll("1"            ));
    T(   1, atoll("+1"           ));
    T(  -1, atoll("-1"           ));
    T(   0, atoll("+-84"         ));
    T(   0, atoll("--84"         ));
    T(   0, atoll("-+84"         ));
    T(   0, atoll("++84"         ));
    T(   0, atoll("+ 84"         ));
    T(   0, atoll("- 84"         ));
    T( 100, atoll("100"          ));
    T( 100, atoll("+100"         ));
    T(-100, atoll("-100"         ));
    T(-123, atoll(" -123junk"    ));
    T( 321, atoll(" +321dust"    ));
    T(  99, atoll(" \f\n\r\t\v99"));
    T(  42, atoll("0042"         ));
    T(   0, atoll("0x2A"         ));
    T(   0, atoll("junk"         ));
    T(   0, atoll("a701"         ));

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
    T(   0, strtol(""             , NULL, 10));
    T(   0, strtol("+"            , NULL, 10));
    T(   0, strtol("-"            , NULL, 10));
    T(   0, strtol("0"            , NULL, 10));
    T(   0, strtol("+0"           , NULL, 10));
    T(   0, strtol("-0"           , NULL, 10));
    T(   1, strtol("1"            , NULL, 10));
    T(   1, strtol("+1"           , NULL, 10));
    T(  -1, strtol("-1"           , NULL, 10));
    T(   0, strtol("+-84"         , NULL, 10));
    T(   0, strtol("--84"         , NULL, 10));
    T(   0, strtol("-+84"         , NULL, 10));
    T(   0, strtol("++84"         , NULL, 10));
    T(   0, strtol("+ 84"         , NULL, 10));
    T(   0, strtol("- 84"         , NULL, 10));
    T( 100, strtol("100"          , NULL, 10));
    T( 100, strtol("+100"         , NULL, 10));
    T(-100, strtol("-100"         , NULL, 10));
    T(-123, strtol(" -123junk"    , NULL, 10));
    T( 321, strtol(" +321dust"    , NULL, 10));
    T(  99, strtol(" \f\n\r\t\v99", NULL, 10));
    T(  42, strtol("0042"         , NULL, 10));
    T(   0, strtol("0x2A"         , NULL, 10));
    T(   0, strtol("junk"         , NULL, 10));
    T(   0, strtol("a701"         , NULL, 10));

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
        char* endptr_test;
    
        /**
         * @remarks If the first digit is '0' and the base is 0, then the
         * string should be treated as octal. This also implies that the string
         * contains a number, even if there are no more digits after the '0'.
         * Here we test that the string has been correctly classified as
         * containing a number, meaning that nptr != endptr.
         */
        char const * const octal_1 = "0";
        char const * const octal_2 = "\n09";
        T(0, strtol(octal_1, &endptr_test, 0));
        C(endptr_test == octal_1 + 1);
        T(0, strtol(octal_2, &endptr_test, 0));
        C(endptr_test == octal_2 + 2);

        char const * const hex_1 = "0x";
        char const * const hex_2 = "0b";
        T(0, strtol(hex_1, &endptr_test, 0));
        printf("%p %p %td\n", hex_1, endptr_test, hex_1 - endptr_test);
        C(endptr_test == hex_1 + 1);
        T(0, strtol(hex_2, &endptr_test, 0));
        C(endptr_test == hex_2 + 1);
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
