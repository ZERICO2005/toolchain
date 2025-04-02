#include <ti/getcsc.h>
#include <graphy.h>

/* Include the converted image data */
#include "gfx/gfx.h"

/* Create a buffer the same size as the source sprite */
gfy_UninitedSprite(sprite_buffer, star_width, star_height);

int main(void)
{
    /* Initialize graphics drawing */
    gfy_Begin();

    /* Set the palette used by the sprites */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);

    /* Draw the sprites */
    gfy_TransparentSprite(gfy_FlipSpriteY(star, sprite_buffer), 136, 24);
    gfy_TransparentSprite(gfy_FlipSpriteX(star, sprite_buffer), 136, 168);
    gfy_TransparentSprite(gfy_RotateSpriteC(star, sprite_buffer), 64, 96);
    gfy_TransparentSprite(gfy_RotateSpriteCC(star, sprite_buffer), 208, 96);
    gfy_TransparentSprite(gfy_RotateSpriteHalf(star, sprite_buffer), 136, 96);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
