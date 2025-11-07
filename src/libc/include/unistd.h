#ifndef _UNISTD_H
#define _UNISTD_H

/**
 * @file unistd.h
 *
 * @note this is a very minimal and non-compliant unistd.h header
 */

#include <cdefs.h>
#include <stdlib.h>
#include <sys/timers.h>

__BEGIN_DECLS

int isatty(int);

void _exit(int) __NOEXCEPT __attribute__((noreturn));

__END_DECLS

#endif /* _UNISTD_H */
