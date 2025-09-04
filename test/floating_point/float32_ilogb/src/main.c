#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/util.h>
#include <ti/sprintf.h>

#include "f32_ilogb_LUT.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

size_t run_test(void) {
    typedef float input_t;
    typedef int output_t;

    const size_t length = ARRAY_LENGTH(f32_ilogb_LUT_input);
    const input_t  *input  = (const input_t* )((const void*)f32_ilogb_LUT_input );
    const output_t *output = (const output_t*)((const void*)f32_ilogb_LUT_output);

    for (size_t i = 0; i < length; i++) {
        int result = ilogbf(input[i]);
        if (result != output[i]) {
            #if 1
                printf("%3zu: %08lX\n\t%d != %d\n", i, input[i], result, output[i]);
            #endif
            return i;
        }
        float f_guess = logbf(input[i]);
        
        switch (fpclassify(input[i])) {
            case FP_NAN:
            case FP_INFINITE:
            {
                float f_truth = fabsf(input[i]);
                if (memcmp(&f_guess, &f_truth, sizeof(float)) != 0) {
                    return i;
                }
            } break;
            case FP_ZERO:
            {
                if (f_guess != -HUGE_VALF) {
                    return i;
                }
            } break;
            default: {
                float f_truth = (float)result;
                if (memcmp(&f_guess, &f_truth, sizeof(float)) != 0) {
                    printf("%08lX != %08lX\n", f_guess, f_truth);
                    // return i;
                }
            } break;
        }
    }

    /* passed all */
    return SIZE_MAX;
}

int main(void) {
    os_ClrHome();
    size_t fail_index = run_test();
    if (fail_index == SIZE_MAX) {
        fputs("All tests passed", stdout);
    } else {
        char buf[sizeof("Failed test: 16777215\n")];
        boot_sprintf(buf, "Failed test: %u\n", fail_index);
        fputs(buf, stdout);
    }

    while (!os_GetCSC());

    return 0;
}
