#include <math.h>

void _mulsc3_c(float const * const src, float * const * const dst) {
	if (isnan(src[0]) || isnan(src[1])) {
		(*dst)[0] = copysignf(HUGE_VALF, src[0]);
		(*dst)[1] = copysignf(0.0f, src[1]);
	}
	
}
