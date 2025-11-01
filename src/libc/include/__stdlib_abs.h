#ifndef _STDLIB_ABS_H
#define _STDLIB_ABS_H

#ifdef __cplusplus
extern "C" {
#endif

int abs(int);

long labs(long);

#ifdef __SIZEOF_INT48__
signed __int48 i48abs(signed __int48 n) __NOEXCEPT_CONST;
#endif /* __SIZEOF_INT48__ */

long long llabs(long long);

#ifdef __cplusplus
}
#endif

#endif /* _STDLIB_ABS_H */
