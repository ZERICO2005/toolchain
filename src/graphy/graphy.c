#include "graphy.h"
#include <graphx.h>

// For memcpy and memset
#include <string.h>

/**
 * @brief Implementation of graphx for Column-Major mode.
 * 
 * @todo Implement graphx v1 functions.
*/

#ifdef __cplusplus
extern "C" 
#endif

/**
 * @file
 * @authors
 * @brief FTSSP010 SPI controller define file
 */

/* TEMPORARY SPI CODE */

    /* @cond */
    #define spi_ControlRegister0     ((volatile void*)0xF80000)
    #define spi_ControlRegister1     ((volatile void*)0xF80004)
    #define spi_ControlRegister2     ((volatile void*)0xF80008)
    #define spi_StatusBits           ((const volatile void*)0xF8000C)
    #define spi_InterruptControl     ((volatile void*)0xF80010)
    #define spi_InterruptStatus      ((const volatile void*)0xF80014)
    #define spi_FIFO                 ((volatile void*)0xF80018)
    #define spi_InsideReservedRange  ((volatile void*)0xF8001C)
    #define spi_Revision             ((const volatile void*)0xF80060)
    #define spi_Features             (*(const volatile uint32_t*)0xF80064)
    /* @endcond */

    /**
     * In order to reliably use the LCD interface, the
     * boot_InitializeHardware routine should be called at the start of a program
     * to select the LCD interface and reset its configuration to the default.
     */
    #define boot_InitializeHardware()  ((void(*)(void))0x384)();

    /**
     * Sends a Command to the SPI controller using the 9bit FIFO protocol.
     *
     * @param[in] x 8bit parameter.
     */
    #define SPI_COMMAND(x) \
    do { \
        *(volatile uint8_t*)spi_FIFO = ((x) >> 6) & 0b111; \
        *(volatile uint8_t*)spi_FIFO = ((x) >> 3) & 0b111; \
        *(volatile uint8_t*)spi_FIFO = (x) & 0b111; \
        *(volatile uint8_t*)spi_ControlRegister2 = 0x1; \
        while ( ((const volatile uint8_t*)spi_StatusBits)[1] & 0xF0) {}; \
        while ( ((const volatile uint8_t*)spi_StatusBits)[0] & (1 << 2)) {}; \
        *(volatile uint8_t*)spi_ControlRegister2 = 0x0; \
    } while(0)

    /**
     * Sends a Parameter to the SPI controller using the 9bit FIFO protocol.
     *
     * @param[in] x 8bit parameter.
     */
    #define SPI_PARAMETER(x) \
    do { \
        *(volatile uint8_t*)spi_FIFO = (((x) >> 6) & 0b111) | 0b100; \
        *(volatile uint8_t*)spi_FIFO = ((x) >> 3) & 0b111; \
        *(volatile uint8_t*)spi_FIFO = (x) & 0b111; \
        *(volatile uint8_t*)spi_ControlRegister2 = 0x1; \
        while ( ((const volatile uint8_t*)spi_StatusBits)[1] & 0xF0) {}; \
        while ( ((const volatile uint8_t*)spi_StatusBits)[0] & (1 << 2)) {}; \
        *(volatile uint8_t*)spi_ControlRegister2 = 0x0; \
    } while(0)

    /** @todo Implement vsync */
    #define SPI_UNINVERT_COLORS() SPI_COMMAND(0x20)

    /** @todo Implement vsync */
    #define SPI_INVERT_COLORS() SPI_COMMAND(0x21)

    /** Sets the LCD to BGR Row-Major mode (TI-OS default) */
    #define SPI_ROW_MAJOR() \
    do { \
        SPI_COMMAND(0x36); \
        SPI_PARAMETER(0b00001000); \
        SPI_COMMAND(0x2A); \
        SPI_PARAMETER(0x00); SPI_PARAMETER(0x00); \
        SPI_PARAMETER(0x01); SPI_PARAMETER(0x3F); \
        SPI_COMMAND(0x2B); \
        SPI_PARAMETER(0x00); SPI_PARAMETER(0x00); \
        SPI_PARAMETER(0x00); SPI_PARAMETER(0xEF); \
    } while(0)

    /** Sets the LCD to BGR Column-Major mode */
    #define SPI_COLUMN_MAJOR() \
    do { \
        SPI_COMMAND(0x36); \
        SPI_PARAMETER(0b00101000); \
        SPI_COMMAND(0x2A); \
        SPI_PARAMETER(0x00); SPI_PARAMETER(0x00); \
        SPI_PARAMETER(0x00); SPI_PARAMETER(0xEF); \
        SPI_COMMAND(0x2B); \
        SPI_PARAMETER(0x00); SPI_PARAMETER(0x00); \
        SPI_PARAMETER(0x01); SPI_PARAMETER(0x3F); \
    } while(0)

