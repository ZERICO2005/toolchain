#ifndef _STDBIT_H
#define _STDBIT_H

#include <cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

// #define __STDC_VERSION_STDBIT_H__ 202311L
#define __STDC_ENDIAN_LITTLE__ __ORDER_LITTLE_ENDIAN__
#define __STDC_ENDIAN_BIG__ __ORDER_BIG_ENDIAN__
#define __STDC_ENDIAN_NATIVE__ __BYTE_ORDER__

unsigned int stdc_leading_zeros_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_leading_zeros_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_leading_zeros_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_leading_zeros_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_leading_zeros_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_leading_zeros_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_leading_ones_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_leading_ones_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_leading_ones_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_leading_ones_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_leading_ones_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_leading_ones_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_trailing_zeros_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_zeros_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_zeros_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_zeros_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_zeros_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_zeros_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_trailing_ones_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_ones_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_ones_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_ones_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_ones_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_trailing_ones_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_first_leading_zero_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_zero_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_zero_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_zero_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_zero_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_zero_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_first_leading_one_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_one_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_one_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_one_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_one_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_first_leading_one_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_first_trailing_zero_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_zero_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_zero_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_zero_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_zero_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_zero_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_first_trailing_one_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_one_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_one_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_one_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_one_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_first_trailing_one_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_count_zeros_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_count_zeros_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_count_zeros_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_count_zeros_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_count_zeros_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_count_zeros_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_count_ones_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_count_ones_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_count_ones_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_count_ones_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_count_ones_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_count_ones_ull(unsigned long long int value) __NOEXCEPT_CONST;

bool stdc_has_single_bit_uc(unsigned char value) __NOEXCEPT_CONST;
bool stdc_has_single_bit_us(unsigned short value) __NOEXCEPT_CONST;
bool stdc_has_single_bit_ui(unsigned int value) __NOEXCEPT_CONST;
bool stdc_has_single_bit_ul(unsigned long int value) __NOEXCEPT_CONST;
bool stdc_has_single_bit_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
bool stdc_has_single_bit_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned int stdc_bit_width_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned int stdc_bit_width_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_bit_width_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned int stdc_bit_width_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_bit_width_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned int stdc_bit_width_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned char stdc_bit_floor_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned short stdc_bit_floor_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_bit_floor_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned long int stdc_bit_floor_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_bit_floor_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned long long int stdc_bit_floor_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned char stdc_bit_ceil_uc(unsigned char value) __NOEXCEPT_CONST;
unsigned short stdc_bit_ceil_us(unsigned short value) __NOEXCEPT_CONST;
unsigned int stdc_bit_ceil_ui(unsigned int value) __NOEXCEPT_CONST;
unsigned long int stdc_bit_ceil_ul(unsigned long int value) __NOEXCEPT_CONST;
unsigned int stdc_bit_ceil_ui48(unsigned __int48 value) __NOEXCEPT_CONST;
unsigned long long int stdc_bit_ceil_ull(unsigned long long int value) __NOEXCEPT_CONST;

unsigned char stdc_rotate_left_uc(unsigned char value, unsigned int count) __NOEXCEPT_CONST;
unsigned short stdc_rotate_left_us(unsigned short value, unsigned int count) __NOEXCEPT_CONST;
unsigned int stdc_rotate_left_ui(unsigned int value, unsigned int count) __NOEXCEPT_CONST;
unsigned long stdc_rotate_left_ul(unsigned long value, unsigned int count) __NOEXCEPT_CONST;
unsigned int stdc_rotate_left_ui48(unsigned __int48 value, unsigned int count) __NOEXCEPT_CONST;
unsigned long long stdc_rotate_left_ull(unsigned long long value, unsigned int count) __NOEXCEPT_CONST;

unsigned char stdc_rotate_right_uc(unsigned char value, unsigned int count) __NOEXCEPT_CONST;
unsigned short stdc_rotate_right_us(unsigned short value, unsigned int count) __NOEXCEPT_CONST;
unsigned int stdc_rotate_right_ui(unsigned int value, unsigned int count) __NOEXCEPT_CONST;
unsigned long stdc_rotate_right_ul(unsigned long value, unsigned int count) __NOEXCEPT_CONST;
unsigned int stdc_rotate_right_ui48(unsigned __int48 value, unsigned int count) __NOEXCEPT_CONST;
unsigned long long stdc_rotate_right_ull(unsigned long long value, unsigned int count) __NOEXCEPT_CONST;

