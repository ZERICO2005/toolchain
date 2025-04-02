#include <ti/getcsc.h>
#include <graphy.h>
#include <stdlib.h>

void PrintCentered(const char *str);

int main(void)
{
    /* Initialize graphics drawing */
    gfy_Begin();

    /* Print the message on the screen */
    PrintCentered("Hello, World!");

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}

/* Prints a screen centered string */
void PrintCentered(const char *str)
{
    gfy_PrintStringXY(str,
                      (GFY_LCD_WIDTH - gfy_GetStringWidth(str)) / 2,
                      (GFY_LCD_HEIGHT - 8) / 2);
}