/**
 * @brief GraphY function implementation status
 * 
 * Labels:
 * Wrap     > Wraps a graphx function
 * Bad Wrap > Wraps a graphx function, but sets global variables outside of graphx.asm
 * Bad      > Doesn't updates values in graphx.asm
 * Verified > Function works and has been tested
 * Testing  > Function needs to be tested/verified
 * Partial  > Partial functionality
 * Minimal  > Function ignores parameters or safety checks
 * Broken   > Function does not work
 * Dummy    > Function does nothing or is incomplete
 * None     > Unimplemented
 * 
 * ------------
 * v1 functions
 * ------------
 * Partial  gfx_Begin
 * Partial  gfx_End
 * Bad Wrap gfx_SetColor
 * Wrap     gfx_SetDefaultPalette
 * Wrap     gfx_SetPalette
 * Wrap     gfx_FillScreen
 * Testing  gfx_SetPixel
 * Testing  gfx_GetPixel
 * Wrap     gfx_GetDraw
 * Wrap     gfx_SetDraw
 * Wrap     gfx_SwapDraw
 * Testing  gfx_Blit
 * Testing  gfx_BlitLines
 * Testing  gfx_BlitRectangle
 * Partial  gfx_PrintChar
 * Testing  gfx_PrintInt
 * Testing  gfx_PrintUInt
 * Testing  gfx_PrintString
 * Testing  gfx_PrintStringXY
 * Bad Wrap gfx_SetTextXY
 * Bad Wrap gfx_SetTextBGColor
 * Bad Wrap gfx_SetTextFGColor
 * Bad Wrap gfx_SetTextTransparentColor
 * Bad Wrap gfx_SetFontData
 * Bad Wrap gfx_SetFontSpacing
 * Bad Wrap gfx_SetMonospaceFont
 * Partial  gfx_GetStringWidth
 * Wrap     gfx_GetCharWidth
 * Partial  gfx_GetTextX
 * Partial  gfx_GetTextY
 * Minimal  gfx_Line
 * Minimal  gfx_HorizLine
 * Minimal  gfx_VertLine
 *          gfx_Circle
 *          gfx_FillCircle
 * Minimal  gfx_Rectangle
 * Partial  gfx_FillRectangle
 * Minimal  gfx_Line_NoClip
 * Partial  gfx_HorizLine_NoClip
 * Partial  gfx_VertLine_NoClip
 *          gfx_FillCircle_NoClip
 * Minimal  gfx_Rectangle_NoClip
 * Partial  gfx_FillRectangle_NoClip
 *          gfx_SetClipRegion
 *          gfx_GetClipRegion
 *          gfx_ShiftDown
 *          gfx_ShiftUp
 *          gfx_ShiftLeft
 *          gfx_ShiftRight
 *          gfx_Tilemap
 *          gfx_Tilemap_NoClip
 *          gfx_TransparentTilemap
 *          gfx_TransparentTilemap_NoClip
 *          gfx_TilePtr
 *          gfx_TilePtrMapped
 *          gfx_Reserved
 *          gfx_AllocSprite
 *          gfx_Sprite
 *          gfx_TransparentSprite
 *          gfx_Sprite_NoClip
 *          gfx_TransparentSprite_NoClip
 *          gfx_GetSprite
 *          gfx_ScaledSprite_NoClip
 *          gfx_ScaledTransparentSprite_NoClip
 *          gfx_FlipSpriteY
 *          gfx_FlipSpriteX
 *          gfx_RotateSpriteC
 *          gfx_RotateSpriteCC
 *          gfx_RotateSpriteHalf
 *          gfx_Polygon
 *          gfx_Polygon_NoClip
 *          gfx_FillTriangle
 *          gfx_FillTriangle_NoClip
 * ------------
 * v2 functions
 * ------------
 *          gfx_Deprecated
 * Bad Wrap gfx_SetTextScale
 * ------------
 * v3 functions
 * ------------
 * Bad Wrap gfx_SetTransparentColor
 * Wrap     gfx_ZeroScreen
 * Bad Wrap gfx_SetTextConfig
 *          gfx_GetSpriteChar
 * ------------
 * v4 functions
 * ------------
 * Wrap     gfx_Lighten
 * Wrap     gfx_Darken
 * ------------
 * v5 functions
 * ------------
 *          gfx_SetFontHeight
 *          gfx_ScaleSprite
 *          gfx_FloodFill
 * ------------
 * v6 functions
 * ------------
 *          gfx_RLETSprite
 *          gfx_RLETSprite_NoClip
 *          gfx_ConvertFromRLETSprite
 *          gfx_ConvertToRLETSprite
 *          gfx_ConvertToNewRLETSprite
 * ------------
 * v7 functions
 * ------------
 *          gfx_RotateScaleSprite
 *          gfx_RotatedScaledTransparentSprite_NoClip
 *          gfx_RotatedScaledSprite_NoClip
 * ------------
 * v8 functions
 * ------------
 * Bad Wrap gfx_SetCharData
 * ------------
 * v9 functions
 * ------------
 * Wrap     gfx_Wait
 * ------------
 * v10 functions
 * ------------

 * ------------
 * v11 functions
 * ------------
 *          gfx_CopyRectangle
 * ------------
 * v12 functions
 * ------------
 *          gfx_Ellipse
 *          gfx_Ellipse_NoClip
 *          gfx_FillEllipse
 *          gfx_FillEllipse_NoClip
*/

// TEMP DATA //
const uint8_t gfy_DefaultCharSpacing[] = {
    /* X    0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F*/
    /* 0 */ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    /* 1 */ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    /* 2 */ 3,4,6,8,8,8,8,5,5,5,8,7,4,7,3,8,
    /* 3 */ 8,7,8,8,8,8,8,8,8,8,3,4,6,7,6,7,
    /* 4 */ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    /* 5 */ 8,8,8,8,8,8,8,8,8,8,8,5,8,5,8,8,
    /* 6 */ 4,8,8,8,8,8,8,8,8,5,8,8,5,8,8,8,
    /* 7 */ 8,8,8,8,7,8,8,8,8,8,8,7,3,7,8,8,
    /* 8 */ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    /* 9 */ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};

