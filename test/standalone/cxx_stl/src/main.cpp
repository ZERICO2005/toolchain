#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <ti/sprintf.h>
#include <cstdio>

#define TEST(test) { ret = test; if (ret != 0) { return ret; }}

int test_basic();
int test_apply();
int test_tuple();
int test_span();

extern "C" void outchar(char ch) {
    char *ptr = (char*)0xFB0000;
    *ptr++ = ch;
    *ptr++ = '\0';
}

int run_tests(void) {
    int ret = 0;

    TEST(test_basic());
    TEST(test_apply());
    TEST(test_tuple());
    TEST(test_span());

    return ret;
}

int main(void) {
    os_ClrHome();
    int failed_test = run_tests();
    if (failed_test != 0) {
        char buf[sizeof("Failed test L-8388608\n")];
        boot_sprintf(buf, "Failed test L%d\n", failed_test);
        std::fputs(buf, stdout);
    } else {
        std::fputs("All tests passed", stdout);
    }

    // while (!os_GetCSC());

    return 0;
}