// #define stdc_leading_zeros_uc(x) __builtin_(x)
#define stdc_leading_zeros_us(x) __builtin_clzs(x)
#define stdc_leading_zeros_ui(x) __builtin_clz(x)
#define stdc_leading_zeros_ul(x) __builtin_clzl(x)
// #define stdc_leading_zeros_ui48(x) __builtin_(x)
#define stdc_leading_zeros_ull(x) __builtin_clzll(x)

// #define stdc_leading_ones_uc(x) __builtin_(x)
// #define stdc_leading_ones_us(x) __builtin_(x)
// #define stdc_leading_ones_ui(x) __builtin_(x)
// #define stdc_leading_ones_ul(x) __builtin_(x)
// #define stdc_leading_ones_ui48(x) __builtin_(x)
// #define stdc_leading_ones_ull(x) __builtin_(x)

// #define stdc_trailing_zeros_uc(x) __builtin_(x)
#define stdc_trailing_zeros_us(x) __builtin_ctzs(x)
#define stdc_trailing_zeros_ui(x) __builtin_ctz(x)
#define stdc_trailing_zeros_ul(x) __builtin_ctzl(x)
// #define stdc_trailing_zeros_ui48(x) __builtin_(x)
#define stdc_trailing_zeros_ull(x) __builtin_ctzll(x)

// #define stdc_trailing_ones_uc(x) __builtin_(x)
// #define stdc_trailing_ones_us(x) __builtin_(x)
// #define stdc_trailing_ones_ui(x) __builtin_(x)
// #define stdc_trailing_ones_ul(x) __builtin_(x)
// #define stdc_trailing_ones_ui48(x) __builtin_(x)
// #define stdc_trailing_ones_ull(x) __builtin_(x)

// #define stdc_first_leading_zero_uc(x) __builtin_(x)
// #define stdc_first_leading_zero_us(x) __builtin_(x)
// #define stdc_first_leading_zero_ui(x) __builtin_(x)
// #define stdc_first_leading_zero_ul(x) __builtin_(x)
// #define stdc_first_leading_zero_ui48(x) __builtin_(x)
// #define stdc_first_leading_zero_ull(x) __builtin_(x)

// #define stdc_first_leading_one_uc(x) __builtin_(x)
// #define stdc_first_leading_one_us(x) __builtin_(x)
// #define stdc_first_leading_one_ui(x) __builtin_(x)
// #define stdc_first_leading_one_ul(x) __builtin_(x)
// #define stdc_first_leading_one_ui48(x) __builtin_(x)
// #define stdc_first_leading_one_ull(x) __builtin_(x)

// #define stdc_first_trailing_zero_uc(x) __builtin_(x)
// #define stdc_first_trailing_zero_us(x) __builtin_(x)
// #define stdc_first_trailing_zero_ui(x) __builtin_(x)
// #define stdc_first_trailing_zero_ul(x) __builtin_(x)
// #define stdc_first_trailing_zero_ui48(x) __builtin_(x)
// #define stdc_first_trailing_zero_ull(x) __builtin_(x)

// #define stdc_first_trailing_one_uc(x) __builtin_(x)
// #define stdc_first_trailing_one_us(x) __builtin_(x)
// #define stdc_first_trailing_one_ui(x) __builtin_(x)
// #define stdc_first_trailing_one_ul(x) __builtin_(x)
// #define stdc_first_trailing_one_ui48(x) __builtin_(x)
// #define stdc_first_trailing_one_ull(x) __builtin_(x)

// #define stdc_count_zeros_uc(x) __builtin_(x)
// #define stdc_count_zeros_us(x) __builtin_(x)
// #define stdc_count_zeros_ui(x) __builtin_(x)
// #define stdc_count_zeros_ul(x) __builtin_(x)
// #define stdc_count_zeros_ui48(x) __builtin_(x)
// #define stdc_count_zeros_ull(x) __builtin_(x)

// #define stdc_count_ones_uc(x) __builtin_(x)
// #define stdc_count_ones_us(x) __builtin_(x)
#define stdc_count_ones_ui(x) __builtin_popcount(x)
#define stdc_count_ones_ul(x) __builtin_popcountl(x)
// #define stdc_count_ones_ui48(x) __builtin_(x)
#define stdc_count_ones_ull(x) __builtin_popcountll(x)