const uint8_t gfy_DefaultTextData[] = {
/* 0x00   */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 0x01 ☺ */ 0x7E,0x81,0xA5,0x81,0xBD,0xBD,0x81,0x7E,
/* 0x02 ☻ */ 0x7E,0xFF,0xDB,0xFF,0xC3,0xC3,0xFF,0x7E,
/* 0x03 ♥ */ 0x6C,0xFE,0xFE,0xFE,0x7C,0x38,0x10,0x00,
/* 0x04 ♦ */ 0x10,0x38,0x7C,0xFE,0x7C,0x38,0x10,0x00,
/* 0x05 ♣ */ 0x38,0x7C,0x38,0xFE,0xFE,0x10,0x10,0x7C,
/* 0x06 ♠ */ 0x00,0x18,0x3C,0x7E,0xFF,0x7E,0x18,0x7E,
/* 0x07 • */ 0x00,0x00,0x18,0x3C,0x3C,0x18,0x00,0x00,
/* 0x08 ◘ */ 0xFF,0xFF,0xE7,0xC3,0xC3,0xE7,0xFF,0xFF,
/* 0x09 ○ */ 0x00,0x3C,0x66,0x42,0x42,0x66,0x3C,0x00,
/* 0x0A ◙ */ 0xFF,0xC3,0x99,0xBD,0xBD,0x99,0xC3,0xFF,
/* 0x0B ♂ */ 0x0F,0x07,0x0F,0x7D,0xCC,0xCC,0xCC,0x78,
/* 0x0C ♀ */ 0x3C,0x66,0x66,0x66,0x3C,0x18,0x7E,0x18,
/* 0x0D ♪ */ 0x3F,0x33,0x3F,0x30,0x30,0x70,0xF0,0xE0,
/* 0x0E ♫ */ 0x7F,0x63,0x7F,0x63,0x63,0x67,0xE6,0xC0,
/* 0x0F * */ 0x99,0x5A,0x3C,0xE7,0xE7,0x3C,0x5A,0x99,
/* 0x10 ► */ 0x80,0xE0,0xF8,0xFE,0xF8,0xE0,0x80,0x00,
/* 0x11 ◄ */ 0x02,0x0E,0x3E,0xFE,0x3E,0x0E,0x02,0x00,
/* 0x12 ↕ */ 0x18,0x3C,0x7E,0x18,0x18,0x7E,0x3C,0x18,
/* 0x13 ‼ */ 0x66,0x66,0x66,0x66,0x66,0x00,0x66,0x00,
/* 0x14 ¶ */ 0x7F,0xDB,0xDB,0x7B,0x1B,0x1B,0x1B,0x00,
/* 0x15 § */ 0x3F,0x60,0x7C,0x66,0x66,0x3E,0x06,0xFC,
/* 0x16 ▬ */ 0x00,0x00,0x00,0x00,0x7E,0x7E,0x7E,0x00,
/* 0x17 ↨ */ 0x18,0x3C,0x7E,0x18,0x7E,0x3C,0x18,0xFF,
/* 0x18 ↑ */ 0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x00,
/* 0x19 ↓ */ 0x18,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,
/* 0x1A → */ 0x00,0x18,0x0C,0xFE,0x0C,0x18,0x00,0x00,
/* 0x1B ← */ 0x00,0x30,0x60,0xFE,0x60,0x30,0x00,0x00,
/* 0x1C └ */ 0x00,0x00,0xC0,0xC0,0xC0,0xFE,0x00,0x00,
/* 0x1D ↔ */ 0x00,0x24,0x66,0xFF,0x66,0x24,0x00,0x00,
/* 0x1E ▲ */ 0x00,0x18,0x3C,0x7E,0xFF,0xFF,0x00,0x00,
/* 0x1F ▼ */ 0x00,0xFF,0xFF,0x7E,0x3C,0x18,0x00,0x00,
/* 0x20   */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 0x21 ! */ 0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0xC0,0x00,
/* 0x22 " */ 0xD8,0xD8,0xD8,0x00,0x00,0x00,0x00,0x00,
/* 0x23 # */ 0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00,
/* 0x24 $ */ 0x18,0x7E,0xC0,0x7C,0x06,0xFC,0x18,0x00,
/* 0x25 % */ 0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6,0x00,
/* 0x26 & */ 0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00,
/* 0x27 ' */ 0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,
/* 0x28 ( */ 0x30,0x60,0xC0,0xC0,0xC0,0x60,0x30,0x00,
/* 0x29 ) */ 0xC0,0x60,0x30,0x30,0x30,0x60,0xC0,0x00,
/* 0x2A * */ 0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,
/* 0x2B + */ 0x00,0x30,0x30,0xFC,0xFC,0x30,0x30,0x00,
/* 0x2C , */ 0x00,0x00,0x00,0x00,0x00,0x60,0x60,0xC0,
/* 0x2D - */ 0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,
/* 0x2E . */ 0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0x00,
/* 0x2F / */ 0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00,
/* 0x30 0 */ 0x7C,0xCE,0xDE,0xF6,0xE6,0xC6,0x7C,0x00,
/* 0x31 1 */ 0x30,0x70,0x30,0x30,0x30,0x30,0xFC,0x00,
/* 0x32 2 */ 0x7C,0xC6,0x06,0x7C,0xC0,0xC0,0xFE,0x00,
/* 0x33 3 */ 0xFC,0x06,0x06,0x3C,0x06,0x06,0xFC,0x00,
/* 0x34 4 */ 0x0C,0xCC,0xCC,0xCC,0xFE,0x0C,0x0C,0x00,
/* 0x35 5 */ 0xFE,0xC0,0xFC,0x06,0x06,0xC6,0x7C,0x00,
/* 0x36 6 */ 0x7C,0xC0,0xC0,0xFC,0xC6,0xC6,0x7C,0x00,
/* 0x37 7 */ 0xFE,0x06,0x06,0x0C,0x18,0x30,0x30,0x00,
/* 0x38 8 */ 0x7C,0xC6,0xC6,0x7C,0xC6,0xC6,0x7C,0x00,
/* 0x39 9 */ 0x7C,0xC6,0xC6,0x7E,0x06,0x06,0x7C,0x00,
/* 0x3A : */ 0x00,0xC0,0xC0,0x00,0x00,0xC0,0xC0,0x00,
/* 0x3B ; */ 0x00,0x60,0x60,0x00,0x00,0x60,0x60,0xC0,
/* 0x3C < */ 0x18,0x30,0x60,0xC0,0x60,0x30,0x18,0x00,
/* 0x3D = */ 0x00,0x00,0xFC,0x00,0xFC,0x00,0x00,0x00,
/* 0x3E > */ 0xC0,0x60,0x30,0x18,0x30,0x60,0xC0,0x00,
/* 0x3F ? */ 0x78,0xCC,0x18,0x30,0x30,0x00,0x30,0x00,
/* 0x40 @ */ 0x7C,0xC6,0xDE,0xDE,0xDE,0xC0,0x7E,0x00,
/* 0x41 A */ 0x38,0x6C,0xC6,0xC6,0xFE,0xC6,0xC6,0x00,
/* 0x42 B */ 0xFC,0xC6,0xC6,0xFC,0xC6,0xC6,0xFC,0x00,
/* 0x43 C */ 0x7C,0xC6,0xC0,0xC0,0xC0,0xC6,0x7C,0x00,
/* 0x44 D */ 0xF8,0xCC,0xC6,0xC6,0xC6,0xCC,0xF8,0x00,
/* 0x45 E */ 0xFE,0xC0,0xC0,0xF8,0xC0,0xC0,0xFE,0x00,
/* 0x46 F */ 0xFE,0xC0,0xC0,0xF8,0xC0,0xC0,0xC0,0x00,
/* 0x47 G */ 0x7C,0xC6,0xC0,0xC0,0xCE,0xC6,0x7C,0x00,
/* 0x48 H */ 0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0x00,
/* 0x49 I */ 0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,
/* 0x4A J */ 0x06,0x06,0x06,0x06,0x06,0xC6,0x7C,0x00,
/* 0x4B K */ 0xC6,0xCC,0xD8,0xF0,0xD8,0xCC,0xC6,0x00,
/* 0x4C L */ 0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFE,0x00,
/* 0x4D M */ 0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0x00,
/* 0x4E N */ 0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6,0x00,
/* 0x4F O */ 0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,
/* 0x50 P */ 0xFC,0xC6,0xC6,0xFC,0xC0,0xC0,0xC0,0x00,
/* 0x51 Q */ 0x7C,0xC6,0xC6,0xC6,0xD6,0xDE,0x7C,0x06,
/* 0x52 R */ 0xFC,0xC6,0xC6,0xFC,0xD8,0xCC,0xC6,0x00,
/* 0x53 S */ 0x7C,0xC6,0xC0,0x7C,0x06,0xC6,0x7C,0x00,
/* 0x54 T */ 0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0x00,
/* 0x55 U */ 0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xFE,0x00,
/* 0x56 V */ 0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x38,0x00,
/* 0x57 W */ 0xC6,0xC6,0xC6,0xC6,0xD6,0xFE,0x6C,0x00,
/* 0x58 X */ 0xC6,0xC6,0x6C,0x38,0x6C,0xC6,0xC6,0x00,
/* 0x59 Y */ 0xC6,0xC6,0xC6,0x7C,0x18,0x30,0xE0,0x00,
/* 0x5A Z */ 0xFE,0x06,0x0C,0x18,0x30,0x60,0xFE,0x00,
/* 0x5B [ */ 0xF0,0xC0,0xC0,0xC0,0xC0,0xC0,0xF0,0x00,
/* 0x5C \ */ 0xC0,0x60,0x30,0x18,0x0C,0x06,0x02,0x00,
/* 0x5D ] */ 0xF0,0x30,0x30,0x30,0x30,0x30,0xF0,0x00,
/* 0x5E ^ */ 0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,
/* 0x5F _ */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
/* 0x60 ` */ 0xC0,0xC0,0x60,0x00,0x00,0x00,0x00,0x00,
/* 0x61 a */ 0x00,0x00,0x7C,0x06,0x7E,0xC6,0x7E,0x00,
/* 0x62 b */ 0xC0,0xC0,0xC0,0xFC,0xC6,0xC6,0xFC,0x00,
/* 0x63 c */ 0x00,0x00,0x7C,0xC6,0xC0,0xC6,0x7C,0x00,
/* 0x64 d */ 0x06,0x06,0x06,0x7E,0xC6,0xC6,0x7E,0x00,
/* 0x65 e */ 0x00,0x00,0x7C,0xC6,0xFE,0xC0,0x7C,0x00,
/* 0x66 f */ 0x1C,0x36,0x30,0x78,0x30,0x30,0x78,0x00,
/* 0x67 g */ 0x00,0x00,0x7E,0xC6,0xC6,0x7E,0x06,0xFC,
/* 0x68 h */ 0xC0,0xC0,0xFC,0xC6,0xC6,0xC6,0xC6,0x00,
/* 0x69 i */ 0x60,0x00,0xE0,0x60,0x60,0x60,0xF0,0x00,
/* 0x6A j */ 0x06,0x00,0x06,0x06,0x06,0x06,0xC6,0x7C,
/* 0x6B k */ 0xC0,0xC0,0xCC,0xD8,0xF8,0xCC,0xC6,0x00,
/* 0x6C l */ 0xE0,0x60,0x60,0x60,0x60,0x60,0xF0,0x00,
/* 0x6D m */ 0x00,0x00,0xCC,0xFE,0xFE,0xD6,0xD6,0x00,
/* 0x6E n */ 0x00,0x00,0xFC,0xC6,0xC6,0xC6,0xC6,0x00,
/* 0x6F o */ 0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7C,0x00,
/* 0x70 p */ 0x00,0x00,0xFC,0xC6,0xC6,0xFC,0xC0,0xC0,
/* 0x71 q */ 0x00,0x00,0x7E,0xC6,0xC6,0x7E,0x06,0x06,
/* 0x72 r */ 0x00,0x00,0xFC,0xC6,0xC0,0xC0,0xC0,0x00,
/* 0x73 s */ 0x00,0x00,0x7E,0xC0,0x7C,0x06,0xFC,0x00,
/* 0x74 t */ 0x30,0x30,0xFC,0x30,0x30,0x30,0x1C,0x00,
/* 0x75 u */ 0x00,0x00,0xC6,0xC6,0xC6,0xC6,0x7E,0x00,
/* 0x76 v */ 0x00,0x00,0xC6,0xC6,0xC6,0x7C,0x38,0x00,
/* 0x77 w */ 0x00,0x00,0xC6,0xC6,0xD6,0xFE,0x6C,0x00,
/* 0x78 x */ 0x00,0x00,0xC6,0x6C,0x38,0x6C,0xC6,0x00,
/* 0x79 y */ 0x00,0x00,0xC6,0xC6,0xC6,0x7E,0x06,0xFC,
/* 0x7A z */ 0x00,0x00,0xFE,0x0C,0x38,0x60,0xFE,0x00,
/* 0x7B { */ 0x1C,0x30,0x30,0xE0,0x30,0x30,0x1C,0x00,
/* 0x7C | */ 0xC0,0xC0,0xC0,0x00,0xC0,0xC0,0xC0,0x00,
/* 0x7D } */ 0xE0,0x30,0x30,0x1C,0x30,0x30,0xE0,0x00,
/* 0x7E ~ */ 0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00,
/* 0x7F Δ */ 0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE,0x00
};
// TEMP DATA //

