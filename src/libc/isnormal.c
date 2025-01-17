#include <stdint.h>

typedef union F64_pun {
    long double flt;
    uint64_t bin;
} F64_pun;

#define Float64_exp_mask UINT64_C(0x7FF0000000000000)

int _isnormall(long double x)
{
    F64_pun val;
    val.flt = x;
    val.bin &= Float64_exp_mask;
    
    // Check that the exponent isn't all zeros (subnormal) or all ones (nan/inf)
    return (val.bin != 0 && val.bin != Float64_exp_mask);
}
