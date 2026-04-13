//------------------------------------------------------------------------------
// source
//------------------------------------------------------------------------------

#include "extF80_add.c"
#include "extF80_div.c"
#include "extF80_eq.c"
#include "extF80_eq_signaling.c"
#include "extF80_isSignalingNaN.c"
#include "extF80_le.c"
#include "extF80_le_quiet.c"
#include "extF80_lt.c"
#include "extF80_lt_quiet.c"
#include "extF80_mul.c"
#include "extF80_rem.c"
#include "extF80_roundToInt.c"
#include "extF80_sqrt.c"
#include "extF80_sub.c"
#include "extF80_to_f128.c"
#include "extF80_to_f16.c"
#include "extF80_to_f32.c"
#include "extF80_to_f64.c"
#include "extF80_to_i32.c"
#include "extF80_to_i32_r_minMag.c"
#include "extF80_to_i64.c"
#include "extF80_to_i64_r_minMag.c"
#include "extF80_to_ui32.c"
#include "extF80_to_ui32_r_minMag.c"
#include "extF80_to_ui64.c"
#include "extF80_to_ui64_r_minMag.c"
#include "extF80M_add.c"
#include "extF80M_div.c"
#include "extF80M_eq.c"
#include "extF80M_eq_signaling.c"
#include "extF80M_isSignalingNaN.c"
#include "extF80M_le.c"
#include "extF80M_le_quiet.c"
#include "extF80M_lt.c"
#include "extF80M_lt_quiet.c"
#include "extF80M_mul.c"
#include "extF80M_rem.c"
#include "extF80M_roundToInt.c"
#include "extF80M_sqrt.c"
#include "extF80M_sub.c"
#include "extF80M_to_f128M.c"
#include "extF80M_to_f16.c"
#include "extF80M_to_f32.c"
#include "extF80M_to_f64.c"
#include "extF80M_to_i32.c"
#include "extF80M_to_i32_r_minMag.c"
#include "extF80M_to_i64.c"
#include "extF80M_to_i64_r_minMag.c"
#include "extF80M_to_ui32.c"
#include "extF80M_to_ui32_r_minMag.c"
#include "extF80M_to_ui64.c"
#include "extF80M_to_ui64_r_minMag.c"
#include "f128_add.c"
#include "f128_div.c"
#include "f128_eq.c"
#include "f128_eq_signaling.c"
#include "f128_isSignalingNaN.c"
#include "f128_le.c"
#include "f128_le_quiet.c"
#include "f128_lt.c"
#include "f128_lt_quiet.c"
#include "f128_mul.c"
#include "f128_mulAdd.c"
#include "f128_rem.c"
#include "f128_roundToInt.c"
#include "f128_sqrt.c"
#include "f128_sub.c"
#include "f128_to_extF80.c"
#include "f128_to_f16.c"
#include "f128_to_f32.c"
#include "f128_to_f64.c"
#include "f128_to_i32.c"
#include "f128_to_i32_r_minMag.c"
#include "f128_to_i64.c"
#include "f128_to_i64_r_minMag.c"
#include "f128_to_ui32.c"
#include "f128_to_ui32_r_minMag.c"
#include "f128_to_ui64.c"
#include "f128_to_ui64_r_minMag.c"
#include "f128M_add.c"
#include "f128M_div.c"
#include "f128M_eq.c"
#include "f128M_eq_signaling.c"
#include "f128M_isSignalingNaN.c"
#include "f128M_le.c"
#include "f128M_le_quiet.c"
#include "f128M_lt.c"
#include "f128M_lt_quiet.c"
#include "f128M_mul.c"
#include "f128M_mulAdd.c"
#include "f128M_rem.c"
#include "f128M_roundToInt.c"
#include "f128M_sqrt.c"
#include "f128M_sub.c"
#include "f128M_to_extF80M.c"
#include "f128M_to_f16.c"
#include "f128M_to_f32.c"
#include "f128M_to_f64.c"
#include "f128M_to_i32.c"
#include "f128M_to_i32_r_minMag.c"
#include "f128M_to_i64.c"
#include "f128M_to_i64_r_minMag.c"
#include "f128M_to_ui32.c"
#include "f128M_to_ui32_r_minMag.c"
#include "f128M_to_ui64.c"
#include "f128M_to_ui64_r_minMag.c"
#include "f16_add.c"
#include "f16_div.c"
#include "f16_eq.c"
#include "f16_eq_signaling.c"
#include "f16_isSignalingNaN.c"
#include "f16_le.c"
#include "f16_le_quiet.c"
#include "f16_lt.c"
#include "f16_lt_quiet.c"
#include "f16_mul.c"
#include "f16_mulAdd.c"
#include "f16_rem.c"
#include "f16_roundToInt.c"
#include "f16_sqrt.c"
#include "f16_sub.c"
#include "f16_to_extF80.c"
#include "f16_to_extF80M.c"
#include "f16_to_f128.c"
#include "f16_to_f128M.c"
#include "f16_to_f32.c"
#include "f16_to_f64.c"
#include "f16_to_i32.c"
#include "f16_to_i32_r_minMag.c"
#include "f16_to_i64.c"
#include "f16_to_i64_r_minMag.c"
#include "f16_to_ui32.c"
#include "f16_to_ui32_r_minMag.c"
#include "f16_to_ui64.c"
#include "f16_to_ui64_r_minMag.c"
#include "f32_add.c"
#include "f32_div.c"
#include "f32_eq.c"
#include "f32_eq_signaling.c"
#include "f32_isSignalingNaN.c"
#include "f32_le.c"
#include "f32_le_quiet.c"
#include "f32_lt.c"
#include "f32_lt_quiet.c"
#include "f32_mul.c"
#include "f32_mulAdd.c"
#include "f32_rem.c"
#include "f32_roundToInt.c"
#include "f32_sqrt.c"
#include "f32_sub.c"
#include "f32_to_extF80.c"
#include "f32_to_extF80M.c"
#include "f32_to_f128.c"
#include "f32_to_f128M.c"
#include "f32_to_f16.c"
#include "f32_to_f64.c"
#include "f32_to_i32.c"
#include "f32_to_i32_r_minMag.c"
#include "f32_to_i64.c"
#include "f32_to_i64_r_minMag.c"
#include "f32_to_ui32.c"
#include "f32_to_ui32_r_minMag.c"
#include "f32_to_ui64.c"
#include "f32_to_ui64_r_minMag.c"
#include "f64_add.c"
#include "f64_div.c"
#include "f64_eq.c"
#include "f64_eq_signaling.c"
#include "f64_isSignalingNaN.c"
#include "f64_le.c"
#include "f64_le_quiet.c"
#include "f64_lt.c"
#include "f64_lt_quiet.c"
#include "f64_mul.c"
#include "f64_mulAdd.c"
#include "f64_rem.c"
#include "f64_roundToInt.c"
#include "f64_sqrt.c"
#include "f64_sub.c"
#include "f64_to_extF80.c"
#include "f64_to_extF80M.c"
#include "f64_to_f128.c"
#include "f64_to_f128M.c"
#include "f64_to_f16.c"
#include "f64_to_f32.c"
#include "f64_to_i32.c"
#include "f64_to_i32_r_minMag.c"
#include "f64_to_i64.c"
#include "f64_to_i64_r_minMag.c"
#include "f64_to_ui32.c"
#include "f64_to_ui32_r_minMag.c"
#include "f64_to_ui64.c"
#include "f64_to_ui64_r_minMag.c"
#include "i32_to_extF80.c"
#include "i32_to_extF80M.c"
#include "i32_to_f128.c"
#include "i32_to_f128M.c"
#include "i32_to_f16.c"
#include "i32_to_f32.c"
#include "i32_to_f64.c"
#include "i64_to_extF80.c"
#include "i64_to_extF80M.c"
#include "i64_to_f128.c"
#include "i64_to_f128M.c"
#include "i64_to_f16.c"
#include "i64_to_f32.c"
#include "i64_to_f64.c"
#include "s_add128.c"
#include "s_add256M.c"
#include "s_addCarryM.c"
#include "s_addComplCarryM.c"
#include "s_addExtF80M.c"
#include "s_addF128M.c"
#include "s_addM.c"
#include "s_addMagsExtF80.c"
#include "s_addMagsF128.c"
#include "s_addMagsF16.c"
#include "s_addMagsF32.c"
#include "s_addMagsF64.c"
#include "s_approxRecip_1Ks.c"
#include "s_approxRecip32_1.c"
#include "s_approxRecipSqrt_1Ks.c"
#include "s_approxRecipSqrt32_1.c"
#include "s_commonNaNToExtF80M.c"
#include "s_commonNaNToExtF80UI.c"
#include "s_commonNaNToF128M.c"
#include "s_commonNaNToF128UI.c"
#include "s_commonNaNToF16UI.c"
#include "s_commonNaNToF32UI.c"
#include "s_commonNaNToF64UI.c"
#include "s_compare128M.c"
#include "s_compare96M.c"
#include "s_compareNonnormExtF80M.c"
#include "s_countLeadingZeros16.c"
#include "s_countLeadingZeros32.c"
#include "s_countLeadingZeros64.c"
#include "s_countLeadingZeros8.c"
#include "s_eq128.c"
#include "s_extF80MToCommonNaN.c"
#include "s_extF80UIToCommonNaN.c"
#include "s_f128MToCommonNaN.c"
#include "s_f128UIToCommonNaN.c"
#include "s_f16UIToCommonNaN.c"
#include "s_f32UIToCommonNaN.c"
#include "s_f64UIToCommonNaN.c"
#include "s_invalidExtF80M.c"
#include "s_invalidF128M.c"
#include "s_isNaNF128M.c"
#include "s_le128.c"
#include "s_lt128.c"
#include "s_mul128By32.c"
#include "s_mul128MTo256M.c"
#include "s_mul128To256M.c"
#include "s_mul64ByShifted32To128.c"
#include "s_mul64To128.c"
#include "s_mul64To128M.c"
#include "s_mulAddF128.c"
#include "s_mulAddF128M.c"
#include "s_mulAddF16.c"
#include "s_mulAddF32.c"
#include "s_mulAddF64.c"
#include "s_negXM.c"
#include "s_normExtF80SigM.c"
#include "s_normRoundPackMToExtF80M.c"
#include "s_normRoundPackMToF128M.c"
#include "s_normRoundPackToExtF80.c"
#include "s_normRoundPackToF128.c"
#include "s_normRoundPackToF16.c"
#include "s_normRoundPackToF32.c"
#include "s_normRoundPackToF64.c"
#include "s_normSubnormalExtF80Sig.c"
#include "s_normSubnormalF128Sig.c"
#include "s_normSubnormalF128SigM.c"
#include "s_normSubnormalF16Sig.c"
#include "s_normSubnormalF32Sig.c"
#include "s_normSubnormalF64Sig.c"
#include "s_propagateNaNExtF80M.c"
#include "s_propagateNaNExtF80UI.c"
#include "s_propagateNaNF128M.c"
#include "s_propagateNaNF128UI.c"
#include "s_propagateNaNF16UI.c"
#include "s_propagateNaNF32UI.c"
#include "s_propagateNaNF64UI.c"
#include "s_remStepMBy32.c"
#include "s_roundMToI64.c"
#include "s_roundMToUI64.c"
#include "s_roundPackMToExtF80M.c"
#include "s_roundPackMToF128M.c"
#include "s_roundPackToExtF80.c"
#include "s_roundPackToF128.c"
#include "s_roundPackToF16.c"
#include "s_roundPackToF32.c"
#include "s_roundPackToF64.c"
#include "s_roundToI32.c"
#include "s_roundToI64.c"
#include "s_roundToUI32.c"
#include "s_roundToUI64.c"
#include "s_shiftLeftM.c"
#include "s_shiftNormSigF128M.c"
#include "s_shiftRightJam128.c"
#include "s_shiftRightJam128Extra.c"
#include "s_shiftRightJam256M.c"
#include "s_shiftRightJam32.c"
#include "s_shiftRightJam64.c"
#include "s_shiftRightJam64Extra.c"
#include "s_shiftRightJamM.c"
#include "s_shiftRightM.c"
#include "s_shortShiftLeft128.c"
#include "s_shortShiftLeft64To96M.c"
#include "s_shortShiftLeftM.c"
#include "s_shortShiftRight128.c"
#include "s_shortShiftRightExtendM.c"
#include "s_shortShiftRightJam128.c"
#include "s_shortShiftRightJam128Extra.c"
#include "s_shortShiftRightJam64.c"
#include "s_shortShiftRightJam64Extra.c"
#include "s_shortShiftRightJamM.c"
#include "s_shortShiftRightM.c"
#include "s_sub128.c"
#include "s_sub1XM.c"
#include "s_sub256M.c"
#include "s_subM.c"
#include "s_subMagsExtF80.c"
#include "s_subMagsF128.c"
#include "s_subMagsF16.c"
#include "s_subMagsF32.c"
#include "s_subMagsF64.c"
#include "s_tryPropagateNaNExtF80M.c"
#include "s_tryPropagateNaNF128M.c"
#include "softfloat_raiseFlags.c"
#include "softfloat_state.c"
#include "ui32_to_extF80.c"
#include "ui32_to_extF80M.c"
#include "ui32_to_f128.c"
#include "ui32_to_f128M.c"
#include "ui32_to_f16.c"
#include "ui32_to_f32.c"
#include "ui32_to_f64.c"
#include "ui64_to_extF80.c"
#include "ui64_to_extF80M.c"
#include "ui64_to_f128.c"
#include "ui64_to_f128M.c"
#include "ui64_to_f16.c"
#include "ui64_to_f32.c"
#include "ui64_to_f64.c"

