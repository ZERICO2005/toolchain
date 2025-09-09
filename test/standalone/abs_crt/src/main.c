#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ti/sprintf.h>
#include <ez80_builtin.h>

//------------------------------------------------------------------------------
// Config
//------------------------------------------------------------------------------

#define RANDOM_TEST_COUNT 256

// define to 0 or 1
#define DEBUG_DIAGNOSTICS 0

#define AUTOTEST_SEED 0x7184CE

//------------------------------------------------------------------------------
// Tests
//------------------------------------------------------------------------------

#define C(expr) if (!(expr)) { return __LINE__; }

#define TEST(test) { ret = test; if (ret != 0) { return ret; }}

#ifndef DEBUG_DIAGNOSTICS
#error "DEBUG_DIAGNOSTICS needs to be defined to 0 or 1"
#endif

#if RANDOM_TEST_COUNT < 4
#error "RANDOM_TEST_COUNT is out of range"
#endif

#if DEBUG_DIAGNOSTICS
#define test_printf printf
#else
#define test_printf(...)
#endif

#define CMP(format, x, truth, guess) do { \
    if ((truth) != (guess)) { \
        test_printf("I: " format "\nT: " format "\nG: " format "\n", (x), (truth), (guess)); \
        return __LINE__; \
    } \
} while(0)

static_assert(RAND_MAX == INT_MAX, "RAND_MAX has changed");

#define rand8() ((int8_t)rand())

#define rand16() ((int16_t)rand())

__attribute__((__unused__)) static int24_t rand24(void) {
    union {
        int24_t u24;
        struct {
            uint16_t lo16;
            uint8_t hi8;
        } part;
    } split;
    split.part.lo16 = (uint16_t)rand();
    split.part.hi8 = (uint8_t)rand();
    return split.u24;
}

__attribute__((__unused__)) static int32_t rand32(void) {
    union {
        int32_t u32;
        uint16_t u16[2];
    } split;
    split.u16[0] = (uint16_t)rand();
    split.u16[1] = (uint16_t)rand();
    return split.u32;
}

__attribute__((__unused__)) static int48_t rand48(void) {
    union {
        int48_t u48;
        uint16_t u16[3];
    } split;
    split.u16[0] = (uint16_t)rand();
    split.u16[1] = (uint16_t)rand();
    split.u16[2] = (uint16_t)rand();
    return split.u48;
}

__attribute__((__unused__)) static int64_t rand64(void) {
    union {
        int64_t u64;
        uint16_t u16[4];
    } split;
    split.u16[0] = (uint16_t)rand();
    split.u16[1] = (uint16_t)rand();
    split.u16[2] = (uint16_t)rand();
    split.u16[3] = (uint16_t)rand();
    return split.u64;
}

int8_t CRT_babs_crt(int8_t);
int16_t CRT_sabs_crt(int16_t);
int24_t CRT_iabs_crt(int24_t);
int32_t CRT_labs_crt(int32_t);
int48_t CRT_i48abs_crt(int48_t);
int64_t CRT_llabs_crt(int64_t);
int16_t CRT_sabs_crt_fast(int16_t);
int24_t CRT_iabs_crt_fast(int24_t);
int32_t CRT_labs_crt_fast(int32_t);
int48_t CRT_i48abs_crt_fast(int48_t);
int64_t CRT_llabs_crt_fast(int64_t);

typedef struct reg_group {
    union {
        struct {
            uint24_t HL;
            uint24_t DE;
            uint24_t BC;
        };
        struct {
            uint8_t L;
            uint8_t H;
            uint8_t UHL;
            uint8_t E;
            uint8_t D;
            uint8_t UDE;
            uint8_t C;
            uint8_t B;
            uint8_t UBC;
        };
    };
    uint8_t A;
    // uint24_t IX;
    // uint24_t IY;
} reg_group;
extern reg_group prev_reg;
extern reg_group next_reg;

void print_reg(void) {
    test_printf(
        "A: %02X -> %02X\n"\
        "HL: %06X -> %06X\n"\
        "BC: %06X -> %06X\n"\
        "DE: %06X -> %06X\n",
        prev_reg.A , next_reg.A ,
        prev_reg.HL, next_reg.HL,
        prev_reg.DE, next_reg.DE,
        prev_reg.BC, next_reg.BC
    );
}

static bool test_UBC_UDE_UHL(void) {
    if (
        (prev_reg.BC  == next_reg.BC ) &&
        (prev_reg.DE  == next_reg.DE ) &&
        (prev_reg.HL  == next_reg.HL )
    ) {
        return true;
    }
    print_reg();
    return false;
}

static bool test_A_UBC_UDE(void) {
    if (
        (prev_reg.A   == next_reg.A  ) &&
        (prev_reg.BC  == next_reg.BC ) &&
        (prev_reg.DE  == next_reg.DE )
    ) {
        return true;
    }
    print_reg();
    return false;
}

