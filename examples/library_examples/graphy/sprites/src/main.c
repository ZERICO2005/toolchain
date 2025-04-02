#include <ti/getcsc.h>
#include <graphy.h>

/* Include the converted graphics file */
#include "gfx/gfx.h"

int main(void)
{
    /* Initialize graphics drawing */
    gfy_Begin();

    /* Set the palette for sprites */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);

    /* These were set in the image conversion file */
    gfy_SetTransparentColor(0);
    gfy_FillScreen(1);

    /* Draw a normal sprite */
    gfy_Sprite_NoClip(oiram, 130, 110);

    /* A transparent sprite allows the background to show */
    gfy_TransparentSprite_NoClip(oiram, 190, 110);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
