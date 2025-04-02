#include <ti/getcsc.h>
#include <graphy.h>

int main(void)
{
    /* Starting text posistion */
    int x = GFY_LCD_WIDTH / 2;
    int y = GFY_LCD_HEIGHT / 2;
    int movex = 1;
    int movey = 1;

    /* Initialize graphics drawing */
    gfy_Begin();

    /* Set the text colors */
    gfy_SetTextFGColor(6);
    gfy_SetTextBGColor(255);
    gfy_SetTextTransparentColor(5);

    /* Enable text clipping */
    gfy_SetTextConfig(gfy_text_clip);
    gfy_SetMonospaceFont(8);

    /* Draw to buffer to avoid flicker */
    gfy_SetDrawBuffer();

    /* Draw the box, and set clip region one pixel around box */
    gfy_Rectangle(25, 25, 270, 190);
    
    gfy_SetClipRegion(25 + 1, 25 + 1, 270 + 25 - 1, 190 + 25 - 1);

    /* Bounce text around screen */
    /* Run until any key is pressed */
    do
    {
        x += movex;
        y += movey;

        if (x >= GFY_LCD_WIDTH || x < 0)
        {
            movex = -movex;
        }
        if (y >= GFY_LCD_HEIGHT || y < 0)
        {
            movey = -movey;
        }

        gfy_PrintStringXY("CLIPPED TXT", x, y);

        gfy_BlitBuffer();

        gfy_PrintStringXY("                 ", x, y);

    } while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