// TEMP GLOBALS //
static uint24_t gfy_CurrentBuffer = (uint24_t)gfy_vram;

static uint8_t gfy_Color = 0;
static uint8_t gfy_Transparent_Color = 0;

static uint8_t gfy_Text_FG_Color = 0;
static uint8_t gfy_Text_BG_Color = 255;
static uint8_t gfy_Text_TP_Color = 255;

static const uint8_t *gfy_CharSpacing = gfy_DefaultCharSpacing;
static const uint8_t *gfy_TextData = gfy_DefaultTextData;

static int gfy_TextXPos = 0;
static int gfy_TextYPos = 0;

static uint8_t gfy_TextWidthScale = 1;
static uint8_t gfy_TextHeightScale = 1;

static uint8_t gfy_PrintChar_Clip = gfy_text_noclip;

// TEMP GLOBALS //

// REPLACEMENT FUNCTIONS //
#define wait_quick()
// REPLACEMENT FUNCTIONS //

// MACROS //
#define lcd_Control              (*(volatile uint24_t*)0xE30018)
#define lcd_VideoMode            (*(volatile uint16_t*)0xE30018)
#define lcd_UpBase               (*(volatile uint24_t*)0xE30010)
#define lcd_BGR8bit 0x927
#define lcd_BGR16bit 0x92D
// MACROS //

