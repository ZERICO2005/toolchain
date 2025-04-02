#include <ti/getcsc.h>
#include <graphy.h>

int main(void)
{
    uint8_t c;

    /* Initialize graphics drawing */
    gfy_Begin();

    /* Draw to buffer instead of directly to screen */
    /* This is the same as gfy_SetDraw(gfy_buffer) */
    gfy_SetDrawBuffer();

    /* Draw a gradient (to the buffer) */
    for (c = 0; c < 8; c++)
    {
        gfy_SetColor(c);
        gfy_FillRectangle_NoClip(c * (GFY_LCD_WIDTH / 8), 0, GFY_LCD_WIDTH / 8, GFY_LCD_HEIGHT);
    }

    /* Waits for a key */
    while (!os_GetCSC());

    /* Copy parts of the buffer to the screen */
    gfy_BlitLines(gfy_buffer, GFY_LCD_HEIGHT - 32, 32);
    gfy_BlitRectangle(gfy_buffer, GFY_LCD_WIDTH / 4, GFY_LCD_HEIGHT / 4, GFY_LCD_WIDTH / 2, GFY_LCD_HEIGHT / 2);

    /* Waits for a key */
    while (!os_GetCSC());

    /* Copy the full buffer to the screen */
    /* This is the same as gfy_Blit(gfy_buffer) */
    gfy_BlitBuffer();

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
