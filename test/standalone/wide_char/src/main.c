#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <ti/sprintf.h>
#include <wchar.h>

#define C(expr) if (!(expr)) { return __LINE__; }

int run_tests(void) {
    const wchar_t str_0[] = {0x0000, 0xCAFE, 0x0000};
    const wchar_t str_1[] = {0x1111, 0x0000, 0x0000};
    const wchar_t str_2[] = {0x2200, 0x0000, 0x0000};
    const wchar_t str_3[] = {0x0033, 0x0000, 0x0000};
    const wchar_t str_4[] = {0x4444, 0x4444, 0x0000, 0xCAFE, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000};
    const wchar_t str_5[] = {0x0055, 0x0055, 0x0000, 0xCAFE, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000};
    const wchar_t str_6[] = {0x6600, 0x6600, 0x0000, 0xCAFE, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000};
    const wchar_t str_7[] = {0x7700, 0x0077, 0x0000, 0xCAFE, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000};
    const wchar_t str_8[] = {0x0088, 0x0099, 0xAA00, 0x00BB, 0x00CC, 0xDDDD, 0xEE00, 0xFFFF, 0x0000};
    printf("%zu, ", wcslen(L"")  );
    printf("%zu, ", wcslen(str_0));
    printf("%zu, ", wcslen(str_1));
    printf("%zu, ", wcslen(str_2));
    printf("%zu, ", wcslen(str_3));
    printf("%zu, ", wcslen(str_4));
    printf("%zu, ", wcslen(str_5));
    printf("%zu, ", wcslen(str_6));
    printf("%zu, ", wcslen(str_7));
    printf("%zu, ", wcslen(str_8));
    C( 0 == wcslen(L"")  );
    C( 0 == wcslen(str_0));
    C( 1 == wcslen(str_1));
    C( 1 == wcslen(str_2));
    C( 1 == wcslen(str_3));
    C( 2 == wcslen(str_4));
    C( 2 == wcslen(str_5));
    C( 2 == wcslen(str_6));
    C( 2 == wcslen(str_7));
    C( 8 == wcslen(str_8));
    C(50 == wcslen(L"How many wide characters does this string contain?"));
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
