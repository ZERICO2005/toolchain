#if 0
typedef _Bool bool;

#define UINT64_C(x) x
typedef unsigned char uint8_t;

typedef unsigned int uint24_t;
typedef unsigned __int48 uint48_t;
typedef signed int int24_t;
typedef signed __int48 int48_t;
typedef unsigned long long uint64_t;
typedef unsigned long uint32_t;
typedef unsigned int size_t;

long double frexpl(long double, int*);
void* memcpy(void*, const void*, size_t);

#define signbit _signbitl
bool _signbitl(long double) __attribute__((__const__));
#define isfinite _isfinitel
bool _isfinitel(long double) __attribute__((__const__));
#define isnan _isnanl
bool _isnanl(long double) __attribute__((__const__));
#else
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#endif

#if 0

typedef struct f64 {
    int24_t expon;
    uint64_t mant;
} f64;

#define Float64_mant_mask UINT64_C(0x000FFFFFFFFFFFFF)
#define Float64_infinity  UINT64_C(0x7FF0000000000000)
#define Float64_expon_lsb UINT64_C(0x0010000000000000)

#define Float64_inf_expon 2047
#define Float64_bias 1023
#define Float64_max_subnorm_exp -1023

#define Float64_mant_bits 52
#define Float64_total_bits 53

#define Float64_expon_modulo 0x7FF

#if 1
void split_f64(long double x, f64* ret) {
	int expon;
    x = frexpl(x, &expon);
	ret->expon = (int24_t)(expon - 1);
    uint64_t bin;
    memcpy(&bin, &x, sizeof(bin));
    ret->mant = (bin & Float64_mant_mask);
}
#else
void split_f64(long double x, f64* ret);
#endif

typedef struct uint112 {
	uint64_t lo;
	uint48_t hi;
} uint112;

// assembly
void _i112_shl(uint112* arg, uint8_t shift);
// assembly
static void _i112_shl1(uint112* arg) {
	_i112_shl(arg, 1);
}

// assembly
void _i112_shru(uint112* arg, uint8_t shift);

// assembly
void _i112_sub1(uint112* arg) {
	if (arg->lo == 0) {
		arg->hi--;
	}
	arg->lo--;
}

static void i112_mul64x64(uint112* ret, uint64_t op1, uint64_t op2) {
    uint64_t u1 = (op1 & 0xffffffff);
    uint64_t v1 = (op2 & 0xffffffff);
    uint64_t t = (u1 * v1);
    uint64_t w3 = (t & 0xffffffff);
    uint64_t k = (t >> 32);

    op1 >>= 32;
    t = (op1 * v1) + k;
    k = (t & 0xffffffff);
    uint64_t w1 = (t >> 32);

    op2 >>= 32;
    t = (u1 * op2) + k;
    k = (t >> 32);

    ret->hi = ((uint48_t)op1 * (uint48_t)op2) + (uint48_t)w1 + (uint48_t)k;
    ret->lo = (t << 32) + w3;
}

