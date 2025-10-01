#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <inttypes.h>
#include <ctype.h>

#define NO_FLOAT    0
#define USE_STRTOD  1
#define USE_STRTOF  2
#define USE_STRTOLD 3

/*============================================================================*/
/* Config                                                                     */
/*============================================================================*/

/* maximum characters to scan in matching sequences (such as "%3[^]^123abc]") */
#define SCAN_LIMIT 32

/**
 * Select the float conversion routine used
 * NO_FLOAT    : disables float conversion formats
 * USE_STRTOD  : `strtod`  (default)
 * USE_STRTOF  : `strtof`  (speed)
 * USE_STRTOLD : `strtold` (precision)
 */
#define STRING_TO_FLOAT_ROUTINE USE_STRTOLD

/* define to 0 or 1. Adds support for C23 '%b' format specifiers */
#define ENABLE_BINARY_CONVERSION_FORMAT 1

/*============================================================================*/
/* Code                                                                       */
/*============================================================================*/

#if SCAN_LIMIT < 4
#error "SCAN_LIMIT is too small"
#endif

#ifndef STRING_TO_FLOAT_ROUTINE
#error "STRING_TO_FLOAT_ROUTINE must be defined to a value"
#endif

#ifndef ENABLE_BINARY_CONVERSION_FORMAT
#error "ENABLE_BINARY_CONVERSION_FORMAT must be defined to 0 or 1"
#endif

#define TEST_LENGTH_MODIFIER() do { \
    if (ptr_size == 0) { \
        /* invalid length modifier */ \
        return assignment_count; \
    } \
} while (0)

#define CONSUME_WHITESPACE() while (isspace(*buf)) { buf++; }

#define RETURN_IF_NULL(ptr) if ((ptr) == NULL) { return assignment_count; }

#if UINTMAX_MAX < ULLONG_MAX
#error "UINTMAX_MAX needs to be greater than or equal ULLONG_MAX"
#endif

/**
 * @author zerico2005 (Originally based off of https://github.com/tusharjois/bscanf)
 * @note All character sequence types must have a maximum field width:
 *  - This looks something like: "%*3c %8s %12[^abc]"
 *  - The following will not work: "%*c %s %[^abc]"
 *
 * @note `wchar_t` is not supported
 */
