#include <ti/getcsc.h>
#include <graphy.h>

int main(void)
{
    /* Initialize graphics drawing */
    gfy_Begin();

    /* Set the text colors */
    gfy_SetTextFGColor(6);
    gfy_SetTextBGColor(255);

    /* Print some scaled text */
    gfy_SetTextScale(2, 2);
    gfy_PrintStringXY("SCALED TXT", 10, 10);
    gfy_SetTextScale(3, 3);
    gfy_PrintStringXY("SCALED TXT", 10, 40);
    gfy_SetTextScale(4, 4);
    gfy_PrintStringXY("SCALED TXT", 10, 90);
    gfy_SetTextScale(4, 8);
    gfy_PrintStringXY("SCALED TXT", 10, 150);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