static bool test_A_UBC_UD(void) {
    if (
        (prev_reg.A   == next_reg.A  ) &&
        (prev_reg.BC  == next_reg.BC ) &&
        (prev_reg.UDE == next_reg.UDE) &&
        (prev_reg.D   == next_reg.D  )
    ) {
        return true;
    }
    print_reg();
    return false;
}

static bool test_A_UBC(void) {
    if (
        (prev_reg.A   == next_reg.A  ) &&
        (prev_reg.BC  == next_reg.BC )
    ) {
        return true;
    }
    print_reg();
    return false;
}

static bool test_A(void) {
    if (prev_reg.A   == next_reg.A) {
        return true;
    }
    print_reg();
    return false;
}

int test_babs_crt(void) {
    for (int i = INT8_MIN; i <= INT8_MAX; i++) {
        int8_t truth, guess, x;
        x = (int8_t)i;
        truth = (int8_t)abs(x);
        guess = CRT_babs_crt(x);
        CMP("%02X", x, truth, guess);
        C((test_UBC_UDE_UHL()));
    }
    return 0;
}

int test_sabs_crt(void) {
    C(CRT_sabs_crt(         0) ==         0); C((test_A_UBC_UDE()));
    C(CRT_sabs_crt(         1) ==         1); C((test_A_UBC_UDE()));
    C(CRT_sabs_crt(        -1) ==         1); C((test_A_UBC_UDE()));
    C(CRT_sabs_crt( INT16_MAX) == INT16_MAX); C((test_A_UBC_UDE()));
    C(CRT_sabs_crt(-INT16_MAX) == INT16_MAX); C((test_A_UBC_UDE()));
    C(CRT_sabs_crt( INT16_MIN) == INT16_MIN); C((test_A_UBC_UDE()));
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int16_t truth, guess, x;
        x = rand16();
        truth = (int16_t)abs(x);
        guess = CRT_sabs_crt(x);
        CMP("%04X", x, truth, guess);
        C((test_A_UBC_UDE()));
    }
    return 0;
}

int test_sabs_crt_fast(void) {
    C(CRT_sabs_crt(         0) ==         0);
    C(CRT_sabs_crt(         1) ==         1);
    C(CRT_sabs_crt(        -1) ==         1);
    C(CRT_sabs_crt( INT16_MAX) == INT16_MAX);
    C(CRT_sabs_crt(-INT16_MAX) == INT16_MAX);
    C(CRT_sabs_crt( INT16_MIN) == INT16_MIN);
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int16_t truth, guess, x;
        x = rand16();
        truth = (int16_t)abs(x);
        guess = CRT_sabs_crt_fast(x);
        CMP("%04X", x, truth, guess);
    }
    return 0;
}

int test_iabs_crt(void) {
    C(CRT_iabs_crt(         0) ==         0); C((test_A_UBC_UDE()));
    C(CRT_iabs_crt(         1) ==         1); C((test_A_UBC_UDE()));
    C(CRT_iabs_crt(        -1) ==         1); C((test_A_UBC_UDE()));
    C(CRT_iabs_crt( INT24_MAX) == INT24_MAX); C((test_A_UBC_UDE()));
    C(CRT_iabs_crt(-INT24_MAX) == INT24_MAX); C((test_A_UBC_UDE()));
    C(CRT_iabs_crt( INT24_MIN) == INT24_MIN); C((test_A_UBC_UDE()));
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int24_t truth, guess, x;
        x = rand24();
        truth = abs(x);
        guess = CRT_iabs_crt(x);
        CMP("%06X", x, truth, guess);
        C((test_A_UBC_UDE()));
    }
    return 0;
}

int test_iabs_crt_fast(void) {
    C(CRT_iabs_crt(         0) ==         0);
    C(CRT_iabs_crt(         1) ==         1);
    C(CRT_iabs_crt(        -1) ==         1);
    C(CRT_iabs_crt( INT24_MAX) == INT24_MAX);
    C(CRT_iabs_crt(-INT24_MAX) == INT24_MAX);
    C(CRT_iabs_crt( INT24_MIN) == INT24_MIN);
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int24_t truth, guess, x;
        x = rand24();
        truth = abs(x);
        guess = CRT_iabs_crt(x);
        CMP("%06X", x, truth, guess);
    }
    return 0;
}

int test_labs_crt(void) {
    C(CRT_labs_crt(         0) ==         0); C((test_A_UBC_UD()));
    C(CRT_labs_crt(         1) ==         1); C((test_A_UBC_UD()));
    C(CRT_labs_crt(        -1) ==         1); C((test_A_UBC_UD()));
    C(CRT_labs_crt( INT32_MAX) == INT32_MAX); C((test_A_UBC_UD()));
    C(CRT_labs_crt(-INT32_MAX) == INT32_MAX); C((test_A_UBC_UD()));
    C(CRT_labs_crt( INT32_MIN) == INT32_MIN); C((test_A_UBC_UD()));
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int32_t truth, guess, x;
        x = rand32();
        truth = labs(x);
        guess = CRT_labs_crt(x);
        CMP("%08lX", x, truth, guess);
        C((test_A_UBC_UD()));
    }
    return 0;
}