//------------------------------------------------------------------------------
// config
//------------------------------------------------------------------------------

#if 1
#define FLATTEN __attribute__((__flatten__))
#else
#define FLATTEN
#endif

//------------------------------------------------------------------------------
// types
//------------------------------------------------------------------------------

typedef union F32_pun {
    float flt;
    float32_t soft;
    uint32_t bin;
} F32_pun;

typedef union F64_pun {
    long double flt;
    float64_t soft;
    uint64_t bin;
} F64_pun;

//------------------------------------------------------------------------------
// utility
//------------------------------------------------------------------------------

#include <fenv.h>

static uint_fast8_t GET_FENV_SOFTFLOAT_ROUNDING(void) {
    switch(fegetround()) {
        default:
        case FE_TONEAREST : return softfloat_round_near_even ;
        case FE_TOWARDZERO: return softfloat_round_minMag    ;
        case FE_DOWNWARD  : return softfloat_round_min       ;
        case FE_UPWARD    : return softfloat_round_max       ;
    }
}

//------------------------------------------------------------------------------
// float/double functions
//------------------------------------------------------------------------------

FLATTEN
float fmaf(float x, float y, float z) {
    F32_pun arg_x, arg_y, arg_z, ret;
    arg_x.flt = x;
    arg_y.flt = y;
    arg_z.flt = z;
    ret.soft = f32_mulAdd(arg_x.soft, arg_y.soft, arg_z.soft);
    return ret.flt;
}