//------------------------------------------------------------------------------
// v1 functions
//------------------------------------------------------------------------------

/* gfy_Begin */

void gfy_Begin() {
    gfx_Begin();
    boot_InitializeHardware();
    SPI_COLUMN_MAJOR();
    lcd_VideoMode = lcd_BGR8bit;
    gfy_CurrentBuffer = (uint24_t)gfy_vram;
    
    // Resetting temp globals
    gfy_Color = 0;
    gfy_Transparent_Color = 0;

    gfy_Text_FG_Color = 0;
    gfy_Text_BG_Color = 255;
    gfy_Text_TP_Color = 255;

    gfy_CharSpacing = gfy_DefaultCharSpacing;
    gfy_TextData = gfy_DefaultTextData;

    gfy_TextXPos = 0;
    gfy_TextYPos = 0;

    gfy_TextWidthScale = 1;
    gfy_TextHeightScale = 1;

    gfy_PrintChar_Clip = gfy_text_noclip;
}

/* gfy_End */

void gfy_End(void) {
    gfx_End();
    SPI_ROW_MAJOR();
    lcd_VideoMode = lcd_BGR16bit;
    lcd_UpBase = (uint24_t)gfy_vram;
}

/* gfy_SetColor */

uint8_t gfy_SetColor(uint8_t index) {
    gfy_Color = index;
    return gfx_SetColor(index);
}

/* gfy_SetDefaultPalette */

void gfy_SetDefaultPalette(gfy_mode_t mode) {
    gfx_SetDefaultPalette((gfx_mode_t)mode);
}

/* gfy_SetPalette */

void gfy_SetPalette(
    const void *palette,
    uint24_t size,
    uint8_t offset
) {
    gfx_SetPalette(palette, size, offset);
}

/* gfy_FillScreen */

void gfy_FillScreen(uint8_t index) {
    // Assuming this covers the exact same 320x240 area
    gfx_FillScreen(index);
}

/* gfy_SetPixel */

void gfy_SetPixel(uint24_t x, uint8_t y) {
    if (x < GFY_LCD_WIDTH && y < GFY_LCD_HEIGHT) {
        gfy_vram[(uint24_t)y + (x * GFY_LCD_HEIGHT)] = gfy_Color;
    }
}

/* gfy_GetPixel */

uint8_t gfy_GetPixel(uint24_t x, uint8_t y) {
    return gfy_vram[(uint24_t)y + (x * GFY_LCD_HEIGHT)];
}

/* gfy_GetDraw */

uint8_t gfy_GetDraw(void) {
    // This is what the assembly does
    // (0xD40000 >> 16) ^ (0xD52C00 >> 16) == 0xD4 ^ 0xD5
    // return (gfy_CurrentBuffer >> 16) ^ (uint24_t)(gfy_vram >> 16);
    return gfx_GetDraw();
}

/* gfy_SetDraw */

void gfy_SetDraw(uint8_t location) {
    // switch (location) {
    //     case gfy_screen:
    //         gfy_CurrentBuffer = (uint24_t)&lcd_UpBase;
    //         return;
    //     default:
    //     case gfy_buffer:
    //         if (lcd_UpBase == (uint24_t)gfy_vram) {
    //             gfy_CurrentBuffer = (uint24_t)gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
    //         } else {
    //             gfy_CurrentBuffer = (uint24_t)gfy_vram;
    //         }
    //         return;
    // }
    gfx_SetDraw(location);
}

/* gfy_SwapDraw */

void gfy_SwapDraw(void) {
    gfx_SwapDraw();
}

/* gfy_Blit */

void gfy_Blit(gfy_location_t src) {
    gfx_Blit((gfx_location_t)src);
}

/* gfy_BlitLines */