int _vsscanf_c(
    char const * const __restrict Buffer,
    char const * const __restrict Format,
    va_list args
) {
    char scan_buf[SCAN_LIMIT];
    int assignment_count = 0;
    char const *__restrict buf = Buffer;
    char const *__restrict fmt = Format;
    if (buf == NULL || fmt == NULL) {
        return EOF;
    }
    while (*fmt != '\0') {
        if (isspace(*fmt)) {
            CONSUME_WHITESPACE();
            fmt++;
            continue;
        }
        if (*fmt != '%') {
            if (*fmt != *buf) {
                /* end of format */
                return assignment_count;
            }
            fmt++;
            buf++;
            continue;
        }
        /* conversion specifier */
        fmt++;
        bool is_suppressed = false;
        bool is_long_double = false;
        bool is_double_or_wide_char = false;
        size_t max_width = 0;
        size_t ptr_size = sizeof(int);
        if (*fmt == '*') {
            is_suppressed = true;
            fmt++;
        }
        /* test for digits */
        if (isdigit(*fmt)) {
            char *endptr;
            max_width = (size_t)strtoul(fmt, &endptr, 10);
            if (max_width == 0 || fmt == endptr) {
                /* failed */
                return assignment_count;
            }
            fmt = endptr;
        }
        /* test for length modifiers */
        switch (*fmt) {
            case 'h':
            {
                ptr_size = sizeof(short);
                fmt++;
                if (*fmt == 'h') {
                    ptr_size = sizeof(char);
                    fmt++;
                }
            } break;
            case 'z':
            {
                ptr_size = sizeof(size_t);
                fmt++;
            } break;
            case 'l':
            {
                is_double_or_wide_char = true;
                ptr_size = sizeof(long);
                fmt++;
                if (*fmt == 'l') {
                    ptr_size = sizeof(long long);
                    fmt++;
                }
            } break;
            case 't':
            {
                ptr_size = sizeof(ptrdiff_t);
                fmt++;
            } break;
            case 'L':
            {
                is_long_double = true;
                ptr_size = 0;
                fmt++;
            } break;
            case 'j':
            {
                ptr_size = sizeof(intmax_t);
                fmt++;
            } break;
            default: break;
        }
        /**
         * @remarks All conversion specifiers other than 'n' 'c' '[' consume
         * and discard all leading whitespace characters.
         */
        if (*fmt != 'n' && *fmt != 'c' && *fmt != '[') {
            CONSUME_WHITESPACE();
        }
        /* test for format type */
        switch (*fmt) {
            case '%':
            /* handle "%%" */ {
                if (*fmt != *buf) {
                    return assignment_count;
                }
                buf++;
                fmt++;
                continue;
            }
            case 'n':
            /* number of characters read so far */ {
                if (is_suppressed) {
                    /* "%*n" is undefined behaviour */
                    return assignment_count;
                }
                TEST_LENGTH_MODIFIER();
                void* ptr = va_arg(args, void*);
                RETURN_IF_NULL(ptr);
                unsigned long long diff = buf - Buffer;
                memcpy(ptr, &diff, ptr_size);
                fmt++;
                /* assignment_count is not incremented for %n */
                continue;
            } break;
            case 'c':
            case 's':
            /* string */ {
                const bool string_format = (*fmt == 's');
                if (max_width == 0) {
                    /* enforce bounds checking */
                    return assignment_count;
                }
                if (is_double_or_wide_char) {
                    /* unimplemented */
                    return assignment_count;
                }
                char const *__restrict const begin = buf;
                for (; max_width --> 0;) {
                    if (*buf == '\0') {
                        break;
                    }
                    if (string_format && isspace(*buf)) {
                        break;
                    }
                    buf++;
                }
                size_t copy_size = buf - begin;
                if (!is_suppressed) {
                    char* ptr = va_arg(args, char*);
                    RETURN_IF_NULL(ptr);
                    memcpy(ptr, begin, copy_size);
                    if (string_format) {
                        /* null terminate */
                        *(ptr + copy_size) = '\0';
                    }
                }
                fmt++;
                continue;
            } break;
            case '[':
            /* match range */ {
                fmt++;
                if (max_width == 0) {
                    /* enforce bounds checking */
                    return assignment_count;
                }
                if (is_double_or_wide_char) {
                    /* unimplemented */
                    return assignment_count;
                }
                bool invert_match = false;
                bool starts_with_bracket = false;
                if (*fmt == '^') {
                    invert_match = true;
                    fmt++;
                }
                if (*fmt == ']') {
                    starts_with_bracket = true;
                    fmt++;
                }
                char const *__restrict last_bracket = strchr(fmt, ']');
                if (last_bracket == NULL) {
                    if (!(invert_match && starts_with_bracket)) {
                        return assignment_count;
                    }
                    /* special case of "[^]" "*/
                    scan_buf[0] = '^';
                    scan_buf[1] = '\0';
                } else {
                    if (starts_with_bracket) {
                        fmt--;
                        /* *fmt == ']' */
                    }
                    /* fmt + scan_length points to the ending ']', so move it back */
                    size_t scan_length = (last_bracket - fmt) - 1;
                    if (scan_length >= SCAN_LIMIT) {
                        /* too many characters */
                        return assignment_count;
                    }
                    memcpy(scan_buf, fmt, scan_length);
                    /* move format to the character after the ending ']' */
                    fmt = last_bracket + 1;
                    /* null terminate */
                    *(scan_buf + scan_length) = '\0';
                }

                size_t match_length;
                if (invert_match) {
                    match_length = strcspn(buf, scan_buf);
                } else {
                    match_length = strspn(buf, scan_buf);
                }
                if (!is_suppressed) {
                    char* ptr = va_arg(args, char*);
                    RETURN_IF_NULL(ptr);
                    memcpy(ptr, buf, match_length);
                    /* null terminate */
                    *(ptr + match_length) = '\0';
                }
                /* move buf to the character after the last matched character */
                buf += match_length;
                buf++;
                continue;
            } break;
            case 'i':
            case 'd':
            /* signed integer */ {
                TEST_LENGTH_MODIFIER();
                char *endptr;
                int base = ((*fmt == 'd') ? 10 : 0);
                intmax_t value = strtoimax(buf, &endptr, base);
                if (!is_suppressed) {
                    void* ptr = va_arg(args, void*);
                    RETURN_IF_NULL(ptr);
                    memcpy(ptr, &value, ptr_size);
                }
                if (buf == endptr) {
                    /* failed */
                    return assignment_count;
                }
                buf = endptr;
                fmt++;
                assignment_count++;
            } break;
        #if ENABLE_BINARY_CONVERSION_FORMAT
            case 'b':
        #endif /* ENABLE_BINARY_CONVERSION_FORMAT */
            case 'u':
            case 'o':
            case 'x':
            case 'X':
            case 'p':
            /* unsigned integer or pointer */ {
                TEST_LENGTH_MODIFIER();
                char *endptr;
                int base = 10;
                if (*fmt == 'X' || *fmt == 'x' || *fmt == 'p') {
                    base = 16;
                    if (*fmt == 'p') {
                        ptr_size = sizeof(void*);
                    }
            #if ENABLE_BINARY_CONVERSION_FORMAT
                } else if (*fmt == 'b') {
                    base = 2;
            #endif /* ENABLE_BINARY_CONVERSION_FORMAT*/
                } else if (*fmt == 'o') {
                    base = 8;
                }
                uintmax_t value = strtoumax(buf, &endptr, base);
                if (!is_suppressed) {
                    void* ptr = va_arg(args, void*);
                    RETURN_IF_NULL(ptr);
                    memcpy(ptr, &value, ptr_size);
                }
                if (buf == endptr) {
                    /* failed */
                    return assignment_count;
                }
                buf = endptr;
                fmt++;
                assignment_count++;
            } break;
        #if STRING_TO_FLOAT_ROUTINE
            case 'a':
            case 'A':
            case 'e':
            case 'E':
            case 'f':
            case 'F':
            case 'g':
            case 'G':
            /* float */ {
                char *endptr;
                #if STRING_TO_FLOAT_ROUTINE == USE_STRTOD
                    double value = strtod(buf, &endptr);
                #elif STRING_TO_FLOAT_ROUTINE == USE_STRTOF
                    float value = strtof(buf, &endptr);
                #elif STRING_TO_FLOAT_ROUTINE == USE_STRTOLD
                    long double value = strtold(buf, &endptr);
                #else
                    #error "invalid STRING_TO_FLOAT_ROUTINE value"
                #endif
                if (!is_suppressed) {
                    void* ptr = va_arg(args, void*);
                    RETURN_IF_NULL(ptr);
                    if (is_long_double) {
                        *(long double*)ptr = (long double)value;
                    } else if (is_double_or_wide_char) {
                        *(double*)ptr = (double)value;
                    } else {
                        *(float*)ptr = (float)value;
                    }
                }
                if (buf == endptr) {
                    /* failed */
                    return assignment_count;
                }
                buf = endptr;
                fmt++;
                assignment_count++;
            } break;
        #endif /* STRING_TO_FLOAT_ROUTINE */
            default:
            /* unknown format */ {
                return assignment_count;
            } break;
        }
    }
    return assignment_count;
}

int _sscanf_c(const char *__restrict buffer, const char *__restrict format, ...)
{
    va_list vlist;
    va_start(vlist, format);
    int ret = vsscanf(buffer, format, vlist);
    va_end(vlist);
    return ret;
}
