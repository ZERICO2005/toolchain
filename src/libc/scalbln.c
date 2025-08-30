#include <math.h>
#include <limits.h>

float old_scalblnf(float x, long expon)
{
    // saturated int32_t to int24_t conversion
    int saturated_expon = (int)expon;
    if (expon > INT_MAX) {
        saturated_expon = INT_MAX;
    } else if (expon < INT_MIN) {
        saturated_expon = INT_MIN;
    }
    return scalbnf(x, saturated_expon);
}

double old_scalbln(double, long) __attribute__((alias("old_scalblnf")));

long double old_scalblnl(long double x, long expon)
{
    // saturated int32_t to int24_t conversion
    int saturated_expon = (int)expon;
    if (expon > INT_MAX) {
        saturated_expon = INT_MAX;
    } else if (expon < INT_MIN) {
        saturated_expon = INT_MIN;
    }
    return scalbnl(x, saturated_expon);
}