double fma(double, double, double) __attribute__((alias("fmaf")));

//------------------------------------------------------------------------------
// long double functions
//------------------------------------------------------------------------------

FLATTEN
long double __dadd_c(long double const *__restrict y_ptr, long double x) {
    long double y = *y_ptr;
    F64_pun arg_x, arg_y, ret;
    arg_x.flt = x;
    arg_y.flt = y;
    ret.soft = f64_add(arg_x.soft, arg_y.soft);
    return ret.flt;
}

FLATTEN
long double __dsub_c(long double const *__restrict y_ptr, long double x) {
    long double y = *y_ptr;
    F64_pun arg_x, arg_y, ret;
    arg_x.flt = x;
    arg_y.flt = y;
    ret.soft = f64_sub(arg_x.soft, arg_y.soft);
    return ret.flt;
}

FLATTEN
long double __dmul_c(long double const *__restrict y_ptr, long double x) {
    long double y = *y_ptr;
    F64_pun arg_x, arg_y, ret;
    arg_x.flt = x;
    arg_y.flt = y;
    ret.soft = f64_mul(arg_x.soft, arg_y.soft);
    return ret.flt;
}

FLATTEN
long double __ddiv_c(long double const *__restrict y_ptr, long double x) {
    long double y = *y_ptr;
    F64_pun arg_x, arg_y, ret;
    arg_x.flt = x;
    arg_y.flt = y;
    ret.soft = f64_div(arg_x.soft, arg_y.soft);
    return ret.flt;
}

