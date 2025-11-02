#ifndef _MBSTATE_H
#define _MBSTATE_H

#include <__wchar_def.h>

typedef struct {
    int __count;
    union {
        wint_t __value;
        unsigned char __bytes[sizeof(wint_t)];
    };
} mbstate_t;

#endif /* _MBSTATE_H */
