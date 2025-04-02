#include <ti/getcsc.h>
#include <graphy.h>

/* Include the converted image data */
#include "gfx/gfx.h"

int main(void)
{
    /* Initialize the 8bpp graphics */
    gfy_Begin();

    /* Set the palette for sprites */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);

    /* These were set in the image conversion file */
    gfy_SetTransparentColor(0);
    gfy_FillScreen(1);

    /* Draw a normal scaled sprite */
    gfy_ScaledSprite_NoClip(oiram, 25, 15, 8, 8);

    /* A transparent sprite allows the background to show */
    gfy_ScaledTransparentSprite_NoClip(oiram, 175, 15, 8, 8);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
