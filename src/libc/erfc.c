#include <math.h>

float erfcf(float x)
{
    static const float p = 0.47047f, a1 = 0.3480242f, a2 = -0.0958798f, a3 = 0.7478556f;
    const float t = 1.0f / (1.0f + p * fabsf(x));
    float ret = t * (a1 + t * (a2 + t * a3)) * expf(-x * x);
	if (signbit(x)) {
		ret = 2.0f - ret;
	}
	return ret;
}

double erfc(double) __attribute__((alias("erfcf")));

long double erfcl(long double x)
{
    static const long double p = 0.3275911L, a1 = 0.254829592L, a2 = -0.284496736L, a3 = 1.421413741L, a4 = -1.453152027L, a5 = 1.061405429L;
    const long double t = 1.0L / (1.0L + p * fabsl(x));
    long double ret = t * (a1 + t * (a2 + t * (a3 + t * (a4 + t * a5)))) * expl(-x * x);
	if (signbit(x)) {
		ret = 2.0L - ret;
	}
	return ret;
}
