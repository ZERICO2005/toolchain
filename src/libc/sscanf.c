#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <inttypes.h>
#include <ctype.h>

/* maximum characters to scan in %3[^]^123abc] type strings*/
#define SCAN_LIMIT 32

#if SCAN_LIMIT < 4
#error "SCAN_LIMIT is too small"
#endif

#define DEFAULT_MAX_WIDTH 65535

#define TEST_LENGTH_MODIFIER() do { \
    if ( \
        long_double || \
        ptr_size == 0 || \
        ptr_size > sizeof(uintmax_t) \
    ) { \
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
 * @author https://github.com/tusharjois/bscanf
 * @remarks modified version of bscanf that removes the mandatory maximum
 * field-width feature.
 */
int _vsscanf_c(
    char const * const __restrict Buffer,
    char const * const __restrict Format,
    va_list args
) {
    __attribute__((__unused__)) char scan_buf[SCAN_LIMIT];
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
        bool long_double = false;
        bool wide_char = false;
        size_t max_width = 0;
        size_t ptr_size = 3;
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
                ptr_size = sizeof(long);
                fmt++;
                if (*fmt == 'l') {
                    ptr_size = sizeof(long long);
                    fmt++;
                } else {
                    wide_char = true;
                }
            } break;
            case 't':
            {
                ptr_size = 4;
                fmt++;
            } break;
            case 'L':
            {
                long_double = true;
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
                // assignment_count++;
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
                if (wide_char) {
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
                if (wide_char) {
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
                        ptr_size = 3;
                    }
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
                long double value = strtold(buf, &endptr);
                if (!is_suppressed) {
                    void* ptr = va_arg(args, void*);
                    RETURN_IF_NULL(ptr);
                    if (long_double) {
                        *(long double*)ptr = (long double)value;
                    } else {
                        *(double*)ptr = (double)value;
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
            default:
            /* unknown format */ {
                return assignment_count;
            } break;
        }
    }
    return assignment_count;
}

#if 0

#define _BSCANF_CONSUME_WSPACE() while (isspace(*buf_ptr)) {buf_ptr++;}
#define _BSCANF_CHECK(x) if (!(x)) goto exit;
#define _BSCANF_MATCH() _BSCANF_CHECK(*buf_ptr == *fmt_ptr);
#define _BSCANF_CHECK_NULL(ptr) _BSCANF_CHECK(NULL != ptr);
#define _BSCANF_CHECK_STRING() _BSCANF_CHECK(0 != max_width);
#define _BSCANF_CHECK_BUFFER() _BSCANF_CHECK('\0' != *buffer);
#define _BSCANF_CHECK_STRTONUM() _BSCANF_CHECK(buf_ptr != end_ptr);

/**
 * @author https://github.com/tusharjois/bscanf
 * @remarks modified version of bscanf that removes the mandatory maximum
 * field-width feature.
 */
int _vsscanf_c(const char *__restrict buffer, const char *__restrict format, va_list args)
{
    __attribute__((__unused__)) char scan_buf[SCAN_LIMIT];
    /* Our return value. On a conversion error, we return this immediately. */
    int num_args_set = 0;

    /* We use these to index into our buffer and format string. */
    const char *__restrict buf_ptr = buffer;
    const char *__restrict fmt_ptr = format;

    /* Variadic arguments -- pointers in which we put our conversion results. */
    long *long_ptr;
    int *int_ptr;
    short *short_ptr;
    unsigned long *ulong_ptr;
    unsigned short *ushort_ptr;
    unsigned int *uint_ptr;
    double *double_ptr;
    float *float_ptr;
    char *char_ptr;
    __attribute__((__unused__)) wchar_t *wchar_ptr;

    /* These are useful variables when doing string to number conversion. */
    char* end_ptr;
    int base;

    /* These are flags that are used by different conversion specifiers. */
    bool is_suppressed = false;
    size_t max_width = 0;
    char length_mod = '\0';

    /* Return a special value when one of the arguments is NULL. */
    if (NULL == buffer || NULL == format) {
        return EOF;
    }

    while ('\0' != *fmt_ptr) {
        /* We ignore spaces before specifiers. */
        if (isspace(*fmt_ptr)) {
            /* Any whitespace in the format consumes all of the whitespace in the
                 buffer. */
            _BSCANF_CONSUME_WSPACE();
            fmt_ptr++;
            continue;
        }

        if ('%' == *fmt_ptr) {
            /* Handle conversion specifier. */
            fmt_ptr++;

            /* Check for assignment-suppressing character. */
            if ('*' == *fmt_ptr) {
                is_suppressed = true;
                fmt_ptr++;
            } else {
                is_suppressed = false;
            }

            /* Check for maximum field width. */
            if (isdigit(*fmt_ptr)) {
                max_width = strtoul(fmt_ptr, &end_ptr, 10);
                /* Check if the sequence is a number > 0. */
                _BSCANF_CHECK(fmt_ptr != end_ptr);
                _BSCANF_CHECK(max_width > 0);

                fmt_ptr = end_ptr;
            }

            /* Check for a length modifier. */
            if ('h' == *fmt_ptr || 'l' == *fmt_ptr) {
                length_mod = *fmt_ptr;
                fmt_ptr++;
            } else {
                length_mod = '\0';
            }

            /* Handle the conversion format specifier. */
            if ('n' == *fmt_ptr) {
                /* 'n': number of characters read so far. */
                /* 'n' conversion specifiers DO NOT consume whitespace. */
                /* Technically undefined, but just stop here for safety. */
                _BSCANF_CHECK(!is_suppressed);
                if ('l' == length_mod) {
                    long_ptr = va_arg(args, long*);
                    _BSCANF_CHECK_NULL(long_ptr);
                    *long_ptr = (long) (buf_ptr - buffer);
                } else if ('h' == length_mod) {
                    short_ptr = va_arg(args, short*);
                    _BSCANF_CHECK_NULL(short_ptr);
                    *short_ptr = (short) (buf_ptr - buffer);
                } else {
                    int_ptr = va_arg(args, int*);
                    _BSCANF_CHECK_NULL(int_ptr);
                    *int_ptr = (int) (buf_ptr - buffer);
                }
                fmt_ptr++;
                num_args_set++;
                continue;
            }

            /* All other specifiers move the buffer pointer, so check that it's not
                 NUL. */
            _BSCANF_CHECK_BUFFER();

            if ('%' == *fmt_ptr) {
                /* '%': match literal %. */
                _BSCANF_CONSUME_WSPACE();
                _BSCANF_MATCH();
                buf_ptr++;
            } else if ('c' == *fmt_ptr || 's' == *fmt_ptr) {
                /* 'c'/'s': match a character sequence/string. */
                /* String conversion requires a width. */
                _BSCANF_CHECK_STRING();

                /* 'c' conversion specifiers DO NOT consume whitespace. */
                if ('c' != *fmt_ptr) {
                    _BSCANF_CONSUME_WSPACE();
                }

                if (is_suppressed) {
                    /* Consume the character (string) and ignore it in this case. */
                    for (; max_width > 0; max_width--) {
                        buf_ptr++;
                        if (*buf_ptr == '\0' || (isspace(*buf_ptr) && 's' == *fmt_ptr)) {
                            break;
                        }
                    }
                    fmt_ptr++;
                    continue;

                } else if ('l' == length_mod) {
                    wchar_ptr = va_arg(args, wchar_t*);
                    _BSCANF_CHECK_NULL(char_ptr);
                    /* TODO: Implementation. */
                    _BSCANF_CHECK(0);

                } else {
                    char_ptr = va_arg(args, char*);
                    _BSCANF_CHECK_NULL(char_ptr);

                    for (; max_width > 0; max_width--) {
                        *char_ptr = *buf_ptr;
                        if (*buf_ptr == '\0' || (isspace(*buf_ptr) && 's' == *fmt_ptr)) {
                            break;
                        }
                        char_ptr++;
                        buf_ptr++;
                    }

                    /* Strings are null-terminated. */
                    if ('s' == *fmt_ptr) {
                        *char_ptr = '\0';
                    }
                    num_args_set++;
                }

            } else if ('[' == *fmt_ptr) {
                /* TODO: '[': match a non-empty sequence of characters from a set. */
                _BSCANF_CHECK(0);

                /* String conversion requires a width. */
                _BSCANF_CHECK_STRING();
                /* '[' conversion specifiers DO NOT consume whitespace. */

            } else if ('i' == *fmt_ptr || 'd' == *fmt_ptr) {
                /* 'i'/'d': match a integer/decimal integer. */

                _BSCANF_CONSUME_WSPACE();
                base = ('d' == *fmt_ptr) * 10;

                if (is_suppressed) {
                    /* Consume the integer and ignore it in this case. */
                    strtol(buf_ptr, &end_ptr, base);
                } else if ('l' == length_mod) {
                    long_ptr = va_arg(args, long*);
                    _BSCANF_CHECK_NULL(long_ptr);
                    *long_ptr = (long) strtol(buf_ptr, &end_ptr, base);
                } else if ('h' == length_mod) {
                    short_ptr = va_arg(args, short*);
                    _BSCANF_CHECK_NULL(short_ptr);
                    *short_ptr = (short) (strtol(buf_ptr, &end_ptr, base));
                } else {
                    int_ptr = va_arg(args, int*);
                    _BSCANF_CHECK_NULL(int_ptr);
                    *int_ptr = (int) (strtol(buf_ptr, &end_ptr, base));
                }

                _BSCANF_CHECK_STRTONUM();
                buf_ptr = end_ptr;
                num_args_set++;

            } else if (
                'g' == *fmt_ptr || 'e' == *fmt_ptr || 'f' == *fmt_ptr ||
                'G' == *fmt_ptr || 'E' == *fmt_ptr || 'F' == *fmt_ptr
            ) {
                /* 'g'/'e'/'f': match a float in strtod form. */
                /* TODO: 'a': match a float in C99 binary floating-point form. */

                _BSCANF_CONSUME_WSPACE();

                if (is_suppressed) {
                    /* Consume the float and ignore it in this case. */
                    strtod(buf_ptr, &end_ptr);
                } else if ('l' == length_mod) {
                    double_ptr = va_arg(args, double*);
                    _BSCANF_CHECK_NULL(double_ptr);
                    *double_ptr = (double) (strtod(buf_ptr, &end_ptr));
                } else {
                    float_ptr = va_arg(args, float*);
                    _BSCANF_CHECK_NULL(float_ptr);
                    *float_ptr = (float) (strtod(buf_ptr, &end_ptr));
                }

                _BSCANF_CHECK_STRTONUM();
                buf_ptr = end_ptr;
                num_args_set++;

            } else if (
                'u' == *fmt_ptr || 'o' == *fmt_ptr ||
                'x' == *fmt_ptr || 'X' == *fmt_ptr
            ) {
                /* 'u'/'o'/'x': match a unsigned decimal/octal/hexadecimal integer */

                _BSCANF_CONSUME_WSPACE();
                base =
                    ('u' == *fmt_ptr) * 10 +
                    ('o' == *fmt_ptr) * 8 +
                    ('x' == *fmt_ptr || 'X' == *fmt_ptr) * 16;

                if (is_suppressed) {
                    /* Consume the unsigned integer and ignore it in this case. */
                    strtoul(buf_ptr, &end_ptr, base);
                } else if ('l' == length_mod) {
                    ulong_ptr = va_arg(args, unsigned long*);
                    _BSCANF_CHECK_NULL(ulong_ptr);
                    *ulong_ptr = (unsigned long) strtoul(buf_ptr, &end_ptr, base);
                } else if ('h' == length_mod) {
                    ushort_ptr = va_arg(args, unsigned short*);
                    _BSCANF_CHECK_NULL(ushort_ptr);
                    *ushort_ptr = (unsigned short) (strtoul(buf_ptr, &end_ptr, base));
                } else {
                    uint_ptr = va_arg(args, unsigned int*);
                    _BSCANF_CHECK_NULL(uint_ptr);
                    *uint_ptr = (unsigned int) (strtoul(buf_ptr, &end_ptr, base));
                }

                _BSCANF_CHECK_STRTONUM();
                buf_ptr = end_ptr;
                num_args_set++;

            } else {
                /* Unknown conversion specifier. */
                _BSCANF_CHECK(0);
            }

            /* TODO: 'p': match a (implementation-defined) pointer. */

        } else {
            /* Match character with that in buffer. */
            _BSCANF_MATCH();
            buf_ptr++;
        }

        /* Get the next format specifier. */
        fmt_ptr++;
    }

 exit:
    return num_args_set;
}
#endif

int _sscanf_c(const char *__restrict buffer, const char *__restrict format, ...)
{
    va_list vlist;
    va_start(vlist, format);
    int ret = vsscanf(buffer, format, vlist);
    va_end(vlist);
    return ret;
}
