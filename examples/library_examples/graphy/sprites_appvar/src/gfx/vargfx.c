#include "vargfx.h"
#include <fileioc.h>

#define vargfy_HEADER_SIZE 0

unsigned char *vargfy_appvar[3] =
{
    (unsigned char*)0,
    (unsigned char*)494,
    (unsigned char*)1520,
};

unsigned char vargfy_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("vargfx", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)vargfy_appvar[0] + vargfy_HEADER_SIZE;
    for (i = 0; i < 3; i++)
    {
        vargfy_appvar[i] += data;
    }

    ti_Close(appvar);

    return 1;
}

