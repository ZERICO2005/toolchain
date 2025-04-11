#include <ti/getcsc.h>
#include <compression.h>
#include <graphy.h>

/* Include the converted graphics appvar file */
#include "gfx/vargfx.h"

int main(void)
{
    unsigned int i;
    unsigned int x, y;
    gfy_sprite_t *tile;

    /* This function is automatically created by convimg */
    /* Simply call it to load in graphics from the appvar */
    /* The common function name is <appvar name>_init */
    /* This function returns 1 on success or 0 if the appvar failed to load. */
    if (vargfy_init() == 0)
    {
        return 1;
    }

    /* Allocate storage for temporary decompression */
    /* Same as gfy_AllocSprite() */
    tile = gfy_MallocSprite(image_tile_width, image_tile_height);

    /* Start the graphics */
    gfy_Begin();

    /* Set intial drawing positions */
    x = 0;
    y = 0;

    /* Draw to buffer */
    gfy_SetDrawBuffer();

    /* Decompress the tiles */
    for (i = 0; i < image_tiles_num; ++i)
    {
        zx7_Decompress(tile, image_tiles_compressed[i]);
        gfy_Sprite_NoClip(tile, x, y);

        x += image_tile_width;
        if (x >= GFY_LCD_WIDTH)
        {
            y += image_tile_height;
            x = 0;
        }
    }

    /* Copy to visible screen */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);
    gfy_BlitBuffer();

    /* Waits for a key */
    while (!os_GetCSC());

    /* Free the allocated temp tile */
    free(tile);

    /* End graphics drawing */
    gfy_End();

    return 0;
}
