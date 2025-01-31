#include <math.h>

float cabsf(float _Complex x) {
	const float *x_pun = (const float*)((const void*)&x);
	return hypotf(x_pun[0], x_pun[1]);
}

double cabs(_Complex double) __attribute__((alias("cabsf")));

long double cabsl(long double _Complex x) {
	const long double *x_pun = (const long double*)((const void*)&x);
	return hypotl(x_pun[0], x_pun[1]);
}
