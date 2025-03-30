#include <iostream>
#include <cstdio>

std::ostream cout;
std::ostream cerr;
std::ostream clog;
std::istream cin;

void __attribute__((weak)) dConsolePut(const char * str) {
    fputs(str, stdout);
}

void __attribute__((weak)) dConsolePutChar(char c) {
    fputc(c, stdout);
}
