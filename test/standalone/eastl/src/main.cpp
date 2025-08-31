#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <cstdio>

int test();

int main(void) {
    os_ClrHome();
    int failed_test = test();
    if (failed_test != 0) {
        std::printf("Failed test L%d\n", failed_test);
    } else {
        std::printf("All tests passed");
    }

    while (!os_GetCSC());

    return 0;
}
