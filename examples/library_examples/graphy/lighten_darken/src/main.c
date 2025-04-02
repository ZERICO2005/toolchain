#include <ti/getcsc.h>
#include <graphy.h>

#define COLOR gfy_RGBTo1555(34, 55, 89)

int main(void)
{
    uint8_t i = 0;

    /* Initialize graphics drawing */
    gfy_Begin();

    /* For i in 0..255 */
    do
    {
        /* Fade out to black */
        gfy_palette[0] = gfy_Darken(COLOR, i);
        gfy_SetColor(0);
        gfy_FillRectangle_NoClip(0, 0, 160, 120);

        /* Fade in from black */
        gfy_palette[1] = gfy_Darken(COLOR, ~i);
        gfy_SetColor(1);
        gfy_FillRectangle_NoClip(160, 0, 160, 120);

        /* Fade in from white */
        gfy_palette[2] = gfy_Lighten(COLOR, i);
        gfy_SetColor(2);
        gfy_FillRectangle_NoClip(0, 120, 160, 120);

        /* Fade out to white */
        gfy_palette[3] = gfy_Lighten(COLOR, ~i);
        gfy_SetColor(3);
        gfy_FillRectangle_NoClip(160, 120, 160, 120);

        /* Wait for a keypress at the start of each quarter of the fade */
        if (!(i & 0x3f))
        {
            while (!os_GetCSC());
        }

    /* Loop until i is 0 again because of 8 bit range */
    } while (++i);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
