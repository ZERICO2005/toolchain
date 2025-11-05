#include <stddef.h>
#include <stdlib.h>

void* __dso_handle(void* ptr) {
    (void)ptr;
    return NULL;
}

void* __cxa_atexit(void* ptr) {
    (void)ptr;
    return NULL;
}

void* __cxa_guard_release(void* ptr) {
    (void)ptr;
    return NULL;
}

void* __cxa_guard_acquire(void* ptr) {
    (void)ptr;
    return NULL;
}