long double _dmul_c(const bool result_sign, long double x_flt, long double* y_flt_ptr) {
	long double y_flt = *y_flt_ptr;
    f64 x, y;
    split_f64(x_flt, &x);
    split_f64(y_flt, &y);
	// const bool result_sign = (x.sign != y.sign);
    if (!isfinite(x_flt) || !isfinite(y_flt)) {
		if (isnan(x_flt)) {
			if (isnan(y_flt)) {
				uint64_t bin = Float64_infinity;
				bin |= x.mant;
				bin |= y.mant;
				long double flt;
				memcpy(&flt, &bin, sizeof(flt));
				return result_sign ? -flt : flt;
			}
			return x_flt;
		}
		if (isnan(y_flt)) {
			return y_flt;
		}
		return __builtin_nanl("");
    }
	if (x_flt == 0.0L && y_flt == 0.0L) {
		return result_sign ? -0.0L : 0.0L;
	}

	x.mant |= Float64_expon_lsb;
	y.mant |= Float64_expon_lsb;
	uint112 prod_mant;
	i112_mul64x64(&prod_mant, x.mant, y.mant);

	int prod_expon = x.expon + y.expon + 1;
	// ensure the the MSB is set after multiplication
	if ((prod_mant.hi & ((uint48_t)1 << (105 - 64))) == 0) {
		prod_expon--;
		_i112_shl1(&prod_mant);
	}
	
	prod_expon += Float64_bias;
	#if 0
		bool sticky, round, guard;
		if (prod_expon <= 0) {
			unsigned int shift = (unsigned int)-(prod_expon);
			if (shift >= Float64_total_bits) {
				return result_sign ? -0.0 : 0.0;
			}
			shift += 1;
			uint112 mask = {.lo = Float64_expon_lsb, .hi = 0};
			uint112 sticky_mask, round_mask, guard_mask;
			i112_shl(&mask, (uint8_t)shift);
			sticky_mask = mask;
			round_mask = mask;
			guard_mask = mask;
			i112_sub1(&sticky_mask);
			i112_shl1(&guard_mask);

			sticky = (prod_mant.lo & sticky_mask.lo) || (prod_mant.hi & sticky_mask.hi);
			round  = (prod_mant.lo & round_mask.lo ) || (prod_mant.hi & round_mask.hi );
			guard  = (prod_mant.lo & guard_mask.lo ) || (prod_mant.hi & guard_mask.hi );

			i112_shru(&prod_mant, (uint8_t)shift);
		} else {
			uint64_t sticky_mask = (uint64_t)Float64_expon_lsb - 1;
			uint64_t round_mask  = (uint64_t)Float64_expon_lsb;
			uint64_t guard_mask  = (uint64_t)Float64_expon_lsb * 2;
			sticky = (prod_mant.lo & sticky_mask) != 0;
			round  = (prod_mant.lo & round_mask ) != 0;
			guard  = (prod_mant.lo & guard_mask ) != 0;
		}
		i112_shru(&prod_mant, Float64_total_bits);
	#else
		unsigned int shift = 0;
		if (prod_expon <= 0) {
			shift = (unsigned int)-(prod_expon);
			if (shift >= Float64_total_bits) {
				return result_sign ? -0.0L : 0.0L;
			}
			shift += 1;
		}
		uint112 mask = {.lo = Float64_expon_lsb, .hi = 0};
		uint112 sticky_mask, round_mask, guard_mask;
		_i112_shl(&mask, (uint8_t)shift);
		sticky_mask = mask;
		round_mask = mask;
		guard_mask = mask;
		_i112_sub1(&sticky_mask);
		_i112_shl1(&guard_mask);

		bool sticky = (prod_mant.lo & sticky_mask.lo) || (prod_mant.hi & sticky_mask.hi);
		bool round  = (prod_mant.lo & round_mask.lo ) || (prod_mant.hi & round_mask.hi );
		bool guard  = (prod_mant.lo & guard_mask.lo ) || (prod_mant.hi & guard_mask.hi );
		_i112_shru(&prod_mant, (uint8_t)(shift + Float64_total_bits));
	#endif
	uint64_t round_mant = prod_mant.lo;
	if (round) {
		if (guard || sticky) {
			// round up to even
			round_mant++;
			if (round_mant == 0) {
				prod_expon++;
			}
		}
	}

	if (prod_expon >= Float64_inf_expon) {
		return result_sign ? -__builtin_huge_vall() : __builtin_huge_vall();
	}
	uint64_t bin = round_mant;
	if (prod_expon > 0) {
		bin &= Float64_mant_mask;
		bin |= (uint64_t)(prod_expon & Float64_expon_modulo) << Float64_mant_bits;
	}
	long double flt;
	memcpy(&flt, &bin, sizeof(flt));
	return result_sign ? -flt : flt;
}

