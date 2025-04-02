#include <graphy.h>

void begin(void);
void end(void);
bool step(void);
void draw(void);

int main(void)
{
    bool partial_redraw = false;

    /* No rendering allowed! */
    begin();

    /* Initialize graphics drawing */
    gfy_Begin();

    /* Draw to the buffer to avoid rendering artifacts */
    gfy_SetDrawBuffer();

    /* No rendering allowed in step! */
    while (step())
    {
        /* Only want to redraw part of the previous frame? */
        if (partial_redraw)
        {
            /* Copy previous frame as a base for this frame */
            gfy_BlitScreen();
        }

        /* As little non-rendering logic as possible */
        draw();

        /* Queue the buffered frame to be displayed */
        gfy_SwapDraw();
    }

    /* End graphics drawing */
    gfy_End();
    end();

    return 0;
}

/* Implement me! */
void begin(void)
{

}

/* Implement me! */
void end(void)
{

}

/* Implement me! */
bool step(void)
{
    return false;
}

/* Implement me! */
void draw(void)
{

}
