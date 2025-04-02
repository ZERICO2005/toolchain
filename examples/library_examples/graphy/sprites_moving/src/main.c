#include <sys/util.h>
#include <graphy.h>
#include <keypadc.h>

/* Include the sprite data */
#include "gfx/gfx.h"

#define START_X ((GFY_LCD_WIDTH - oiram_width) / 2)
#define START_Y ((GFY_LCD_HEIGHT - oiram_height) / 2)

/* Create a buffer to store the background behind the sprite */
gfy_UninitedSprite(background, oiram_width, oiram_height);

void DrawSprite(int x, int y);

int main(void)
{
    int i;

    background->width = oiram_width;
    background->height = oiram_height;

    /* Coordinates used for the sprite */
    int x = START_X;
    int y = START_Y;

    /* Initialize the graphics */
    gfy_Begin();

    /* Set the palette for the sprites */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);
    gfy_SetTransparentColor(0);

    /* Draw to the offscreen buffer */
    gfy_SetDrawBuffer();
    gfy_FillScreen(1);

    /* Draw a bunch of random sprites to demonstrate the partial redraw */
    for (i = 0; i < 6; ++i)
    {
        gfy_Sprite_NoClip(ubuntu,
                          randInt(0, 320 - ubuntu_width),
                          randInt(0, 240 - ubuntu_height));
    }

    /* Get the original background behind the sprite */
    gfy_GetSprite(background, x, y);

    /* Draw the main sprite */
    DrawSprite(x, y);

    /* Copy the buffer to the screen */
    /* Same as gfy_Blit(gfy_buffer) */
    gfy_BlitBuffer();

    do
    {
        kb_key_t arrows;

        /* Scan the keypad to update kb_Data */
        kb_Scan();

        /* Get the arrow key statuses */
        arrows = kb_Data[7];

        /* Check if any arrows are pressed */
        if (arrows)
        {
            /* Do different directions depending on the keypress */
            if (arrows & kb_Right)
            {
                x += 2;
            }
            if (arrows & kb_Left)
            {
                x -= 2;
            }
            if (arrows & kb_Down)
            {
                y += 2;
            }
            if (arrows & kb_Up)
            {
                y -= 2;
            }

            /* Render the sprite */
            DrawSprite(x, y);

            /* Copy the buffer to the screen */
            /* Same as gfy_Blit(gfy_buffer) */
            gfy_BlitBuffer();
        }

    } while (kb_Data[6] != kb_Clear);

    /* End graphics drawing */
    gfy_End();

    return 0;
}

/* Function for drawing the main sprite */
void DrawSprite(int x, int y)
{
    static int oldX = START_X;
    static int oldY = START_Y;

    /* Render the original background */
    gfy_Sprite(background, oldX, oldY);

    /* Get the background behind the sprite */
    gfy_GetSprite(background, x, y);

    /* Render the sprite */
    gfy_TransparentSprite(oiram, x, y);

    oldX = x;
    oldY = y;
}