// #define stdc_has_single_bit_uc(x) __builtin_(x)
// #define stdc_has_single_bit_us(x) __builtin_(x)
// #define stdc_has_single_bit_ui(x) __builtin_(x)
// #define stdc_has_single_bit_ul(x) __builtin_(x)
// #define stdc_has_single_bit_ui48(x) __builtin_(x)
// #define stdc_has_single_bit_ull(x) __builtin_(x)

// #define stdc_bit_width_uc(x) __builtin_(x)
// #define stdc_bit_width_us(x) __builtin_(x)
// #define stdc_bit_width_ui(x) __builtin_(x)
// #define stdc_bit_width_ul(x) __builtin_(x)
// #define stdc_bit_width_ui48(x) __builtin_(x)
// #define stdc_bit_width_ull(x) __builtin_(x)

// #define stdc_bit_floor_uc(x) __builtin_(x)
// #define stdc_bit_floor_us(x) __builtin_(x)
// #define stdc_bit_floor_ui(x) __builtin_(x)
// #define stdc_bit_floor_ul(x) __builtin_(x)
// #define stdc_bit_floor_ui48(x) __builtin_(x)
// #define stdc_bit_floor_ull(x) __builtin_(x)

// #define stdc_bit_ceil_uc(x) __builtin_(x)
// #define stdc_bit_ceil_us(x) __builtin_(x)
// #define stdc_bit_ceil_ui(x) __builtin_(x)
// #define stdc_bit_ceil_ul(x) __builtin_(x)
// #define stdc_bit_ceil_ui48(x) __builtin_(x)
// #define stdc_bit_ceil_ull(x) __builtin_(x)

// #define stdc_rotate_left_uc(x) __builtin_(x)
// #define stdc_rotate_left_us(x) __builtin_(x)
// #define stdc_rotate_left_ui(x) __builtin_(x)
// #define stdc_rotate_left_ul(x) __builtin_(x)
// #define stdc_rotate_left_ui48(x) __builtin_(x)
// #define stdc_rotate_left_ull(x) __builtin_(x)

// #define stdc_rotate_right_uc(x) __builtin_(x)
// #define stdc_rotate_right_us(x) __builtin_(x)
// #define stdc_rotate_right_ui(x) __builtin_(x)
// #define stdc_rotate_right_ul(x) __builtin_(x)
// #define stdc_rotate_right_ui48(x) __builtin_(x)
// #define stdc_rotate_right_ull(x) __builtin_(x)

#define stdc_leading_ones(x) _Generic((x), \
    unsigned char:      stdc_leading_ones_uc, \
    unsigned short:     stdc_leading_ones_us, \
    unsigned int:       stdc_leading_ones_ui, \
    unsigned long:      stdc_leading_ones_ul, \
    unsigned __int48:   stdc_leading_ones_ui48, \
    unsigned long long: stdc_leading_ones_ull \
)(x)

#define stdc_trailing_zeros(x) _Generic((x), \
    unsigned char:      stdc_trailing_zeros_uc, \
    unsigned short:     stdc_trailing_zeros_us, \
    unsigned int:       stdc_trailing_zeros_ui, \
    unsigned long:      stdc_trailing_zeros_ul, \
    unsigned __int48:   stdc_trailing_zeros_ui48, \
    unsigned long long: stdc_trailing_zeros_ull \
)(x)

#define stdc_trailing_ones(x) _Generic((x), \
    unsigned char:      stdc_trailing_ones_uc, \
    unsigned short:     stdc_trailing_ones_us, \
    unsigned int:       stdc_trailing_ones_ui, \
    unsigned long:      stdc_trailing_ones_ul, \
    unsigned __int48:   stdc_trailing_ones_ui48, \
    unsigned long long: stdc_trailing_ones_ull \
)(x)

#define stdc_first_leading_zero(x) _Generic((x), \
    unsigned char:      stdc_first_leading_zero_uc, \
    unsigned short:     stdc_first_leading_zero_us, \
    unsigned int:       stdc_first_leading_zero_ui, \
    unsigned long:      stdc_first_leading_zero_ul, \
    unsigned __int48:   stdc_first_leading_zero_ui48, \
    unsigned long long: stdc_first_leading_zero_ull \
)(x)

#define stdc_first_leading_one(x) _Generic((x), \
    unsigned char:      stdc_first_leading_one_uc, \
    unsigned short:     stdc_first_leading_one_us, \
    unsigned int:       stdc_first_leading_one_ui, \
    unsigned long:      stdc_first_leading_one_ul, \
    unsigned __int48:   stdc_first_leading_one_ui48, \
    unsigned long long: stdc_first_leading_one_ull \
)(x)

