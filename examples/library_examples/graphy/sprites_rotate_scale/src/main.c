#include <ti/getcsc.h>
#include <graphy.h>

/* Include the converted image data */
#include "gfx/gfx.h"

int main(void)
{
    uint8_t x = 0;

    /* Initialize graphics drawing */
    gfy_Begin();

    /* Set the palette used by the sprites */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);
    gfy_SetTransparentColor(0);

    /* Draw to buffer to avoid artifacts */
    gfy_SetDrawBuffer();

    /* Rotate the sprite until a key is pressed */
    do
    {

        /* Draw a rotated transparent scaled spite */
        gfy_RotatedScaledTransparentSprite_NoClip(star, 120, 80, 256 - x, 128);

        /* Show the buffered screen */
        gfy_BlitBuffer();

        /* Clear the old drawn sprite */
        gfy_FillScreen(1);

        /* Change the rotation amount */
        x++;

    } while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
