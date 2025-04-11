#include <ti/getcsc.h>
#include <sys/util.h>
#include <graphy.h>

int main(void)
{
    uint8_t y;
    uint24_t i;
    int count = 0;

    /* Initialize graphics drawing */
    gfy_Begin();

    /* Circle Drawing */
    y = 40;
    gfy_SetColor(18);
    for (i = 2; i < 13; i++)
    {
        y += i;
        gfy_FillCircle_NoClip(80, y, i);
        y += i + 3;
    }

    y = 40;
    gfy_SetColor(19);
    for (i = 2; i < 13; i++)
    {
        y += i;
        gfy_FillCircle(240, y, i);
        y += i + 3;
    }

    y = 40;
    gfy_SetColor(20);
    for (i = 2; i < 13; i++)
    {
        y += i;
        gfy_Circle(160, y, i);
        y += i + 3;
    }

    /* Waits for a key */
    while (!os_GetCSC());
    
    /* Clear the screen */
    gfy_FillScreen(255);
    
    /* Ellipse Drawing */
    gfy_SetColor(56);
    for (i = 10; i < 50; i += 2)
    {
        gfy_Ellipse_NoClip(100, 100, i, 40);
    }
    gfy_SetColor(195);
    gfy_Ellipse(300, 35, 140, 80);
    gfy_SetColor(5);
    gfy_FillEllipse_NoClip(200, 150, 120, 50);
    gfy_SetColor(210);
    gfy_FillEllipse(25, 200, 60, 90);
    
    /* Waits for a key */
    while (!os_GetCSC());

    /* Clear the screen */
    gfy_FillScreen(255);

    /* Rectangle Drawing */
    gfy_SetColor(227);
    gfy_Rectangle(20, 20, 280, 200);
    gfy_SetColor(18);
    gfy_Rectangle(-10, -10, 50, 50);
    gfy_Rectangle(280, -10, 50, 50);
    gfy_Rectangle(-10, 200, 50, 50);
    gfy_Rectangle(280, 200, 50, 50);
    gfy_SetColor(4);
    gfy_Rectangle_NoClip(30, 30, 260, 180);

    /* Waits for a key */
    while (!os_GetCSC());

    /* Clear the screen */
    gfy_FillScreen(255);

    /* Filled Rectangle Drawing */
    gfy_SetColor(227);
    gfy_FillRectangle(20, 20, 280, 200);
    gfy_SetColor(18);
    gfy_FillRectangle(-10, -10, 50, 50);
    gfy_FillRectangle(280, -10, 50, 50);
    gfy_FillRectangle(-10, 200, 50, 50);
    gfy_FillRectangle(280, 200, 50, 50);
    gfy_SetColor(4);
    gfy_FillRectangle(30, 30, 260, 180);

    /* Wait for any key */
    while (!os_GetCSC());

    /* Clear the screen */
    gfy_FillScreen(255);

    /* Triangle drawing */
    gfy_SetColor(18);
    gfy_FillTriangle(110, 170, 110, 70, 230, 70);
    gfy_SetColor(5);
    gfy_FillTriangle_NoClip(110, 170, 230, 70, 230, 170);
    gfy_SetColor(228);
    gfy_FillTriangle(110, 170, 200, 200, 230, 200);
    gfy_FillTriangle_NoClip(140, 40, 110, 40, 230, 70);
    gfy_FillTriangle(1, 1, 318, 1, 1, 1);
    gfy_FillTriangle_NoClip(1, 238, 1, 238, 318, 238);
    gfy_FillTriangle(1, 3, 1, 236, 1, 236);
    gfy_FillTriangle_NoClip(318, 3, 318, 236, 318, 3);

    /* Waits for a key */
    while (!os_GetCSC());

    /* Clear the screen */
    gfy_FillScreen(255);

    /* Line Drawing */
    gfy_SetColor(227);
    gfy_Line(-10, -10, 329, 249);
    gfy_Line(-10, 249, 329, -10);
    gfy_Line_NoClip(0, 0, 319, 0);
    gfy_Line_NoClip(0, 0, 0, 239);
    gfy_Line_NoClip(0, 239, 319, 239);
    gfy_Line_NoClip(319, 239, 319, 0);
    gfy_SetColor(18);
    gfy_HorizLine(-10, 10, 340);
    gfy_HorizLine_NoClip(0, 230, 320);
    gfy_VertLine(10, -10, 260);
    gfy_VertLine_NoClip(310, 0, 240);

    /* Waits for a key */
    while (!os_GetCSC());

    /* Clear the screen */
    gfy_FillScreen(255);

    /* Pixel Drawing */
    gfy_SetColor(227);
    for (i = 0; i < 2048; ++i)
    {
        int x = randInt(0, 319);
        int y = randInt(0, 239);
        uint8_t color = randInt(0, 255);

        gfy_SetColor(color);
        gfy_SetPixel(x, y);
        if (gfy_GetPixel(x, y) == color)
        {
            count++;

            gfy_SetColor(255);
            gfy_FillRectangle(0, 0, 50, 10);
            gfy_SetTextXY(0, 0);
            gfy_PrintInt(count, 5);
        }
    }

    /* Test character printing */
    gfy_SetTextXY(1, 232);
    gfy_PrintChar('D');
    gfy_PrintChar('O');
    gfy_PrintChar('N');
    gfy_PrintChar('E');

    /* Waits for a key */
    while (!os_GetCSC());

    /* End graphics drawing */
    gfy_End();

    return 0;
}