#define stdc_first_trailing_zero(x) _Generic((x), \
    unsigned char:      stdc_first_trailing_zero_uc, \
    unsigned short:     stdc_first_trailing_zero_us, \
    unsigned int:       stdc_first_trailing_zero_ui, \
    unsigned long:      stdc_first_trailing_zero_ul, \
    unsigned __int48:   stdc_first_trailing_zero_ui48, \
    unsigned long long: stdc_first_trailing_zero_ull \
)(x)

#define stdc_first_trailing_one(x) _Generic((x), \
    unsigned char:      stdc_first_trailing_one_uc, \
    unsigned short:     stdc_first_trailing_one_us, \
    unsigned int:       stdc_first_trailing_one_ui, \
    unsigned long:      stdc_first_trailing_one_ul, \
    unsigned __int48:   stdc_first_trailing_one_ui48, \
    unsigned long long: stdc_first_trailing_one_ull \
)(x)

#define stdc_count_zeros(x) _Generic((x), \
    unsigned char:      stdc_count_zeros_uc, \
    unsigned short:     stdc_count_zeros_us, \
    unsigned int:       stdc_count_zeros_ui, \
    unsigned long:      stdc_count_zeros_ul, \
    unsigned __int48:   stdc_count_zeros_ui48, \
    unsigned long long: stdc_count_zeros_ull \
)(x)

#define stdc_count_ones(x) _Generic((x), \
    unsigned char:      stdc_count_ones_uc, \
    unsigned short:     stdc_count_ones_us, \
    unsigned int:       stdc_count_ones_ui, \
    unsigned long:      stdc_count_ones_ul, \
    unsigned __int48:   stdc_count_ones_ui48, \
    unsigned long long: stdc_count_ones_ull \
)(x)

#define stdc_has_single_bit(x) _Generic((x), \
    unsigned char:      stdc_has_single_bit_uc, \
    unsigned short:     stdc_has_single_bit_us, \
    unsigned int:       stdc_has_single_bit_ui, \
    unsigned long:      stdc_has_single_bit_ul, \
    unsigned __int48:   stdc_has_single_bit_ui48, \
    unsigned long long: stdc_has_single_bit_ull \
)(x)

#define stdc_bit_width(x) _Generic((x), \
    unsigned char:      stdc_bit_width_uc, \
    unsigned short:     stdc_bit_width_us, \
    unsigned int:       stdc_bit_width_ui, \
    unsigned long:      stdc_bit_width_ul, \
    unsigned __int48:   stdc_bit_width_ui48, \
    unsigned long long: stdc_bit_width_ull \
)(x)

#define stdc_bit_floor(x) _Generic((x), \
    unsigned char:      stdc_bit_floor_uc, \
    unsigned short:     stdc_bit_floor_us, \
    unsigned int:       stdc_bit_floor_ui, \
    unsigned long:      stdc_bit_floor_ul, \
    unsigned __int48:   stdc_bit_floor_ui48, \
    unsigned long long: stdc_bit_floor_ull \
)(x)

#define stdc_bit_ceil(x) _Generic((x), \
    unsigned char:      stdc_bit_ceil_uc, \
    unsigned short:     stdc_bit_ceil_us, \
    unsigned int:       stdc_bit_ceil_ui, \
    unsigned long:      stdc_bit_ceil_ul, \
    unsigned __int48:   stdc_bit_ceil_ui48, \
    unsigned long long: stdc_bit_ceil_ull \
)(x)

#define stdc_rotate_left(x) _Generic((x), \
    unsigned char:      stdc_rotate_left_uc, \
    unsigned short:     stdc_rotate_left_us, \
    unsigned int:       stdc_rotate_left_ui, \
    unsigned long:      stdc_rotate_left_ul, \
    unsigned __int48:   stdc_rotate_left_ui48, \
    unsigned long long: stdc_rotate_left_ull \
)(x)

#define stdc_rotate_right(x) _Generic((x), \
    unsigned char:      stdc_rotate_right_uc, \
    unsigned short:     stdc_rotate_right_us, \
    unsigned int:       stdc_rotate_right_ui, \
    unsigned long:      stdc_rotate_right_ul, \
    unsigned __int48:   stdc_rotate_right_ui48, \
    unsigned long long: stdc_rotate_right_ull \
)(x)

#ifdef __cplusplus
}
#endif

#endif /* _STDBIT_H */