void gfy_BlitLines(gfy_location_t src, uint8_t y_loc, uint8_t num_lines) {
    const uint8_t *src_buf = gfy_vram;
    uint8_t *dst_buf = gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
    if (src) {
        src_buf = gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
        dst_buf = gfy_vram;
    }
    src_buf += y_loc;
    dst_buf += y_loc;
    for (uint24_t x_cord = 0; x_cord < GFY_LCD_HEIGHT; x_cord++) {
        memcpy(dst_buf, src_buf, num_lines);
        src_buf += GFY_LCD_HEIGHT;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* gfy_BlitRectangle */

void gfy_BlitRectangle(
    gfy_location_t src,
    uint24_t x,
    uint8_t y,
    uint24_t width,
    uint24_t height
) {
    const uint8_t *src_buf = gfy_vram;
    uint8_t *dst_buf = gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
    if (src) {
        src_buf = gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
        dst_buf = gfy_vram;
    }
    src_buf += y + (x * GFY_LCD_HEIGHT);
    dst_buf += y + (x * GFY_LCD_HEIGHT);
    for (uint24_t x_cord = 0; x_cord < width; x_cord++) {
        memcpy(dst_buf, src_buf, height);
        src_buf += GFY_LCD_HEIGHT;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* gfy_internal_PrintChar */

static void gfy_internal_PrintCharXY(const char c, int posX, int posY) {
    if (gfy_PrintChar_Clip == gfy_text_clip) {
        if (
            posX < 0 || posY < 0 ||
            posX >= GFY_LCD_WIDTH || posY >= GFY_LCD_HEIGHT ||
            posX + 8 * gfy_TextWidthScale > GFY_LCD_WIDTH ||
            posY + 8 * gfy_TextHeightScale > GFY_LCD_HEIGHT
        ) {
            return;
        }
    }
    const uint8_t *bitImage = gfy_TextData + 8 * (uint24_t)((unsigned char)c);
    uint8_t *fillLinePtr = (uint8_t*)(gfy_CurrentBuffer + (posY + (posX * GFY_LCD_HEIGHT)));
    uint8_t b = 0x1;
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t v = 0; v < gfy_TextHeightScale; v++) {
            uint8_t *fillPtr = fillLinePtr;
            for (uint8_t x = 0; x < 8; x++) {
                const uint8_t fillColor = *bitImage & b ? gfy_Text_FG_Color : gfy_Text_BG_Color;
                for (uint8_t u = 0; u < gfy_TextWidthScale; u++) {
                    *fillPtr = fillColor;
                    fillPtr++;
                }
                bitImage++;
            }
            fillLinePtr += GFY_LCD_HEIGHT;
            bitImage -= 8;
        }
        b <<= 1;
    }
}

static void gfy_internal_PrintCharX(const char c, int posX) {
    gfy_internal_PrintCharXY(c, posX, gfy_TextYPos);
}

/* gfy_PrintChar */

void gfy_PrintChar(const char c) {
    gfy_internal_PrintCharXY(c, gfy_TextXPos, gfy_TextYPos);
}

/* gfy_internal_PrintUInt */

static void gfy_internal_PrintUInt(unsigned int n, int posX, uint8_t length) {
    if (length == 0) {
        return;
    }
    length = (length > 8) ? 8 : length;
    uint8_t digit_count = 0;
    {
        int n_temp = n;
        while (n_temp != 0) {
            n_temp /= 10;
            digit_count++;
        }
    }
    for (uint8_t l = digit_count; l < length; l++) {
        gfy_internal_PrintCharX('0', posX);
        posX += gfy_GetCharWidth('0');
    }
    uint8_t digit;
    switch (digit_count) {
        case 8: // 10'000'000 <= n <= 16'777'215
            gfy_internal_PrintCharX('1', posX);
            posX += gfy_GetCharWidth('1');
        case 7:
            digit = ((n / 1000000) % 10) + '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 6:
            digit = ((n / 100000) % 10) + '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 5:
            digit = ((n / 10000) % 10) + '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 4:
            digit = ((n / 1000) % 10) + '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 3:
            digit = ((n / 100) % 10) + '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 2:
            digit = ((n / 10) % 10) + '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 1:
            digit = (n % 10) + '0';
            gfy_internal_PrintCharX(digit, posX);
        case 0:
        return;
    }
}

/* gfy_PrintInt */

void gfy_PrintInt(int n, uint8_t length) {
    if (n >= 0) {
        gfy_internal_PrintUInt(n, gfy_TextXPos, length);
        return;
    }
    gfy_PrintChar('-');
    gfy_internal_PrintUInt(n, gfy_TextXPos + gfy_GetCharWidth('-'), length);
}

/* gfy_PrintUInt */

void gfy_PrintUInt(unsigned int n, uint8_t length) {
    gfy_internal_PrintUInt(n, gfy_TextXPos, length);
}

/* gfy_PrintUInt */

void gfy_PrintHex(unsigned int n, uint8_t length) {
    if (length == 0) {
        return;
    }
    length = (length > 6) ? 6 : length;
    uint8_t digit_count = 0;
    {
        int n_temp = n;
        while (n_temp != 0) {
            n_temp >>= 4;
            digit_count++;
        }
    }
    int posX = gfy_TextXPos;
    for (uint8_t l = digit_count; l < length; l++) {
        gfy_internal_PrintCharX('0', posX);
        posX += gfy_GetCharWidth('0');
    }
    uint8_t digit;
    switch (digit_count) {
        case 6:
            digit = (n >> 20) & 0xF;
            digit += (digit >= 0xA) ? 'A' : '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 5:
            digit = (n >> 16) & 0xF;
            digit += (digit >= 0xA) ? 'A' : '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 4:
            digit = (n >> 12) & 0xF;
            digit += (digit >= 0xA) ? 'A' : '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 3:
            digit = (n >> 8) & 0xF;
            digit += (digit >= 0xA) ? 'A' : '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 2:
            digit = (n >> 4) & 0xF;
            digit += (digit >= 0xA) ? 'A' : '0';
            gfy_internal_PrintCharX(digit, posX);
            posX += gfy_GetCharWidth(digit);
        case 1:
            digit = n & 0xF;
            digit += (digit >= 0xA) ? 'A' : '0';
            gfy_internal_PrintCharX(digit, posX);
        case 0:
        return;
    }
}

/* gfy_PrintString */

void gfy_PrintString(const char *string) {
    int posX = gfy_TextXPos;
    while (*string != '\0') {
        gfy_internal_PrintCharX(*string, posX);
        posX += gfy_GetCharWidth(*string);
        string++;
    }
}

/* gfy_PrintStringXY */

void gfy_PrintStringXY(const char *string, int x, int y) {
    while (*string != '\0') {
        gfy_internal_PrintCharXY(*string, x, y);
        x += gfy_GetCharWidth(*string);
        string++;
    }
}

/* gfy_SetTextXY */

void gfy_SetTextXY(int x, int y) {
    gfy_TextXPos = x;
    gfy_TextYPos = y;
    gfx_SetTextXY(x, y);
}

/* gfy_SetTextBGColor */

uint8_t gfy_SetTextBGColor(uint8_t color) {
    gfy_Text_BG_Color = color;
    return gfx_SetTextBGColor(color);
}

/* gfy_SetTextFGColor */

uint8_t gfy_SetTextFGColor(uint8_t color) {
    gfy_Text_FG_Color = color;
    return gfx_SetTextFGColor(color);
}

/* gfy_SetTextTransparentColor */

uint8_t gfy_SetTextTransparentColor(uint8_t color) {
    gfy_Text_TP_Color = color;
    return gfx_SetTextTransparentColor(color);
}

/* gfy_SetFontData */

uint8_t *gfy_SetFontData(const uint8_t *data) {
    gfy_TextData = (data == NULL) ? gfy_DefaultTextData : data;
    return gfx_SetFontData(data);
}

/* gfy_SetFontSpacing */

void gfy_SetFontSpacing(const uint8_t *data) {
    gfy_CharSpacing = (data == NULL) ? gfy_DefaultCharSpacing : data;
    return gfx_SetFontSpacing(data);
}

/* gfy_SetMonospaceFont */

void gfy_SetMonospaceFont(uint8_t spacing) {
    gfx_SetMonospaceFont(spacing);
}

/* gfy_GetStringWidth */

unsigned int gfy_GetStringWidth(const char *string) {
    unsigned int len = 0;
    while (*string != '\0') {
        len += gfy_GetCharWidth(*string);
        string++;
    }
    return len;
}

/* gfy_GetCharWidth */

unsigned int gfy_GetCharWidth(const char c) {
    return gfx_GetCharWidth(c);
}

/* gfy_GetTextX */

int gfy_GetTextX(void) {
    return gfy_TextXPos;
}

/* gfy_GetTextY */

int gfy_GetTextY(void) {
    return gfy_TextYPos;
}

/* gfy_Line */

// Unoptimized routine
static void drawLine0(int x0, int y0, int x1, int y1) {
    int dX = x1 - x0;
    int dY = y1 - y0;
    int yI = 1;
    if (dY < 0) {
        yI = -1;
        dY = -dY;
    }
    int dD = 2 * dY - dX;
    int y = y0;
    for (int x = x0; x < x1; x++) {
        if (x >= 0 && x < GFY_LCD_WIDTH && y >= 0 && y < GFY_LCD_HEIGHT) {
            gfy_SetPixel(x, y);
        }
        if (dD > 0) {
            y += yI;
            dD += 2 * (dY - dX);
        } else {
            dD += 2 * dY;
        }
    }
}

// Unoptimized routine
static void drawLine1(int x0, int y0, int x1, int y1) {
    int dX = x1 - x0;
    int dY = y1 - y0;
    int xI = 1;
    if (dX < 0) {
        xI = -1;
        dX = -dX;
    }
    int dD = (2 * dX) - dY;
    int x = x0;

    for (int y = y0; y < y1; y++) {
        if (x >= 0 && x < GFY_LCD_WIDTH && y >= 0 && y < GFY_LCD_HEIGHT) {
            gfy_SetPixel(x, y);
        }
        if (dD > 0) {
            x = x + xI;
            dD = dD + (2 * (dX - dY));
        } else {
            dD = dD + 2 * dX;
        }
    }
}

void gfy_Line(int x0, int y0, int x1, int y1) {
    // Unoptimized routine
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            drawLine0(x1, y1, x0, y0);
        } else {
            drawLine0(x0, y0, x1, y1);
        }
    } else {
        if (y0 > y1) {
            drawLine1(x1, y1, x0, y0);
        } else {
            drawLine1(x0, y0, x1, y1);
        }
    }
}

