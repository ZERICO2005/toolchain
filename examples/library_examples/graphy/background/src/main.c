#include <ti/getcsc.h>
#include <graphy.h>
#include <compression.h>

#include "gfx/gfx.h"

int main(void)
{
    gfy_Begin();

    gfy_SetPalette(global_palette, sizeof_global_palette, 0);

    zx7_Decompress(gfy_vram, background_compressed);

    while (!os_GetCSC());

    gfy_End();

    return 0;
}