FLATTEN
long double __drem_c(long double const *__restrict y_ptr, long double x) {
    long double y = *y_ptr;
    F64_pun arg_x, arg_y, ret;
    arg_x.flt = x;
    arg_y.flt = y;
    ret.soft = f64_rem(arg_x.soft, arg_y.soft);
    return ret.flt;
}

FLATTEN
long double fmodl(long double x, long double y) {
    F64_pun arg_x, arg_y, ret;
    arg_x.flt = x;
    arg_y.flt = y;
    ret.soft = f64_rem(arg_x.soft, arg_y.soft);
    return ret.flt;
}

FLATTEN
long double sqrtl(long double x) {
    F64_pun arg_x, ret;
    arg_x.flt = x;
    ret.soft = f64_sqrt(arg_x.soft);
    return ret.flt;
}

FLATTEN
long double fmal(long double x, long double y, long double z) {
    F64_pun arg_x, arg_y, arg_z, ret;
    arg_x.flt = x;
    arg_y.flt = y;
    arg_z.flt = z;
    ret.soft = f64_mulAdd(arg_x.soft, arg_y.soft, arg_z.soft);
    return ret.flt;
}

FLATTEN
long double floorl(long double x) {
    F64_pun arg_x, ret;
    arg_x.flt = x;
    ret.soft = f64_roundToInt(arg_x.soft, softfloat_round_min, false);
    return ret.flt;
}

