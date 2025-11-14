#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <cstdio>
#include <iostream>
#include <cstdarg>

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

int my_vprintf(const char *__restrict format, va_list vlist)
{
  return vfprintf(stdout, format, vlist);
}

int my_printf(char const *__restrict format, ...) {
  va_list va;
  va_start(va, format);
  const int ret = my_vprintf(format, va);
  va_end(va);
  return ret;
}

int main(void) {
    std::ios_base::Init();

    int n = 123;
    std::cout << n << std::endl;
    // fprintf(stdout, "hello %d\n", n);
    my_printf("hello %d\n", n);

    return 0;
}
