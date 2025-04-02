#include <ti/getcsc.h>
#include <graphy.h>

int main(void)
{
    /* Initialize graphics drawing */
    gfy_Begin();

    gfy_FillScreen(19);
    gfy_SetColor(255);
    gfy_FillRectangle(100, 100, 60, 40);
    gfy_SetColor(227);
    gfy_FillRectangle(160, 100, 60, 40);

    /* Waits for a key */
    while (!os_GetCSC());

    gfy_ShiftDown(20);
    gfy_ShiftDown(20);

    /* Waits for a key */
    while (!os_GetCSC());

    gfy_ShiftLeft(20);
    gfy_ShiftLeft(20);

    /* Waits for a key */
    while (!os_GetCSC());

    gfy_ShiftUp(20);
    gfy_ShiftUp(20);

    /* Waits for a key */
    while (!os_GetCSC());

    gfy_ShiftRight(20);
    gfy_ShiftRight(20);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
