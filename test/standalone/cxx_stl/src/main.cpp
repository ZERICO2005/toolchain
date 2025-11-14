#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <ti/sprintf.h>
#include <cstdio>
#include <iostream>
#include <iomanip>

#define TEST(test) { ret = test; if (ret != 0) { return ret; }}

int test_basic();
int test_apply();
int test_tuple();
int test_span();
int test_charconv();
int test_random();

extern "C" void outchar(char ch) {
    char *ptr = (char*)0xFB0000;
    *ptr++ = ch;
    *ptr++ = '\0';
}


extern "C" void ti_outchar(char ch);

void ti_puts(const char* str) {
    while(*str != '\0') {
        ti_outchar(*str);
        str++;
    }
}

int run_tests(void) {
    int ret = 0;

    TEST(test_basic());
    // TEST(test_apply());
    TEST(test_tuple());
    // TEST(test_span());
    // TEST(test_charconv());
    // TEST(test_random());

    return ret;
}

int main(void) {

    std::ios_base::Init();

    int n = 0x7184CE;

    std::cout << std::hex << n << std::endl;

    os_ClrHome();
    int failed_test = run_tests();
    if (failed_test != 0) {
        char buf[sizeof("Failed test L-8388608\n")];
        boot_sprintf(buf, "Failed test L%d\n", failed_test);
        ti_puts(buf);
    } else {
        ti_puts("All tests passed");
    }

    while (!os_GetCSC());

    return 0;
}
