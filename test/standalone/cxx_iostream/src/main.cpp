#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <cstdio>
#include <iostream>
#include <iomanip>

#if 1
extern "C" void outchar(char ch) {
    char *ptr = (char*)0xFB0000;
    *ptr++ = ch;
    *ptr++ = '\0';
}
#endif

extern "C" {
    int main(void);
}

int main(void) {

    os_ClrHome();

    std::ios_base::Init();

    while (!os_GetCSC());

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 123;

    std::cout << n << std::endl;

    fprintf(stdout, "hello %d\n %f\n", n, 123.424f);

    std::cout << 123.424f << std::endl;

    while (!os_GetCSC());

    return 0;
}
