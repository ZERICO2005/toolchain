#include "vargfx.h"
#include <fileioc.h>

#define vargfx_HEADER_SIZE 6

unsigned char *vargfx_appvar[2] =
{
    (unsigned char*)6,
    (unsigned char*)518,
};

unsigned char *image_tiles_compressed[12] =
{
    (unsigned char*)0,
    (unsigned char*)4045,
    (unsigned char*)8390,
    (unsigned char*)12896,
    (unsigned char*)16390,
    (unsigned char*)21906,
    (unsigned char*)26871,
    (unsigned char*)31842,
    (unsigned char*)37324,
    (unsigned char*)43071,
    (unsigned char*)47958,
    (unsigned char*)52338,
};

unsigned char vargfx_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("vargfx", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)vargfx_appvar[0] + vargfx_HEADER_SIZE;
    for (i = 0; i < 2; i++)
    {
        vargfx_appvar[i] += data;
    }

    ti_Close(appvar);

    data = (unsigned int)vargfx_appvar[1] - (unsigned int)image_tiles_compressed[0];
    for (i = 0; i < image_tiles_num; i++)
    {
        image_tiles_compressed[i] += data;
    }

    return 1;
}