int test_labs_crt_fast(void) {
    C(CRT_labs_crt(         0) ==         0);
    C(CRT_labs_crt(         1) ==         1);
    C(CRT_labs_crt(        -1) ==         1);
    C(CRT_labs_crt( INT32_MAX) == INT32_MAX);
    C(CRT_labs_crt(-INT32_MAX) == INT32_MAX);
    C(CRT_labs_crt( INT32_MIN) == INT32_MIN);
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int32_t truth, guess, x;
        x = rand32();
        truth = labs(x);
        guess = CRT_labs_crt(x);
        CMP("%08lX", x, truth, guess);
    }
    return 0;
}

int test_i48abs_crt(void) {
    C(CRT_i48abs_crt(         0) ==         0); C((test_A_UBC()));
    C(CRT_i48abs_crt(         1) ==         1); C((test_A_UBC()));
    C(CRT_i48abs_crt(        -1) ==         1); C((test_A_UBC()));
    C(CRT_i48abs_crt( INT48_MAX) == INT48_MAX); C((test_A_UBC()));
    C(CRT_i48abs_crt(-INT48_MAX) == INT48_MAX); C((test_A_UBC()));
    C(CRT_i48abs_crt( INT48_MIN) == INT48_MIN); C((test_A_UBC()));
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int48_t truth, guess, x;
        x = rand48();
        truth = i48abs(x);
        guess = CRT_i48abs_crt(x);
        CMP("%012llX", (uint64_t)x, (uint64_t)truth, (uint64_t)guess);
        C((test_A_UBC()));
    }
    return 0;
}

#if 0
int test_i48abs_crt_fast(void) {
    C(CRT_i48abs_crt(         0) ==         0);
    C(CRT_i48abs_crt(         1) ==         1);
    C(CRT_i48abs_crt(        -1) ==         1);
    C(CRT_i48abs_crt( INT48_MAX) == INT48_MAX);
    C(CRT_i48abs_crt(-INT48_MAX) == INT48_MAX);
    C(CRT_i48abs_crt( INT48_MIN) == INT48_MIN);
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int48_t truth, guess, x;
        x = rand48();
        truth = i48abs(x);
        guess = CRT_i48abs_crt_fast(x);
        CMP("%012llX", (uint64_t)x, (uint64_t)truth, (uint64_t)guess);
    }
    return 0;
}
#endif

int test_llabs_crt(void) {
    C(CRT_llabs_crt(         0) ==         0); C((test_A()));
    C(CRT_llabs_crt(         1) ==         1); C((test_A()));
    C(CRT_llabs_crt(        -1) ==         1); C((test_A()));
    C(CRT_llabs_crt( INT64_MAX) == INT64_MAX); C((test_A()));
    C(CRT_llabs_crt(-INT64_MAX) == INT64_MAX); C((test_A()));
    C(CRT_llabs_crt( INT64_MIN) == INT64_MIN); C((test_A()));
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int64_t truth, guess, x;
        x = rand64();
        truth = llabs(x);
        guess = CRT_llabs_crt(x);
        CMP("%016llX", x, truth, guess);
        C((test_A()));
    }
    return 0;
}

int test_llabs_crt_fast(void) {
    C(CRT_llabs_crt(         0) ==         0);
    C(CRT_llabs_crt(         1) ==         1);
    C(CRT_llabs_crt(        -1) ==         1);
    C(CRT_llabs_crt( INT64_MAX) == INT64_MAX);
    C(CRT_llabs_crt(-INT64_MAX) == INT64_MAX);
    C(CRT_llabs_crt( INT64_MIN) == INT64_MIN);
    for (int i = 0; i < RANDOM_TEST_COUNT; i++) {
        int64_t truth, guess, x;
        x = rand64();
        truth = llabs(x);
        guess = CRT_llabs_crt_fast(x);
        CMP("%016llX", x, truth, guess);
    }
    return 0;
}

int run_tests(void) {
    srand(AUTOTEST_SEED);
    int ret = 0;

    TEST(test_babs_crt());
    TEST(test_sabs_crt());
    TEST(test_sabs_crt_fast());
    TEST(test_iabs_crt());
    TEST(test_iabs_crt_fast());
    TEST(test_labs_crt());
    TEST(test_labs_crt_fast());
    TEST(test_i48abs_crt());
    // TEST(test_i48abs_crt_fast());
    TEST(test_llabs_crt());
    TEST(test_llabs_crt_fast());

    return ret;
}

int main(void) {
    os_ClrHome();
    int failed_test = run_tests();
    if (failed_test != 0) {
        char buf[sizeof("Failed test L-8388608\n")];
        boot_sprintf(buf, "Failed test L%d\n", failed_test);
        fputs(buf, stdout);
    } else {
        fputs("All tests passed", stdout);
    }

    while (!os_GetCSC());

    return 0;
}
