#include <ti/getcsc.h>
#include <compression.h>
#include <graphy.h>

/* Include the converted graphics file */
#include "gfx/gfx.h"

int main(void)
{
    /* Define our sprite */
    gfy_sprite_t *apple;

    /* Allocate space for the decompressed sprite */
    /* Same as gfy_AllocSprite() */
    apple = gfy_MallocSprite(apple_width, apple_height);

    /* Decompress the sprite */
    zx0_Decompress(apple, apple_compressed);

    /* Initialize graphics drawing */
    gfy_Begin();

    /* Set up the palette */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);
    gfy_FillScreen(0);

    /* Draw the decompressed sprite centered */
    gfy_Sprite(apple,
               (GFY_LCD_WIDTH - apple_width) / 2,
               (GFY_LCD_HEIGHT - apple_height) / 2);

    /* Waits for a key */
    while (!os_GetCSC());

    /* Free allocated sprite memory */
    free(apple);

    /* End graphics drawing */
    gfy_End();

    return 0;
}
