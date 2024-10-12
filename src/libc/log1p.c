/*
  Copyright (c) 2015-2023, Norbert Juffa
  All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright 
     notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
  Source:
  https://forums.developer.nvidia.com/t/faster-and-more-accurate-implementation-of-log1pf/40575
*/

#include <math.h>
#include <errno.h>

// This can reduce the function size
#define USE_LOOKUP_TABLE 0

#define lut_0 +3.28586996e-2f // +0x1.0d2db0p-5

#define lut_1 -9.10350382e-2f // -0x1.74e128p-4
#define lut_2 +1.22964852e-1f // +0x1.f7a9fep-4
#define lut_3 -1.30156621e-1f // -0x1.0a8f8ep-3
#define lut_4 +1.42396331e-1f // +0x1.23a0b0p-3
#define lut_5 -1.66184038e-1f // -0x1.54584cp-3
#define lut_6 +1.99986562e-1f // +0x1.99928ep-3
#define lut_7 -2.50015855e-1f // -0x1.000428p-2
#define lut_8 +3.33333999e-1f // +0x1.555582p-2
#define lut_9 -4.99999851e-1f // -0x1.fffff6p-2

#if USE_LOOKUP_TABLE != 0
static const float log1pf_lut[] {
	lut_1, lut_2, lut_3, lut_4, lut_5, lut_6, lut_7, lut_8, lut_9
};
#endif

#define naive_fmaf(x, y, z) x * y + z

/**
 * @brief Calculates `log(x + 1.0)` without losing precision when `x` is close
 * to zero.
 * @remarks Minimum precision: 22.678617 bits when `-0.304 < x < +0.609`,
 * otherwise 21.053935 bits when using `logf(x + 1.0f)` as a fallback (based on
 * the implmentation of `logf` from 2024, October 11th).
 * Tested across all 2^32 input values.
 * @note Precision tested with 32bit floats on x86_64
 */
float log1pf(const float x) {
	// Unordered not_greater_than catches NaN
	if (!(x > -1.0f)) {
		errno = EDOM;
		return -HUGE_VALF;
	}
	if (x == 0.0f) {
		return x;
	}
	if ((x < -0.3046875f) || (x > 0.609375f)) {
		return logf(x + 1.0f);
	}
	
	float r = lut_0;
	
	#if USE_LOOKUP_TABLE != 0
		const float* lut_ptr = log1pf_lut;
		for (uint8_t i = 0; i < (uint8_t)(sizeof(log1pf_lut) / sizeof(float)); i++) {
			r = naive_fmaf(r, x, *lut_ptr);
			lut_ptr++;
		}
	#else
		r = naive_fmaf(r, x, lut_1);
		r = naive_fmaf(r, x, lut_2);
		r = naive_fmaf(r, x, lut_3);
		r = naive_fmaf(r, x, lut_4);
		r = naive_fmaf(r, x, lut_5);
		r = naive_fmaf(r, x, lut_6);
		r = naive_fmaf(r, x, lut_7);
		r = naive_fmaf(r, x, lut_8);
		r = naive_fmaf(r, x, lut_9);
	#endif

	r = r * x;
	r = naive_fmaf(r, x, x);
	return r;
}

double log1p(double) __attribute__((alias("log1pf")));
