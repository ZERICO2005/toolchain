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

#define SINK (wchar_t*)0xE40000

int run_tests(void) {
    {
        const wchar_t str_0[] = {0x0000, 0xCAFE, 0x0000};
        const wchar_t str_1[] = {0x1111, 0x0000, 0x0000};
        const wchar_t str_2[] = {0x2200, 0x0000, 0x0000};
        const wchar_t str_3[] = {0x0033, 0x0000, 0x0000};
        const wchar_t str_4[] = {0x4444, 0x4444, 0x0000, 0xCAFE, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000};
        const wchar_t str_5[] = {0x0055, 0x0055, 0x0000, 0xCAFE, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000};
        const wchar_t str_6[] = {0x6600, 0x6600, 0x0000, 0xCAFE, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000};
        const wchar_t str_7[] = {0x7700, 0x0077, 0x0000, 0xCAFE, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000};
        const wchar_t str_8[] = {0x0088, 0x0099, 0xAA00, 0x00BB, 0x00CC, 0xDDDD, 0xEE00, 0xFFFF, 0x0000};
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
    }

    {
        // test zero byte case
        void* ptr = wmemcpy((void*)0xC0FFEE, (void*)0x123456, 0);
        if (ptr != (void*)0xC0FFEE) {
            printf("%p != %p\n", ptr, (void*)0xC0FFEE);
            return __LINE__;
        }
        wchar_t data[192 + 1];
        wmemset(wmemset(&data[  0], 0x12, 64) + 64, 0x12, 64);
        wmemset(&data[128], 0x56, 64);
        wchar_t append[64 + 1];
        wmemset(append, 0x34, 64);
        wchar_t* res = wmemcpy(&data[64], append, 64);
        if (res != &data[64]) {
            printf("%p != %p\n", res, &data[64]);
            return __LINE__;
        }

        wchar_t truth[192 + 1];
        wmemset(&truth[  0], 0x12, 64);
        wmemset(&truth[ 64], 0x34, 64);
        wmemset(&truth[128], 0x56, 64);

        int cmp = wmemcmp(data, truth, 192);
        if (cmp != 0) {
            printf("cmp: %d\n", cmp);
            return __LINE__;
        }
    }

    {
        C(wmemchr(NULL, 0x0000, 0) == NULL);
        C(wmemchr(NULL, 0xFFFF, 0) == NULL);

        C(wmemchr(SINK, 0x0000, 0) == NULL);
        C(wmemchr(SINK, 0x0000, 1) == SINK);
        C(wmemchr(SINK, 0x00FF, 1) == NULL);
        C(wmemchr(SINK, 0xFF00, 1) == NULL);
        C(wmemchr(SINK, 0xFFFF, 1) == NULL);
        C(wmemchr(SINK, 0x0000, 2) == SINK);
        C(wmemchr(SINK, 0x0000, 10) == SINK);
        C(wmemchr(SINK, 0x00FF, 10) == NULL);
        C(wmemchr(SINK, 0xFF00, 10) == NULL);
        C(wmemchr(SINK, 0xFFFF, 10) == NULL);
        const wchar_t test0[] = L"GABCDEFABCDEF";
        wchar_t const * const test = &test0[1];

        const size_t test_size = (sizeof(test0) / sizeof(test0[0])) - 1;
        const size_t test0_size = (sizeof(test0) / sizeof(test0[0]));
        const size_t test_strlen = (sizeof(test0) / sizeof(test0[0])) - 2;

        C(wmemchr(test, '\0', test_size) == &test[12]);
        C(wmemchr(test0, 'A', test_strlen) == &test0[1]);
        C(wmemchr(test, 'A', test_strlen) == &test[0]);
        C(wmemchr(&test[7], 'A', 5) == NULL);
        C(wmemchr(&test[6], 'A', 6) == &test[6]);
        C(wmemchr(&test[5], 'A', 7) == &test[6]);
        C(wmemchr(&test[7], 'B', 5) == &test[7]);
        C(wmemchr(&test[8], 'C', 1) == &test[8]);
        C(wmemchr(&test[8], 'C', 8) == &test[8]);
        C(wmemchr(test, 'G', test_strlen) == NULL);
        C(wmemchr(test, 'G', test_size) == NULL);
        C(wmemchr(test0, 'G', test0_size) == test0);
    }

    {
        const wchar_t a1[] = {L'α',L'β',L'γ'};
        const wchar_t a2[] = {L'α',L'β',L'δ'};
    
        const size_t sz = (sizeof(a1) / sizeof(a1[0]));
        C(wmemcmp(a1, a2, sz) < 0);
        C(wmemcmp(a2, a1, sz) > 0);
        C(wmemcmp(a1, a1, sz) == 0);
        C(wmemcmp(a1, a2, 0) == 0);
        C(wmemcmp(a1, a2, 1) == 0);
        C(wmemcmp(a1 + 2, a2 + 2, 0) == 0);
        C(wmemcmp(a1 + 2, a2 + 2, 1) < 0);
    }

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
