#include <math.h>

float cargf(float _Complex x) {
	const float *x_pun = (const float*)((const void*)&x);
	return atan2f(x_pun[1], x_pun[0]);
}

double carg(_Complex double) __attribute__((alias("cargf")));

long double cargl(long double _Complex x) {
	const long double *x_pun = (const long double*)((const void*)&x);
	return atan2l(x_pun[1], x_pun[0]);
}
