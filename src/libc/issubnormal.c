#include <stdbool.h>
#include <stdint.h>
#include <math.h>

typedef union F64_pun {
    long double flt;
    uint64_t bin;
} F64_pun;

#define Float64_exp_mask UINT64_C(0x7FF0000000000000)

int _issubnormall(long double x) {
    // zero is not a subnormal number
    if (iszero(x)) {
        return false;
    }
    F64_pun val;
    val.flt = x;
    val.bin &= Float64_exp_mask;
    // check if the exponent is all zeros
    return (val.bin == 0);
}
