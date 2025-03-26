#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <stdlib.h>

#include "f32_rounding_LUT.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

typedef union F32_pun {
    float flt;
    uint32_t bin;
} F32_pun;

float my_floorf(float);

float my_ceilf(float);

size_t run_test(const char** failed_func) {
    typedef F32_pun input_t;
    typedef struct { F32_pun r_floor; F32_pun r_ceil; F32_pun r_round; } output_t;

    const size_t length = ARRAY_LENGTH(f32_rounding_LUT_input);
    const input_t  *input  = (const input_t* )((const void*)f32_rounding_LUT_input );
    const output_t *output = (const output_t*)((const void*)f32_rounding_LUT_output);

    for (size_t i = 0; i < length; i++) {
        F32_pun result;
        input_t value = input[i];
        result.flt = my_floorf(value.flt);
        if (result.bin != output[i].r_floor.bin) {
            if (!(isnan(result.flt) && isnan(output[i].r_floor.flt))) {
                *failed_func = "floor";
                // printf(
                //     "I: %08lX T: %zu\nG: %08lX\nT: %08lX\n",
                //     value.bin, i,
                //     result.bin,
                //     output[i].r_floor.bin
                // );
                // while (!os_GetCSC());
                return i;
            }
        }

        result.flt = my_ceilf(value.flt);
        if (result.bin != output[i].r_ceil.bin) {
            if (!(isnan(result.flt) && isnan(output[i].r_ceil.flt))) {
                *failed_func = "ceil";
                return i;
            }
        }

        // result.flt = roundf(value.flt);
        // if (result.bin != output[i].r_round.bin) {
        //     if (!(isnan(result.flt) && isnan(output[i].r_round.flt))) {
        //         *failed_func = "round";
        //         return i;
        //     }
        // }
    }

    /* passed all */
    *failed_func = "none";
    return SIZE_MAX;
}

int main(void) {
    os_ClrHome();
    const char* failed_func;
    size_t fail_index = run_test(&failed_func);
    if (fail_index == SIZE_MAX) {
        printf("All tests passed");
    } else {
        printf("Failed test: %zu %s", fail_index, failed_func);
    }

    while (!os_GetCSC());

    return 0;
}