#else

typedef struct f64 {
    int24_t expon;
    uint64_t mant;
} f64;

#define Float64_mant_mask UINT64_C(0x000FFFFFFFFFFFFF)
#define Float64_infinity  UINT64_C(0x7FF0000000000000)
#define Float64_expon_lsb UINT64_C(0x0010000000000000)

#define Float64_inf_expon 2047
#define Float64_bias 1023
#define Float64_max_subnorm_exp -1023

#define Float64_mant_bits 52
#define Float64_total_bits 53

#define Float64_expon_modulo 0x7FF

#if 1
static void split_f64(long double x, f64* ret) {
	int expon;
    x = frexpl(x, &expon);
	ret->expon = (int24_t)(expon - 1);
    uint64_t bin;
    memcpy(&bin, &x, sizeof(bin));
    ret->mant = (bin & Float64_mant_mask);
}
#else
void split_f64(long double x, f64* ret);
#endif

typedef struct uint112 {
	uint64_t lo;
	uint48_t hi;
} uint112;

// assembly
void _i112_shl(uint112* arg, uint8_t shift);
// assembly
static void _i112_shl1(uint112* arg) {
	_i112_shl(arg, 1);
}

static void i112_mul64x64(uint112* ret, uint64_t op1, uint64_t op2) {
    uint64_t u1 = (op1 & 0xffffffff);
    uint64_t v1 = (op2 & 0xffffffff);
    uint64_t t = (u1 * v1);
    uint64_t w3 = (t & 0xffffffff);
    uint64_t k = (t >> 32);

    op1 >>= 32;
    t = (op1 * v1) + k;
    k = (t & 0xffffffff);
    uint64_t w1 = (t >> 32);

    op2 >>= 32;
    t = (u1 * op2) + k;
    k = (t >> 32);

    ret->hi = ((uint48_t)op1 * (uint48_t)op2) + (uint48_t)w1 + (uint48_t)k;
    ret->lo = (t << 32) + w3;
}

long double _i112_shru_to_even(uint112* arg, uint8_t shift_m48, bool sign, int expon);

long double _dmul_c(const bool result_sign, long double x_flt, long double* y_flt_ptr) {
	long double y_flt = *y_flt_ptr;
    f64 x, y;
    split_f64(x_flt, &x);
    split_f64(y_flt, &y);
    if (!isfinite(x_flt) || !isfinite(y_flt)) {
		if (isnan(x_flt)) {
			if (isnan(y_flt)) {
				uint64_t bin = Float64_infinity;
				bin |= x.mant;
				bin |= y.mant;
				long double flt;
				memcpy(&flt, &bin, sizeof(flt));
				return result_sign ? -flt : flt;
			}
			return x_flt;
		}
		if (isnan(y_flt)) {
			return y_flt;
		}
		return __builtin_nanl("");
    }
	if (x_flt == 0.0L && y_flt == 0.0L) {
		return result_sign ? -0.0L : 0.0L;
	}

	x.mant |= Float64_expon_lsb;
	y.mant |= Float64_expon_lsb;
	uint112 prod_mant;
	i112_mul64x64(&prod_mant, x.mant, y.mant);

	int prod_expon = x.expon + y.expon + 1;
	unsigned int shift = 0;
	// ensure the the MSB is set after multiplication
	if ((prod_mant.hi & ((uint48_t)1 << (105 - 64))) == 0) {
		prod_expon--;
		shift++;
		// _i112_shl1(&prod_mant);
	}
	prod_expon += Float64_bias;
	if (prod_expon <= 0) {
		shift -= (prod_expon);
		if (shift >= Float64_total_bits) {
			return result_sign ? -0.0L : 0.0L;
		}
		shift += 1;
	}
	return _i112_shru_to_even(
		&prod_mant, (uint8_t)(shift + Float64_total_bits - 48),
		result_sign, prod_expon
	);
}

#endif