FLATTEN
long double ceill(long double x) {
    F64_pun arg_x, ret;
    arg_x.flt = x;
    ret.soft = f64_roundToInt(arg_x.soft, softfloat_round_max, false);
    return ret.flt;
}

FLATTEN
long double roundl(long double x) {
    F64_pun arg_x, ret;
    arg_x.flt = x;
    ret.soft = f64_roundToInt(arg_x.soft, softfloat_round_near_maxMag, false);
    return ret.flt;
}

FLATTEN
long double roundevenl(long double x) {
    F64_pun arg_x, ret;
    arg_x.flt = x;
    ret.soft = f64_roundToInt(arg_x.soft, softfloat_round_near_even, false);
    return ret.flt;
}

FLATTEN
long lroundl(long double x) {
    F64_pun arg_x;
    arg_x.flt = x;
    return f64_to_i32(arg_x.soft, softfloat_round_near_maxMag, false);
}

FLATTEN
long long llroundl(long double x) {
    F64_pun arg_x;
    arg_x.flt = x;
    return f64_to_i64(arg_x.soft, softfloat_round_near_maxMag, false);
}

FLATTEN
long double nearbyintl(long double x) {
    F64_pun arg_x, ret;
    arg_x.flt = x;
    ret.soft = f64_roundToInt(arg_x.soft, GET_FENV_SOFTFLOAT_ROUNDING(), false);
    return ret.flt;
}

FLATTEN
long double rintl(long double x) {
    F64_pun arg_x, ret;
    arg_x.flt = x;
    ret.soft = f64_roundToInt(arg_x.soft, GET_FENV_SOFTFLOAT_ROUNDING(), true);
    return ret.flt;
}

FLATTEN
long lrintl(long double x) {
    F64_pun arg_x;
    arg_x.flt = x;
    return f64_to_i32(arg_x.soft, GET_FENV_SOFTFLOAT_ROUNDING(), true);
}

FLATTEN
long long llrintl(long double x) {
    F64_pun arg_x;
    arg_x.flt = x;
    return f64_to_i64(arg_x.soft, GET_FENV_SOFTFLOAT_ROUNDING(), true);
}
