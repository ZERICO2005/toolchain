#include <ti/getcsc.h>
#include <graphy.h>

/* Include the converted graphics file */
#include "gfx/vargfx.h"

/* Include the external tilemap data */
extern unsigned char tilemap_map[];

/* Tilemap defines */
#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define TILEMAP_WIDTH       32
#define TILEMAP_HEIGHT      25

#define TILEMAP_DRAW_WIDTH  20
#define TILEMAP_DRAW_HEIGHT 14

#define Y_OFFSET            16
#define X_OFFSET            0

int main(void)
{
    uint8_t key;
    unsigned int x_offset = 0;
    unsigned int y_offset = 0;
    gfy_tilemap_t tilemap;

    /* This function is automatically created by convimg */
    /* Simply call it to load in graphics from the appvar */
    /* The common function name is <appvar name>_init */
    /* This function returns 1 on success or 0 if the appvar failed to load. */
    if (vargfy_init() == 0)
    {
        return 1;
    }

    /* Initialize the tilemap structure */
    tilemap.map         = tilemap_map;
    tilemap.tiles       = tileset_tiles;
    tilemap.type_width  = gfy_tile_16_pixel;
    tilemap.type_height = gfy_tile_16_pixel;
    tilemap.tile_height = TILE_HEIGHT;
    tilemap.tile_width  = TILE_WIDTH;
    tilemap.draw_height = TILEMAP_DRAW_HEIGHT;
    tilemap.draw_width  = TILEMAP_DRAW_WIDTH;
    tilemap.height      = TILEMAP_HEIGHT;
    tilemap.width       = TILEMAP_WIDTH;
    tilemap.y_loc       = Y_OFFSET;
    tilemap.x_loc       = X_OFFSET;

    /* Initialize graphics drawing */
    gfy_Begin();

    /* Set the palette */
    gfy_SetPalette(global_palette, sizeof_global_palette, 0);
    gfy_SetColor(0);
    gfy_SetTextFGColor(1);
    gfy_SetTextBGColor(0);

    /* Draw to buffer to avoid tearing */
    gfy_SetDrawBuffer();

    /* Set monospace font with width of 8 */
    gfy_SetMonospaceFont(8);

    /* Wait for the enter key to quit */
    do
    {
        uint8_t block_mapped;
        uint8_t block_ptr;

        /* Get the key */
        key = os_GetCSC();

        /* Draw tilemap and coords */
        gfy_Tilemap(&tilemap, x_offset, y_offset);
        gfy_FillRectangle(0, 0, 320, 16);
        gfy_PrintStringXY("x:", 64, 4);
        gfy_PrintInt(x_offset, 3);
        gfy_PrintString(" y:");
        gfy_PrintInt(y_offset, 3);
        gfy_PrintString(" block:");

        /* Or use gfy_GetTileMapped() and gfy_GetTile() */
        block_mapped = *gfy_TilePtrMapped(&tilemap, x_offset / TILE_WIDTH, y_offset / TILE_HEIGHT);
        block_ptr = *gfy_TilePtr(&tilemap, x_offset, y_offset);

        gfy_PrintUInt(block_mapped, 3);
        gfy_PrintString("/");
        gfy_PrintUInt(block_ptr, 3);

        /* Do something based on the keypress */
        switch (key)
        {
            case sk_Down:
                if (y_offset < (TILEMAP_HEIGHT * TILE_HEIGHT) - (TILEMAP_DRAW_HEIGHT * TILE_HEIGHT))
                {
                    y_offset += TILE_HEIGHT;
                }
                break;

            case sk_Left:
                if (x_offset)
                {
                    x_offset -= TILE_WIDTH;
                }
                break;

            case sk_Right:
                if (x_offset < (TILEMAP_WIDTH * TILE_WIDTH) - (TILEMAP_DRAW_WIDTH * TILE_WIDTH))
                {
                    x_offset += TILE_WIDTH;
                }
                break;

            case sk_Up:
                if (y_offset)
                {
                    y_offset -= TILE_HEIGHT;
                }
                break;

            default:
                break;
        }

        gfy_SwapDraw();

    } while (key != sk_Enter);

    /* End graphics drawing */
    gfy_End();

    return 0;
}
