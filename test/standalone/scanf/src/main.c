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
#include <math.h>
#include <ti/sprintf.h>

//------------------------------------------------------------------------------
// Config
//------------------------------------------------------------------------------

// define to 0 or 1
#define DEBUG_DIAGNOSTICS 0

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------

#define C(expr) if (!(expr)) { return __LINE__; }

#define TEST(test) { ret = test; if (ret != 0) { return ret; }}

#ifndef DEBUG_DIAGNOSTICS
#error "DEBUG_DIAGNOSTICS needs to be defined to 0 or 1"
#endif

#if DEBUG_DIAGNOSTICS
#define test_printf printf
#else
#define test_printf(...)
#endif

bool double_equals(double x, double y, double absolute_difference) {
    return (fabs(x - y) <= absolute_difference);
}

int basic_test(void) {
    int i, j;
    float x, y;
    char str1[10], str2[4];

    char input[] = "25 54.32E-1 Thompson 56789 0123 56";
    /**
     * https://en.cppreference.com/w/c/io/fscanf
     * parse as follows:
     * %d: an integer
     * %f: a floating-point value
     * %9s: a string of at most 9 non-whitespace characters
     * %2d: two-digit integer (digits 5 and 6)
     * %f:  a floating-point value (digits 7, 8, 9)
     * %*d: an integer which isn't stored anywhere
     * ' ': all consecutive whitespace
     * %3[0-9]: a string of at most 3 decimal digits (digits 5 and 6)
     */
    int ret = sscanf(
        input, "%d%f%9s%2d%f%*d %3[0-9]",
        &i, &x, str1, &j, &y, str2
    );
    printf("Converted %d fields:\n"
        "i = %d\n"
        "x = %f\n"
        "str1 = %s\n"
        "j = %d\n"
        "y = %f\n"
        "str2 = %s\n",
        ret, i, x, str1, j, y, str2);
    C(ret == 6);
    C(i == 25);
    C(double_equals(x, 5.432000, 1.0e-5));
    C(strlen(str1) == strlen("Thompson"))
    C(strcmp(str1, "Thompson") == 0);
    C(j == 56);
    C(y == 789.000000);
    C(strlen(str2) == strlen("56"));
    C(strcmp(str2, "56") == 0);
    return 0;
}

int run_tests(void) {
    int ret = 0;
    TEST(basic_test());

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
