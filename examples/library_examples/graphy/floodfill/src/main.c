#include <math.h>
#include <ti/getcsc.h>
#include <graphy.h>

int main(void)
{
    int pts[10];
    int i;

    int rx = 100;
    int ry = 100;
    int cx = GFY_LCD_WIDTH / 2;
    int cy = GFY_LCD_HEIGHT / 2;

    /* Build the coordinates of the polygon */
    double theta = -M_PI / 2;
    double dtheta = 4 * M_PI / 5;
    for (i = 0; i < 10; i += 2)
    {
        pts[i+0] = (int)(cx + rx * cos(theta)),
        pts[i+1] = (int)(cy + ry * sin(theta));
        theta += dtheta;
    }

    /* Initialize graphics drawing */
    gfy_Begin();

    /* Set the outline color */
    gfy_SetColor(0);

    /* Draw a polygon */
    gfy_Polygon(pts, 5);

    /* Waits for a key */
    while (!os_GetCSC());

    /* Flood fill the inside of the polygon */
    gfy_FloodFill(GFY_LCD_WIDTH / 2, GFY_LCD_HEIGHT / 2, 229);

    /* Waits for a key */
    while (!os_GetCSC());

    /* Flood fill the outside of the polygon */
    gfy_FloodFill(0, 0, 9);

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
