#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <cstdio>
#include <iostream>
#include <cstdarg>

#if 0
extern "C" void outchar(char ch) {
    char *ptr = (char*)0xFB0000;
    *ptr++ = ch;
    *ptr++ = '\0';
}
#endif

int main(void) {
    init_iostream();

    int n = 123;
    std::cout << n << std::endl;
    printf("hello %d\n", n);

    return 0;
}
