#include <math.h>

float _Complex cprojf(float _Complex x) {
	float* x_pun = (float*)((void*)&x);
	if (isinf(x_pun[0]) || isinf(x_pun[1])) {
		x_pun[0] = HUGE_VALF;
		x_pun[1] = copysignf(0.0f, x_pun[1]);
	}
	return *x_pun;
}

_Complex double cproj(_Complex double) __attribute__((alias("cprojf")));

long double _Complex cprojl(long double _Complex x) {
	long double* x_pun = (long double*)((void*)&x);
	if (isinf(x_pun[0]) || isinf(x_pun[1])) {
		x_pun[0] = HUGE_VALF;
		x_pun[1] = copysignl(0.0L, x_pun[1]);
	}
	return *x_pun;
}