/* gfy_HorizLine */

void gfy_HorizLine(int x, int y, int length) {
    // Unoptimized and incorrect routine
    if (
        x >= 0 && x < GFY_LCD_WIDTH &&
        y >= 0 && y < GFY_LCD_HEIGHT &&
        x + length >= 0 && x + length <= GFY_LCD_WIDTH
    ) {
        gfy_HorizLine_NoClip(x, y, length);
    }
}

/* gfy_VertLine */

void gfy_VertLine(int x, int y, int length) {
    // Unoptimized and incorrect routine
    if (
        x >= 0 && x < GFY_LCD_WIDTH &&
        y >= 0 && y < GFY_LCD_HEIGHT &&
        y + length >= 0 && y + length <= GFY_LCD_HEIGHT
    ) {
        gfy_VertLine_NoClip(x, y, length);
    }
}

/* gfy_Circle */



/* gfy_FillCircle */



/* gfy_Rectangle */

void gfy_Rectangle(int x, int y, int width, int height) {
    // Unoptimized and incorrect routine
    if (
        x >= 0 && x < GFY_LCD_WIDTH &&
        y >= 0 && y < GFY_LCD_HEIGHT &&
        x + width >= 0 && x + width <= GFY_LCD_WIDTH &&
        y + height >= 0 && y + height <= GFY_LCD_HEIGHT
    ) {
        return;
    }
    gfy_Rectangle(x, y, width, height);
}

/* gfy_FillRectangle */

void gfy_FillRectangle(int x, int y, int width, int height) {
    // Unoptimized and incorrect routine
    if (
        x >= 0 && x < GFY_LCD_WIDTH &&
        y >= 0 && y < GFY_LCD_HEIGHT &&
        x + width >= 0 && x + width <= GFY_LCD_WIDTH &&
        y + height >= 0 && y + height <= GFY_LCD_HEIGHT
    ) {
        return;
    }
    gfy_FillRectangle(x, y, width, height);
}

/* gfy_Line_NoClip */

void gfy_Line_NoClip(uint24_t x0, uint8_t y0, uint24_t x1, uint8_t y1) {
    // Unoptimized routine
    gfy_Line(x0, y0, x1, y1);
}

/* gfy_HorizLine_NoClip */

