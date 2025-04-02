#include <string.h>
#include <ti/getcsc.h>
#include <graphy.h>

#define TEXT_FG_COLOR            247
#define TEXT_BG_COLOR            0
#define FONT_WIDTH               8
#define FONT_HEIGHT              8

void PrintCenteredFlippedText(char *str);

int main(void)
{
    /* Initialize graphics drawing */
    gfy_Begin();
    gfy_ZeroScreen();

    /* Setup the text */
    gfy_SetTextFGColor(TEXT_FG_COLOR);
    gfy_SetTextBGColor(TEXT_BG_COLOR);
    gfy_SetMonospaceFont(FONT_WIDTH);

    /* Print some upside down text */
    PrintCenteredFlippedText("THIS TEXT IS FLIPPED");

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}

void PrintCenteredFlippedText(char *str)
{
    int x, y;
    char *string = str + strlen(str) - 1;
    gfy_TempSprite(ch, 8, 8);

    x = (GFY_LCD_WIDTH - gfy_GetStringWidth(str)) / 2;
    y = (GFY_LCD_HEIGHT - FONT_HEIGHT) / 2;

    while (*string)
    {
        gfy_Sprite(gfy_RotateSpriteHalf(gfy_GetSpriteChar(*string), ch), x, y);
        x += gfy_GetCharWidth(*string--);
    }
}
