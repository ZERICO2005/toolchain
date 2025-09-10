#include <math.h>
#include <fenv.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3e, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014, 2015, 2016 The Regents of the University of
California.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions, and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions, and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. Neither the name of the University nor the names of its contributors may
    be used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS", AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE
DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/


typedef union F32_pun {
    float flt;
    uint32_t bin;
} F32_pun;

#ifdef _EZ80
#define test_assert(...)
#else
#define test_assert assert
#endif

#ifdef _EZ80
#define test_feraiseexcept feraiseexcept
#else
#define test_feraiseexcept(...)
#endif

#if 0
#define raise_FE_INEXACT() feraiseexcept(FE_INEXACT)
#else
#define raise_FE_INEXACT()
#endif

#ifdef _EZ80
#define CTLZ_U24(x) __builtin_clz((x))
#define CTLZ_U64(x) __builtin_clzll((x))
#else
#define CTLZ_U24(x) (stdc_leading_zeros(((uint32_t)x)) - 8)
#define CTLZ_U64(x) stdc_leading_zeros((x))
#endif

#ifndef _EZ80
static __attribute__((__unused__))
uint32_t cast_u64_to_u32(uint64_t x) {
    uint32_t ret = (uint32_t)x;
    test_assert(ret == x);
    return ret;
}
static __attribute__((__unused__))
uint16_t cast_u32_to_u16(uint32_t x) {
    uint16_t ret = (uint16_t)x;
    test_assert(ret == x);
    return ret;
}
static __attribute__((__unused__))
uint8_t cast_u32_to_u8(uint32_t x) {
    uint8_t ret = (uint8_t)x;
    test_assert(ret == x);
    return ret;
}
static __attribute__((__unused__))
uint24_t cast_u32_to_u24(uint32_t x) {
    uint24_t ret = (uint24_t)x;
    test_assert(ret == x);
    return ret;
}
static __attribute__((__unused__))
uint8_t cast_u24_to_u8(uint24_t x) {
    uint8_t ret = (uint8_t)x;
    test_assert(ret == x);
    return ret;
}
static __attribute__((__unused__))
uint8_t cast_u16_to_u8(uint16_t x) {
    uint8_t ret = (uint8_t)x;
    test_assert(ret == x);
    return ret;
}
#else
#define cast_u64_to_u32(x) ((uint32_t)(x))
#define cast_u32_to_u16(x) ((uint16_t)(x))
#define cast_u32_to_u24(x) ((uint24_t)(x))
#define cast_u32_to_u8(x) ((uint8_t)(x))
#define cast_u24_to_u8(x) ((uint8_t)(x))
#define cast_u16_to_u8(x) ((uint8_t)(x))
#endif

struct exp24_sig24 { int24_t exp; uint24_t sig; };
union ui32_f32 { uint32_t ui; uint32_t f; };

#define sf_isSigNaNF32UI( uiA ) ((((uiA) & 0x7FC00000) == 0x7F800000) && ((uiA) & 0x003FFFFF))
#define defaultNaNF32UI 0xFFC00000

#define signF32UI( a ) ((bool) ((uint32_t) (a)>>31))
#define expF32UI( a ) ((int24_t) ((a)>>23) & 0xFF)
#define fracF32UI( a ) ((a) & 0x007FFFFF)
#define packToF32UI( sign, exp, sig ) (((uint32_t) (sign)<<31) + ((uint32_t) (exp)<<23) + (sig))

#define isNaNF32UI( a ) (((~(a) & 0x7F800000) == 0) && ((a) & 0x007FFFFF))

static uint64_t
sf_shiftRightJam64( uint64_t a, uint24_t dist )
{
    return
        (dist < 63) ? (a>>dist | ((uint64_t) (a<<((uint8_t)(-dist) & 63)) != 0)) : (a != 0);
}

static uint32_t
sf_shiftRightJam32( uint32_t a, uint24_t dist_u16 )
{
    uint8_t dist = cast_u24_to_u8(dist_u16);
    return
        (dist < 31) ? a>>dist | ((uint32_t) (a<<((uint8_t)(-dist) & 31)) != 0) : (a != 0);
}

