#include <ti/getcsc.h>
#include <graphy.h>
#include <fileioc.h>

/* Include the appvar sprite data */
#include "gfx/vargfx.h"

int main(void)
{
    /* This function is automatically created by convimg */
    /* Simply call it to load in graphics from the appvar */
    /* The common function name is <appvar name>_init */
    /* This function returns 1 on success or 0 if the appvar failed to load. */
    if (vargfy_init() == 0)
    {
        return 1;
    }

    /* Initialize the graphics */
    gfy_Begin();

    /* Set the palette for the sprites */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);

    /* Fill the screen */
    gfy_FillScreen(0);

    /* Draw the sprites directly from the appvar */
    gfy_Sprite(ubuntu, GFY_LCD_WIDTH / 2 - 16, GFY_LCD_HEIGHT / 2 - 16);
    gfy_Sprite(mint, GFY_LCD_WIDTH / 2 + 16, GFY_LCD_HEIGHT / 2 - 16);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