void gfy_HorizLine_NoClip(uint24_t x, uint8_t y, uint24_t length) { //x start, y postion, x length
    uint8_t *fill = (uint8_t*)(gfy_CurrentBuffer + (x * GFY_LCD_HEIGHT) + (uint24_t)y);
    for (uint8_t y_cord = 0; y_cord < length; y_cord++) {
        *fill = gfy_Color;
        fill += GFY_LCD_HEIGHT;
    }
}

/* gfy_VertLine_NoClip */

void gfy_VertLine_NoClip(uint24_t x, uint8_t y, uint24_t length) { //x postion, y start, y length
    uint8_t *fill = (uint8_t*)(gfy_CurrentBuffer + ((uint24_t)y + (x * GFY_LCD_HEIGHT)));
    memset(fill, gfy_Color, length);
}

/* gfy_FillCircle_NoClip */



/* gfy_Rectangle_NoClip */

void gfy_Rectangle_NoClip(uint24_t x, uint8_t y, uint24_t width, uint8_t height) {
    gfy_HorizLine_NoClip(x            , y             , width );
    gfy_HorizLine_NoClip(x            , y + height - 1, width );
    gfy_VertLine_NoClip (x            , y             , height);
    gfy_VertLine_NoClip (x + width - 1, y             , height);
}

/* gfy_FillRectangle_NoClip */

void gfy_FillRectangle_NoClip(uint24_t x, uint8_t y, uint24_t width, uint8_t height) {
    if (width == 0 || height == 0) {
        return;
    }
    uint8_t *fill = (uint8_t*)(gfy_CurrentBuffer + ((uint24_t)y + (x * GFY_LCD_HEIGHT)));
    for (uint24_t x_cord = 0; x_cord < width; x_cord++) {
        memset(fill,gfy_Color, height);
        fill += GFY_LCD_HEIGHT;
    }
}

/* gfy_SetClipRegion */



/* gfy_GetClipRegion */



/* gfy_ShiftDown */



/* gfy_ShiftUp */



/* gfy_ShiftLeft */



/* gfy_ShiftRight */



/* gfy_Tilemap */



/* gfy_Tilemap_NoClip */



/* gfy_TransparentTilemap */



/* gfy_TransparentTilemap_NoClip */



/* gfy_TilePtr */



/* gfy_TilePtrMapped */



/* gfy_Reserved */



/* gfy_AllocSprite */



/* gfy_Sprite */



/* gfy_TransparentSprite */



/* gfy_Sprite_NoClip */



/* gfy_TransparentSprite_NoClip */



/* gfy_GetSprite */



/* gfy_ScaledSprite_NoClip */



/* gfy_ScaledTransparentSprite_NoClip */



/* gfy_FlipSpriteY */



/* gfy_FlipSpriteX */



/* gfy_RotateSpriteC */



/* gfy_RotateSpriteCC */



/* gfy_RotateSpriteHalf */



/* gfy_Polygon */



/* gfy_Polygon_NoClip */



/* gfy_FillTriangle */



/* gfy_FillTriangle_NoClip */



//------------------------------------------------------------------------------
// v2 functions
//------------------------------------------------------------------------------

/* gfy_Deprecated */



/* gfy_SetTextScale */

void gfy_SetTextScale(uint8_t width_scale, uint8_t height_scale) {
    gfy_TextWidthScale = width_scale;
    gfy_TextHeightScale = height_scale;
    gfx_SetTextScale(width_scale, height_scale);
}

//------------------------------------------------------------------------------
// v3 functions
//------------------------------------------------------------------------------

/* gfy_SetTransparentColor */

uint8_t gfy_SetTransparentColor(uint8_t index) {
    gfy_Color = index;
    return gfx_SetTransparentColor(index);
}

/* gfy_ZeroScreen */

void gfy_ZeroScreen(void) {
    gfx_ZeroScreen();
}

/* gfy_SetTextConfig */

void gfy_SetTextConfig(uint8_t config) {
    gfy_PrintChar_Clip = config;
    gfx_SetTextConfig(config);
}

/* gfy_GetSpriteChar */



//------------------------------------------------------------------------------
// v4 functions
//------------------------------------------------------------------------------

/* gfy_Lighten */

uint16_t gfy_Lighten(uint16_t color, uint8_t amount) {
    return gfx_Lighten(color, amount);
}

/* gfy_Darken */

uint16_t gfy_Darken(uint16_t color, uint8_t amount) {
    return gfx_Darken(color, amount);
}

//------------------------------------------------------------------------------
// v5 functions
//------------------------------------------------------------------------------

/* gfy_SetFontHeight */



/* gfy_ScaleSprite */



/* gfy_FloodFill */



//------------------------------------------------------------------------------
// v6 functions
//------------------------------------------------------------------------------

/* gfy_RLETSprite */



/* gfy_RLETSprite_NoClip */



/* gfy_ConvertFromRLETSprite */



/* gfy_ConvertToRLETSprite */



/* gfy_ConvertToNewRLETSprite */



//------------------------------------------------------------------------------
// v7 functions
//------------------------------------------------------------------------------

/* gfy_RotateScaleSprite */



/* gfy_RotatedScaledTransparentSprite_NoClip */



/* gfy_RotatedScaledSprite_NoClip */



//------------------------------------------------------------------------------
// v8 functions
//------------------------------------------------------------------------------

/* gfy_SetCharData */

uint8_t *gfy_SetCharData(uint8_t index, const uint8_t *data) {
    return gfx_SetCharData(index, data);
}

//------------------------------------------------------------------------------
// v9 functions
//------------------------------------------------------------------------------

/* gfy_Wait */

void gfy_Wait(void) {
    gfx_Wait();
}

//------------------------------------------------------------------------------
// v10 functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// v11 functions
//------------------------------------------------------------------------------

/* gfy_CopyRectangle */



//------------------------------------------------------------------------------
// v12 functions
//------------------------------------------------------------------------------

/* gfy_Ellipse */



/* gfy_Ellipse_NoClip */



/* gfy_FillEllipse */



/* gfy_FillEllipse_NoClip */



#ifdef __cplusplus

#endif