static uint32_t
sf_shortShiftRightJam64( uint64_t a, uint8_t dist )
{
    return (uint32_t)(a>>dist | ((a & (((uint64_t) 1<<dist) - 1)) != 0));
}

static struct exp24_sig24
sf_normSubnormalF32Sig( uint24_t sig )
{
    int8_t shiftDist;
    struct exp24_sig24 z;

    shiftDist = (int8_t)CTLZ_U24( sig );
    z.exp = 1 - shiftDist;
    test_assert(z.exp >= INT8_MIN && z.exp < INT8_MAX);
    z.sig = sig<<shiftDist;
    return z;
}

static uint32_t
sf_propagateNaNF32UI( uint32_t uiA, uint32_t uiB )
{
    bool isSigNaNA;

    isSigNaNA = isNaNF32UI( uiA );
    if ( isSigNaNA || sf_isSigNaNF32UI( uiB ) ) {
        test_feraiseexcept(FE_INVALID);
        if ( isSigNaNA ) return uiA | 0x00400000;
    }
    return (isNaNF32UI( uiA ) ? uiA : uiB) | 0x00400000;

}

static uint32_t
sf_roundPackToF32( bool sign, int24_t exp, uint32_t sig )
{
    test_assert(sig < (UINT32_C(1) << 31));
    uint8_t roundIncrement, roundBits;
    bool isTiny;
    uint32_t uiZ;
    union ui32_f32 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    roundIncrement = 0x40;
    roundBits = sig & 0x7F;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( 0xFD <= (uint24_t) exp ) {
        if ( exp < 0 ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            isTiny = (exp < -1) || (sig + roundIncrement < 0x80000000);
            sig = sf_shiftRightJam32( sig, (uint24_t)-exp );
            exp = 0;
            roundBits = sig & 0x7F;
            if ( isTiny && roundBits ) {
                test_feraiseexcept( FE_UNDERFLOW );
            }
        } else if ( (0xFD < exp) || (0x80000000 <= sig + roundIncrement) ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
			test_feraiseexcept(FE_OVERFLOW);
            raise_FE_INEXACT();
            uiZ = packToF32UI( sign, 0xFF, 0 ) - ! roundIncrement;
            goto uiZ;
        }
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sig = (sig + roundIncrement)>>7;
    if ( roundBits ) {
		raise_FE_INEXACT();
    }
    sig &= ~(uint32_t) (!(roundBits ^ 0x40));
    if ( ! sig ) exp = 0;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 /* packReturn: */
    uiZ = packToF32UI( sign, exp, sig );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;
}

static uint32_t _fmaf_impl(uint32_t uiA, uint32_t uiB, uint32_t uiC) {
    const bool    signA = signF32UI( uiA );
    int24_t  expA = expF32UI ( uiA );
    uint24_t sigA = fracF32UI( uiA );
    const bool    signB = signF32UI( uiB );
    int24_t  expB = expF32UI ( uiB );
    uint24_t sigB = fracF32UI( uiB );
    const bool    signC = signF32UI( uiC );
    int24_t  expC = expF32UI ( uiC );
    uint24_t sigC = fracF32UI( uiC );
    const bool sign_XOR = (signA ^ signB);
    bool magBits;
    uint32_t uiZ;
    struct exp24_sig24 normExpSig;
    int24_t expProd;
    uint64_t sigProd;
    bool signZ;
    int24_t expZ;
    uint32_t sigZ;
    int24_t expDiff;
    uint64_t sig64Z, sig64C;
    int8_t shiftDist;
    union ui32_f32 uZ;

    uint48_t sigProd_u48;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( expA == 0xFF ) {
        if ( sigA || ((expB == 0xFF) && sigB) ) { goto propagateNaN_ABC; }
        magBits = (expB || sigB);
        goto infProdArg;
    }
    if ( expB == 0xFF ) {
        if ( sigB ) { goto propagateNaN_ABC; }
        magBits = (expA || sigA);
        goto infProdArg;
    }
    if ( expC == 0xFF ) {
        if ( sigC ) {
            uiZ = 0;
            goto propagateNaN_ZC;
        }
        uiZ = uiC;
        goto uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( ! expA ) {
        if ( ! sigA ) { goto zeroProd; }
        normExpSig = sf_normSubnormalF32Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    if ( ! expB ) {
        if ( ! sigB ) { goto zeroProd; }
        normExpSig = sf_normSubnormalF32Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expProd = expA + expB - 0x7E;
    sigProd_u48 = (((uint48_t)sigA | 0x800000) * (sigB | 0x800000));
    if ( sigProd_u48 < (0x800000000000) ) {
        --expProd;
        sigProd_u48 <<= 1;
    }
    sigProd = (uint64_t)sigProd_u48 << 14;
    signZ = sign_XOR;
    if ( ! expC ) {
        if ( ! sigC ) {
            expZ = expProd - 1;
            sigZ = sf_shortShiftRightJam64( sigProd, 31 );
            goto roundPack;
        }
        normExpSig = sf_normSubnormalF32Sig( sigC );
        expC = normExpSig.exp;
        sigC = normExpSig.sig;
    }
    sigC = (sigC | 0x800000);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expProd - expC;
    if ( sign_XOR == signC ) {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        if ( expDiff <= 0 ) {
            expZ = expC;
            sigZ = cast_u64_to_u32(((uint64_t)sigC << 6) + cast_u64_to_u32(sf_shiftRightJam64( sigProd, (uint24_t)(32 - expDiff) )));
        } else {
            expZ = expProd;
            sig64Z = sigProd + sf_shiftRightJam64((uint64_t)sigC << (32 + 6), (uint24_t)expDiff );
            sigZ = sf_shortShiftRightJam64( sig64Z, 32 );
        }
        if ( sigZ < 0x40000000 ) {
            --expZ;
            sigZ <<= 1;
        }
    } else {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        sig64C = (uint64_t)sigC << (32 + 6);
        if ( expDiff < 0 ) {
            signZ = signC;
            expZ = expC;
            sig64Z = sig64C - sf_shiftRightJam64( sigProd, (uint24_t)(-expDiff) );
        } else if ( ! expDiff ) {
            expZ = expProd;
            sig64Z = sigProd - sig64C;
            if ( ! sig64Z ) { goto completeCancellation; }
            if ( sig64Z & UINT64_C( 0x8000000000000000 ) ) {
                signZ = ! signZ;
                sig64Z = -sig64Z;
            }
        } else {
            expZ = expProd;
            sig64Z = sigProd - sf_shiftRightJam64( sig64C, (uint24_t)expDiff );
        }
        shiftDist = (int8_t)CTLZ_U64( sig64Z ) - 1;
        expZ -= shiftDist;
        shiftDist -= 32;
        if ( shiftDist < 0 ) {
            sigZ = sf_shortShiftRightJam64( sig64Z, (uint8_t)(-shiftDist) );
        } else {
            sigZ = (uint32_t) sig64Z<<shiftDist;
        }
    }
 roundPack:
    return sf_roundPackToF32( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN_ABC:
    uiZ = sf_propagateNaNF32UI( uiA, uiB );
    goto propagateNaN_ZC;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infProdArg:
    if ( magBits ) {
        uiZ = sign_XOR ? UINT32_C(0xFF800000) : UINT32_C(0x7F800000);
        if ( expC != 0xFF ) { goto uiZ; }
        if ( sigC ) { goto propagateNaN_ZC; }
        if ( sign_XOR == signC ) { goto uiZ; }
    }
	test_feraiseexcept(FE_INVALID);
    uiZ = defaultNaNF32UI;
 propagateNaN_ZC:
    uiZ = sf_propagateNaNF32UI( uiZ, uiC );
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zeroProd:
    uiZ = uiC;
    if ( ! (expC || sigC) && (sign_XOR != signC) ) {
 completeCancellation:
        uiZ = 0;
    }
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}

float fmaf(float x, float y, float z) {
    F32_pun x_pun, y_pun, z_pun, ret;
    x_pun.flt = x;
    y_pun.flt = y;
    z_pun.flt = z;
    ret.bin = _fmaf_impl(x_pun.bin, y_pun.bin, z_pun.bin);
    return ret.flt;
}

double fma(double, double, double) __attribute__((alias("fmaf")));
