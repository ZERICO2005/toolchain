/**
 * @file
 * @brief Implementation of graphx for Column-Major mode.
 * @author "zerico2005"
 */

/**
 * Macro configuration: 
 *
 * GRAPHY_STANDALONE:
 * GraphY will be independent and will not use any GraphX code.
 *
 * GRAPHY_LCDDRVCE:
 * GraphY will use lcddrvce to send SPI commands to enable/disable Column-Major
 * mode on the LCD. Otherwise, generic code will be used to send the SPI
 * commands, which may or may not work on all Ti84CE models.
 *
 * GRAPHY_SHARE_SYMBOLS:
 * GraphY will reuse symbols from `graphx.asm` to cut down on binary size.
 *
 */

#define GRAPHY_STANDALONE
#define GRAPHY_LCDDRVCE
#define GRAPHY_SHARE_SYMBOLS

#if defined(GRAPHY_STANDALONE) && defined(GRAPHY_SHARE_SYMBOLS)
    #error "GRAPHY_STANDALONE and GRAPHY_SHARE_SYMBOLS conflict"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef GRAPHY_STANDALONE
    #include <graphx.h>
#endif

#include "graphy.h"

/**
 * @brief (Out of date) examples/library_examples/graphx status
 * 
 * background           > 
 * hello_world          > 
 * sprites              > Passed - Casts to (gfy_sprite_t*)
 * sprites_rotate_flip  > Passed - Casts to (gfy_sprite_t*)
 * text_flipped         > 
 * background_appvar    > 
 * lighten_darken       > 
 * sprites_appvar       > 
 * sprites_rotate_scale > 
 * tilemap              > Okay - Runs slow - Casts to (gfy_sprite_t*)
 * blitting             > Passed
 * scaled_text          > Okay - Text transparency unimplemented
 * sprites_compress     > 
 * sprites_scaled       > Passed - Took ~4 frames in CEmu - Casts to (gfy_sprite_t*)
 * tilemap_appvar       > 
 * buffered_cube        > Passed - Line algorithm is slow
 * screen_shift         > Passed
 * sprites_moving       > Passed - Sprite clipping is too extreme - Casts to (gfy_sprite_t*)
 * text_clipped         > Okay - Text clipping is too extreme - Text transparency unimplemented
 * tilemap_compressed   > 
 * floodfill            > 
 * shapes               > Bad - Incorrect Clipping - Missing Shapes
 * sprites_rlet         > Passed - Could be faster
 * text_custom          > 
 */

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------

#ifdef GRAPHY_LCDDRVCE
    #include <lcddrvce.h>
#else

    /* @cond */
    #define spi_ControlRegister0     ((volatile void*)RAM_ADDRESS(0xF80000))
    #define spi_ControlRegister1     ((volatile void*)RAM_ADDRESS(0xF80004))
    #define spi_ControlRegister2     ((volatile void*)RAM_ADDRESS(0xF80008))
    #define spi_StatusBits           ((const volatile void*)RAM_ADDRESS(0xF8000C))
    #define spi_InterruptControl     ((volatile void*)RAM_ADDRESS(0xF80010))
    #define spi_InterruptStatus      ((const volatile void*)RAM_ADDRESS(0xF80014))
    #define spi_FIFO                 ((volatile void*)RAM_ADDRESS(0xF80018))
    #define spi_InsideReservedRange  ((volatile void*)RAM_ADDRESS(0xF8001C))
    #define spi_Revision             ((const volatile void*)RAM_ADDRESS(0xF80060))
    #define spi_Features             (*(const volatile uint32_t*)RAM_ADDRESS(0xF80064))
    /* @endcond */

    /**
     * In order to reliably use the LCD interface, the
     * boot_InitializeHardware routine should be called at the start of a program
     * to select the LCD interface and reset its configuration to the default.
     */
    #define boot_InitializeHardware()  ((void(*)(void))0x000384)();
    
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

#endif

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------

#ifdef GRAPHY_SHARE_SYMBOLS

extern const uint8_t _DefaultCharSpacing[160];
extern const uint8_t _DefaultTextData[1024];
extern const uint8_t *_CharSpacing;
extern const uint8_t *_TextData;
#define gfy_DefaultCharSpacing _DefaultCharSpacing
#define gfy_DefaultTextData    _DefaultTextData
#define gfy_CharSpacing        _CharSpacing
#define gfy_TextData           _TextData

extern int24_t _TextXPos;
extern int24_t _TextYPos;
extern uint8_t _TextWidthScale;
extern uint8_t _TextHeightScale;
extern uint8_t _PrintChar_Clip;
extern uint8_t _FontHeight;
extern uint8_t _TextFixedWidth;
#define gfy_TextXPos        _TextXPos
#define gfy_TextYPos        _TextYPos
#define gfy_TextWidthScale  _TextWidthScale
#define gfy_TextHeightScale _TextHeightScale
#define gfy_PrintChar_Clip  _PrintChar_Clip
#define gfy_FontHeight      _FontHeight
#define gfy_MonospaceFont   _TextFixedWidth

extern uint8_t _TmpCharSprite[sizeof(gfy_sprite_t) + (8 * 8)];
#define gfy_TmpCharSprite (*(gfy_sprite_t*)_TmpCharSprite)

extern uint8_t _Color;
extern uint8_t TRANSPARENT_COLOR;
extern uint8_t TEXT_FG_COLOR;
extern uint8_t TEXT_BG_COLOR;
extern uint8_t TEXT_TP_COLOR;
#define gfy_Color             _Color
#define gfy_Transparent_Color TRANSPARENT_COLOR
#define gfy_Text_FG_Color     TEXT_FG_COLOR
#define gfy_Text_BG_Color     TEXT_BG_COLOR
#define gfy_Text_TP_Color     TEXT_TP_COLOR

extern int24_t _ClipXMin;
extern int24_t _ClipYMin;
extern int24_t _ClipXMax;
extern int24_t _ClipYMax;
#define gfy_ClipXMin _ClipXMin
#define gfy_ClipYMin _ClipYMin
#define gfy_ClipXMax _ClipXMax
#define gfy_ClipYMax _ClipYMax

#else

static const uint8_t gfy_DefaultCharSpacing[160] = {
    /* X    0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F */
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

static const uint8_t gfy_DefaultTextData[1024] = {
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

static const uint8_t *gfy_CharSpacing = gfy_DefaultCharSpacing;
static const uint8_t *gfy_TextData = gfy_DefaultTextData;

static int24_t gfy_TextXPos = 0;
static int24_t gfy_TextYPos = 0;
static uint8_t gfy_TextWidthScale = 1;
static uint8_t gfy_TextHeightScale = 1;
static uint8_t gfy_PrintChar_Clip = gfy_text_noclip;
static uint8_t gfy_FontHeight = GFY_MAXIMUM_FONT_HEIGHT;
static uint8_t gfy_MonospaceFont = 0;

static uint8_t gfy_memory_TmpCharSprite[sizeof(gfy_sprite_t) + (8 * 8)] = {8, 8};
#define gfy_TmpCharSprite (*(gfy_sprite_t*)gfy_memory_TmpCharSprite)

static uint8_t gfy_Color = 0;
static uint8_t gfy_Transparent_Color = 0;
static uint8_t gfy_Text_FG_Color = 0;
static uint8_t gfy_Text_BG_Color = 255;
static uint8_t gfy_Text_TP_Color = 255;

static int24_t gfy_ClipXMin = 0;
static int24_t gfy_ClipYMin = 0;
static int24_t gfy_ClipXMax = GFY_LCD_WIDTH;
static int24_t gfy_ClipYMax = GFY_LCD_HEIGHT;

#endif

//------------------------------------------------------------------------------
// macros
//------------------------------------------------------------------------------

#define lcd_Control              (*(volatile uint24_t*)0xE30018)
#define lcd_VideoMode            (*(volatile uint16_t*)0xE30018)
#define lcd_UpBase               (*(volatile uint24_t*)0xE30010)
#define lcd_LpBase               (*(volatile uint24_t*)0xE30014)
#define lcd_BGR8bit 0x927
#define lcd_BGR16bit 0x92D
#define gfy_CurrentBuffer lcd_LpBase
#define GFY_MAXIMUM_FONT_WIDTH (8)
#define GFY_MAXIMUM_FONT_HEIGHT (8)

#define ti_boot_ClearVRAM()  ((void(*)(void))0x000374)();

#define RAM_ADDRESS(x) ((void*)(x))	
#define RAM_OFFSET(ptr) ((uint24_t)(ptr))

#define GFY_CLIPPING_ASSUMPTIONS()

//------------------------------------------------------------------------------
// column major routines
//------------------------------------------------------------------------------

#ifdef GRAPHY_LCDDRVCE

static void gfy_SetupColumnMajor(void) {
    lcd_Init();
    lcd_SetColumnMajor(true);
}

static void gfy_RestoreRowMajor(void) {
    lcd_SetColumnMajor(false);
    lcd_Cleanup();
}

#else 

static void gfy_SetupColumnMajor(void) {
    boot_InitializeHardware();
    SPI_COLUMN_MAJOR();
}

static void gfy_RestoreRowMajor(void) {
    SPI_ROW_MAJOR();
}

#endif

//------------------------------------------------------------------------------
// internal routines
//------------------------------------------------------------------------------

static const int8_t gfy_SinTable[65] = {
    0  ,3  ,6  ,9  ,  13 ,16 ,19 ,22 ,  25 ,28 ,31 ,34 ,  37 ,40 ,43 ,46 ,
    49 ,52 ,55 ,58 ,  60 ,63 ,66 ,68 ,  71 ,74 ,76 ,79 ,  81 ,84 ,86 ,88 ,
    91 ,93 ,95 ,97 ,  99 ,101,103,105,  106,108,110,111,  113,114,116,117,
    118,119,121,122,  122,123,124,125,  126,126,127,127,  127,127,127,127,
    127
};

/**
 * @brief Calculates sin(x) from a lookup table.
 * 
 * @param theta angle 0-255
 * @return sin(x) * 128
 */
__attribute__((unused)) static uint8_t gfy_Sin(uint8_t theta) {
    if (theta < 128) {
        if (theta < 64) {
            return gfy_SinTable[theta]; // 0-63
        }
        return gfy_SinTable[128 - theta]; // 64-127
    }
    if (theta < 192) {
        return -gfy_SinTable[theta - 128]; // 128-191
    }
    return -gfy_SinTable[256 - theta]; // 192-255
}

/**
 * @brief Calculates cos(x) from a lookup table.
 * 
 * @param theta angle 0-255
 * @return cos(x) * 128
 */
__attribute__((unused)) static uint8_t gfy_Cos(uint8_t theta) {
    return gfy_Sin(theta + 64);
}

//------------------------------------------------------------------------------
// v1 functions
//------------------------------------------------------------------------------

/* gfy_Begin */
void gfy_Begin(void) {
    gfy_SetupColumnMajor();

    #ifdef GRAPHY_STANDALONE
         lcd_VideoMode = lcd_BGR8bit;
         memset(gfy_vram, 0xFF, sizeof(uint16_t) * GFY_LCD_WIDTH * GFY_LCD_HEIGHT);
    #else
        gfx_Begin();
    #endif

    #ifndef GRAPHY_SHARE_SYMBOLS
        // Resetting globals
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
        gfy_FontHeight = GFY_MAXIMUM_FONT_HEIGHT;

        gfy_ClipXMin = 0;
        gfy_ClipYMin = 0;
        gfy_ClipXMax = GFY_LCD_WIDTH;
        gfy_ClipYMax = GFY_LCD_HEIGHT;
        gfy_MonospaceFont = 0;
    #endif
}

/* gfy_End */

void gfy_End(void) {
    lcd_SetColumnMajor(false);

    #ifdef GRAPHY_STANDALONE
        lcd_VideoMode = lcd_BGR16bit;
        lcd_UpBase = RAM_OFFSET(gfy_vram);
        memset(gfy_vram, 0xFF, sizeof(uint16_t) * GFY_LCD_WIDTH * GFY_LCD_HEIGHT);
    #else
        gfx_End();
    #endif
   
}

/* gfy_SetColor */

#ifndef GRAPHY_SHARE_SYMBOLS
uint8_t gfy_SetColor(uint8_t index) {
    const uint8_t prev_Color = gfy_Color;
    gfy_Color = index;
    return prev_Color;
}
#else
uint8_t gfy_SetColor(uint8_t) __attribute__((alias("gfx_SetColor")));
#endif

/* gfy_SetDefaultPalette */

#if 0
    __attribute__((unused)) static const uint16_t gfy_internal_default_palette[256] = {

    };
#endif


void gfy_SetDefaultPalette(__attribute__((unused)) gfy_mode_t mode) {
    #ifndef GRAPHY_STANDALONE
        gfx_SetDefaultPalette(mode);
    #else
        gfx_SetDefaultPalette((gfx_mode_t)mode);
        /* RGB332? */
            // uint8_t i = 0;
            // for (uint8_t r = 0; r < 8; r++) {
            //     for (uint8_t b = 0; b < 4; b++) {
            //         for (uint8_t g = 0; g < 8; g++) {
            //             gfy_palette[i] = (uint16_t)((r << 12) | (g << 7) | (b << 3));
            //             i++;
            //         }
            //     }
            // }

        /* Fails to handle rrca, rla, and rra. Doesn't handle flags */
            uint8_t* palette_ptr = (uint8_t*)(void*)gfy_palette;
            uint8_t b = 0;
            for (size_t i = 0; i < 256; i++) {
                //uint8_t carry = 0;
                uint8_t a = b;
                
                a = (a >> 1) | ((a & 1) << 7); // rrca
                a ^= b;
                a &= 224;
                a ^= b;

                *palette_ptr = a;
                palette_ptr++;

                a = b;
                a <<= 1; // rla
                a <<= 1; // rla
                a <<= 1; // rla
                a = b;
                a >>= 1; // rra

                *palette_ptr = a;
                palette_ptr++;

                b++;
            }
        
        /* Data */
            //memcpy(gfy_palette, gfy_internal_default_palette, sizeof(uint16_t) * 256);
    #endif
}

/* gfy_SetPalette */

#ifdef GRAPHY_STANDALONE
void gfy_SetPalette(const void *palette, uint24_t size, uint8_t offset) {
    if ((2 * offset) + size > 512) {
        size = 512 - (2 * offset);
    }
    memcpy(&gfy_palette[offset], palette, size);
}
#else
void gfy_SetPalette(const void*, uint24_t, uint8_t) __attribute__((alias("gfx_SetPalette")));
#endif

/* gfy_FillScreen */

#ifdef GRAPHY_STANDALONE
void gfy_FillScreen(uint8_t index) {
    memset(RAM_ADDRESS(gfy_CurrentBuffer), index, GFY_LCD_WIDTH * GFY_LCD_HEIGHT);
    gfy_Wait();
}
#else
// Assuming this covers the exact same 320x240 area
void gfy_FillScreen(uint8_t) __attribute__((alias("gfx_FillScreen")));
#endif

/* gfy_SetPixel */

void gfy_SetPixel(uint24_t x, uint8_t y) {
    if (x < GFY_LCD_WIDTH && y < GFY_LCD_HEIGHT) {
        ((uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer))[(uint24_t)y + (x * GFY_LCD_HEIGHT)] = gfy_Color;
    }
}

// Macro to write a pixel without clipping
#define gfy_SetPixel_NoClip(x, y, color) \
    ((uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer))[(y) + ((x) * GFY_LCD_HEIGHT)] = (color)

// Macro to write a pixel inside the screen bounds
#define gfy_SetPixel_ScreenClip(x, y, color); \
    if ((x) < GFY_LCD_WIDTH && (y) < GFY_LCD_HEIGHT) { \
        ((uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer))[(y) + ((x) * GFY_LCD_HEIGHT)] = (color); \
    }

// Macro to write a pixel inside the clipping region
#define gfy_SetPixel_RegionClip(x, y, color) \
    if ( \
        (int24_t)(x) >= gfy_ClipXMin && (int24_t)(x) < gfy_ClipXMax && \
        (int24_t)(y) >= gfy_ClipYMin && (int24_t)(y) < gfy_ClipYMax \
    ) { \
        ((uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer))[(y) + ((x) * GFY_LCD_HEIGHT)] = (color); \
    }

/* gfy_GetPixel */

uint8_t gfy_GetPixel(uint24_t x, uint8_t y) {
    return ((const uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer))[(uint24_t)y + ((x & 0xFFFF) * GFY_LCD_HEIGHT)];
}

/* gfy_GetDraw */

#ifdef GRAPHY_STANDALONE
uint8_t gfy_GetDraw(void) {
    // This is what the assembly does
    // (0xD40000 >> 16) ^ (0xD52C00 >> 16) == 0xD4 ^ 0xD5
    return ((gfy_CurrentBuffer >> 16) ^ (RAM_OFFSET(gfy_vram) >> 16)) ? 1 : 0;
}
#else
uint8_t gfy_GetDraw(void) __attribute__((alias("gfx_GetDraw")));
#endif

/* gfy_SetDraw */

#ifdef GRAPHY_STANDALONE
void gfy_SetDraw(uint8_t location) {
    #ifdef _EZ80
        gfx_SetDraw(location);
    #else
        switch (location) {
            case gfy_screen:
                gfy_CurrentBuffer = lcd_UpBase;
                return;
            default:
            case gfy_buffer:
                if (lcd_UpBase == RAM_OFFSET(gfy_vram)) {
                    gfy_CurrentBuffer = RAM_OFFSET(gfy_vram) + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
                } else {
                    gfy_CurrentBuffer = RAM_OFFSET(gfy_vram);
                }
                return;
        }
    #endif
}
#else
void gfy_SetDraw(uint8_t) __attribute__((alias("gfx_SetDraw")));
#endif

/* gfy_SwapDraw */

#ifdef GRAPHY_STANDALONE
void gfy_SwapDraw(void) {
    #ifdef _EZ80
        gfx_SwapDraw();
    #else
        const uint24_t temp = lcd_UpBase;
        lcd_UpBase = gfy_CurrentBuffer;
        gfy_CurrentBuffer = temp;
        gfy_Wait();
    #endif
}
#else
void gfy_Blit(gfy_location_t) __attribute__((alias("gfx_Blit")));
#endif

/* gfy_Blit */

void gfy_Blit(gfy_location_t src) {
    #ifndef GRAPHY_STANDALONE
        gfx_Blit((gfx_location_t)src);
    #else
        const uint8_t *src_buf = gfy_vram;
        uint8_t *dst_buf = gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
        if (src) {
            src_buf = gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
            dst_buf = gfy_vram;
        }
        memcpy(dst_buf, src_buf, GFY_LCD_WIDTH * GFY_LCD_HEIGHT);
        gfy_Wait();
    #endif
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
    for (uint24_t x_cord = 0; x_cord < GFY_LCD_WIDTH; x_cord++) {
        memcpy(dst_buf, src_buf, num_lines);
        src_buf += GFY_LCD_HEIGHT;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* gfy_BlitLines */

void gfy_BlitColumns(gfy_location_t src, uint24_t x_loc, uint24_t num_columns) {
    const uint8_t *src_buf = gfy_vram;
    uint8_t *dst_buf = gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
    if (src) {
        src_buf = gfy_vram + (GFY_LCD_HEIGHT * GFY_LCD_WIDTH);
        dst_buf = gfy_vram;
    }
    src_buf += x_loc * GFY_LCD_HEIGHT;
    dst_buf += x_loc * GFY_LCD_HEIGHT;
    memcpy(dst_buf, src_buf, num_columns * GFY_LCD_HEIGHT);
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

/* gfy_internal_PrintCharXY_NoClip */

static void gfy_internal_PrintChar_NoClip(const char c, const uint8_t charWidth) {
    const uint8_t *bitImage = gfy_TextData + GFY_MAXIMUM_FONT_HEIGHT * (uint24_t)((unsigned char)c);
    uint8_t *fillLinePtr = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (gfy_TextYPos + (gfy_TextXPos * GFY_LCD_HEIGHT));
    uint8_t b = (1 << 7);
    gfy_TextXPos += charWidth * gfy_TextWidthScale;
    for (uint8_t x = 0; x < charWidth; x++) {
        for (uint8_t u = 0; u < gfy_TextWidthScale; u++) {
            uint8_t *fillPtr = fillLinePtr;
            for (uint8_t y = 0; y < gfy_FontHeight; y++) {
                const uint8_t fillColor = *bitImage & b ? gfy_Text_FG_Color : gfy_Text_BG_Color;
                bitImage++;
                if (fillColor == gfy_Text_TP_Color) {
                    
                    fillPtr += gfy_TextHeightScale;
                    continue;
                }
                for (uint8_t v = 0; v < gfy_TextHeightScale; v++) {
                    *fillPtr = fillColor;
                    fillPtr++;
                }
            }
            fillLinePtr += GFY_LCD_HEIGHT;
            bitImage -= gfy_FontHeight;
        }
        b >>= 1;
    }
}

/* gfy_PrintChar */

void gfy_PrintChar(const char c) {
    GFY_CLIPPING_ASSUMPTIONS();
    
    const uint8_t charWidth = gfy_GetCharWidth(c);
    const uint8_t textSizeX = charWidth * gfy_TextWidthScale;
    const uint8_t textSizeY = GFY_MAXIMUM_FONT_HEIGHT * gfy_TextHeightScale;
    if (
        gfy_PrintChar_Clip == gfy_text_noclip ||
        /* Otherwise, if clipping is enabled */
        gfy_TextXPos >= gfy_ClipXMin || gfy_TextYPos >= gfy_ClipYMin ||
        gfy_TextXPos + textSizeX <= gfy_ClipXMax ||
        gfy_TextYPos + textSizeY <= gfy_ClipYMax
    ) {
        gfy_internal_PrintChar_NoClip(c, charWidth);
        return;
    }
    const uint8_t *bitImage = gfy_TextData + GFY_MAXIMUM_FONT_HEIGHT * (uint24_t)((unsigned char)c);
    uint8_t *fillLinePtr = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (gfy_TextYPos + (gfy_TextXPos * GFY_LCD_HEIGHT));
    uint8_t b = (1 << 7);
    gfy_TextXPos += charWidth * gfy_TextWidthScale;
    for (uint8_t x = 0; x < charWidth; x++) {
        for (uint8_t u = 0; u < gfy_TextWidthScale; u++) {
            uint8_t *fillPtr = fillLinePtr;
            for (uint8_t y = 0; y < gfy_FontHeight; y++) {
                const uint8_t fillColor = *bitImage & b ? gfy_Text_FG_Color : gfy_Text_BG_Color;
                bitImage++;
                if (fillColor == gfy_Text_TP_Color) {
                    fillPtr += gfy_TextHeightScale;
                    continue;
                }
                for (uint8_t v = 0; v < gfy_TextHeightScale; v++) {
                    if (
                        fillPtr >= (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) &&
                        fillPtr < (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer + GFY_LCD_WIDTH * GFY_LCD_HEIGHT)
                    ) {
                        *fillPtr = fillColor;   
                    }
                    fillPtr++;
                }
            }
            fillLinePtr += GFY_LCD_HEIGHT;
            bitImage -= gfy_FontHeight;
        }
        b >>= 1;
    }
}

/* gfy_PrintInt */

void gfy_PrintInt(int24_t n, uint8_t length) {
    if (n >= 0) {
        gfy_PrintUInt(n, length);
        return;
    }
    gfy_PrintChar('-');
    gfy_PrintUInt(-n, length);
}

/* gfy_PrintUInt */

void gfy_PrintUInt(uint24_t n, uint8_t length) {
    if (length == 0) {
        return;
    }
    length = (length > 8) ? 8 : length;
    uint8_t digit_count = 0;
    {
        int24_t n_temp = n;
        while (n_temp != 0) {
            n_temp /= 10;
            digit_count++;
        }
    }
    for (uint8_t l = digit_count; l < length; l++) {
        gfy_PrintChar('0');
    }
    uint8_t digit;
    switch (digit_count) {
        case 8: // 10'000'000 <= n <= 16'777'215
            gfy_PrintChar('1');
        case 7:
            digit = ((n / 1000000) % 10) + '0';
            gfy_PrintChar(digit);
        case 6:
            digit = ((n / 100000) % 10) + '0';
            gfy_PrintChar(digit);
        case 5:
            digit = ((n / 10000) % 10) + '0';
            gfy_PrintChar(digit);
        case 4:
            digit = ((n / 1000) % 10) + '0';
            gfy_PrintChar(digit);
        case 3:
            digit = ((n / 100) % 10) + '0';
            gfy_PrintChar(digit);
        case 2:
            digit = ((n / 10) % 10) + '0';
            gfy_PrintChar(digit);
        case 1:
            digit = (n % 10) + '0';
            gfy_PrintChar(digit);
        case 0:
        return;
    }
}

/* gfy_PrintString */

void gfy_PrintString(const char *string) {
    while (*string != '\0') {
        gfy_PrintChar(*string);
        string++;
    }
}

/* gfy_PrintStringXY */

void gfy_PrintStringXY(const char *string, int24_t x, int24_t y) {
    gfy_SetTextXY(x,y);
    while (*string != '\0') {
        gfy_PrintChar(*string);
        string++;
    }
}

/* gfy_SetTextXY */

#ifdef GRAPHY_SHARE_SYMBOLS
void gfy_SetTextXY(int24_t x, int24_t y) {
    gfy_TextXPos = x;
    gfy_TextYPos = y;
}
#else
void gfy_SetTextXY(int24_t, int24_t) __attribute__((alias("gfx_SetTextXY")));
#endif

/* gfy_SetTextBGColor */

#ifndef GRAPHY_SHARE_SYMBOLS
uint8_t gfy_SetTextBGColor(uint8_t color) {
    const uint8_t prev_Color = gfy_Text_BG_Color;
    gfy_Text_BG_Color = color;
    return prev_Color;
}
#else
uint8_t gfy_SetTextBGColor(uint8_t) __attribute__((alias("gfx_SetTextBGColor")));
#endif

/* gfy_SetTextFGColor */

#ifndef GRAPHY_SHARE_SYMBOLS
uint8_t gfy_SetTextFGColor(uint8_t color) {
    const uint8_t prev_Color = gfy_Text_FG_Color;
    gfy_Text_FG_Color = color;
    return prev_Color;
}
#else
uint8_t gfy_SetTextFGColor(uint8_t) __attribute__((alias("gfx_SetTextFGColor")));
#endif

/* gfy_SetTextTransparentColor */

#ifndef GRAPHY_SHARE_SYMBOLS
uint8_t gfy_SetTextTransparentColor(uint8_t color) {
    const uint8_t prev_Color = gfy_Text_TP_Color;
    gfy_Text_TP_Color = color;
    return prev_Color;
}
#else
uint8_t gfy_SetTextTransparentColor(uint8_t) __attribute__((alias("gfx_SetTextTransparentColor")));
#endif

/* gfy_SetFontData */

#ifndef GRAPHY_SHARE_SYMBOLS
uint8_t *gfy_SetFontData(const uint8_t *data) {
    uint8_t* temp = (uint8_t*)gfy_TextData;
    gfy_TextData = (data == NULL) ? gfy_DefaultTextData : data;
    return temp;
}
#else
uint8_t *gfy_SetFontData(const uint8_t*) __attribute__((alias("gfx_SetFontData")));
#endif

/* gfy_SetFontSpacing */

#ifndef GRAPHY_SHARE_SYMBOLS
void gfy_SetFontSpacing(const uint8_t *data) {
    gfy_CharSpacing = (data == NULL) ? gfy_DefaultCharSpacing : data;
}
#else
void gfy_SetFontSpacing(const uint8_t*) __attribute__((alias("gfx_SetFontSpacing")));
#endif

/* gfy_SetMonospaceFont */

#ifndef GRAPHY_SHARE_SYMBOLS
void gfy_SetMonospaceFont(uint8_t spacing) {
    gfy_MonospaceFont = spacing;
}
#else
void gfy_SetMonospaceFont(uint8_t spacing) __attribute__((alias("gfx_SetMonospaceFont")));
#endif

/* gfy_GetStringWidth */

#ifndef GRAPHY_SHARE_SYMBOLS
uint24_t gfy_GetStringWidth(const char *string) {
    uint24_t len = 0;
    while (*string != '\0') {
        len += gfy_GetCharWidth(*string);
        string++;
    }
    return len * gfy_TextWidthScale;
}
#else
uint24_t gfy_GetStringWidth(const char*) __attribute__((alias("gfx_GetStringWidth")));
#endif

/* gfy_GetCharWidth */

#ifndef GRAPHY_SHARE_SYMBOLS
uint24_t gfy_GetCharWidth(char c) {
    return (gfy_MonospaceFont != 0) ? gfy_MonospaceFont : gfy_CharSpacing[(unsigned char)c];
}
#else
uint24_t gfy_GetCharWidth(char) __attribute__((alias("gfx_GetCharWidth")));
#endif

/* gfy_GetTextX */

#ifndef GRAPHY_SHARE_SYMBOLS
int24_t gfy_GetTextX(void) {
    return gfy_TextXPos;
}
#else
int24_t gfy_GetTextX(void) __attribute__((alias("gfx_GetTextX")));
#endif

/* gfy_GetTextY */

#ifndef GRAPHY_SHARE_SYMBOLS
int24_t gfy_GetTextY(void) {
    return gfy_TextYPos;
}
#else
int24_t gfy_GetTextY(void) __attribute__((alias("gfx_GetTextY")));
#endif

/* gfy_Line */

// Unoptimized routine
static void gfy_internal_Line0(int24_t x0, int24_t y0, int24_t x1, int24_t y1) {
    int24_t dX = x1 - x0;
    int24_t dY = y1 - y0;
    int24_t yI = 1;
    if (dY < 0) {
        yI = -1;
        dY = -dY;
    }
    int24_t dD = 2 * dY - dX;
    const int24_t dD_jump = 2 * (dY - dX);
    dY *= 2;
    int24_t y = y0;
    for (int24_t x = x0; x < x1; x++) {
        gfy_SetPixel_RegionClip(x, y, gfy_Color);
        if (dD > 0) {
            y += yI;
            dD += dD_jump;
        } else {
            dD += dY;
        }
    }
}

// Unoptimized routine
static void gfy_internal_Line1(int24_t x0, int24_t y0, int24_t x1, int24_t y1) {
    int24_t dX = x1 - x0;
    int24_t dY = y1 - y0;
    int24_t xI = 1;
    if (dX < 0) {
        xI = -1;
        dX = -dX;
    }
    int24_t dD = (2 * dX) - dY;
    const int24_t dD_jump = 2 * (dX - dY);
    dX *= 2;
    int24_t x = x0;

    for (int24_t y = y0; y < y1; y++) {
        gfy_SetPixel_RegionClip(x, y, gfy_Color);
        if (dD > 0) {
            x += xI;
            dD += dD_jump;
        } else {
            dD += dX;
        }
    }
}

void gfy_Line(int24_t x0, int24_t y0, int24_t x1, int24_t y1) {
    // Unoptimized routine
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            gfy_internal_Line0(x1, y1, x0, y0);
        } else {
            gfy_internal_Line0(x0, y0, x1, y1);
        }
    } else {
        if (y0 > y1) {
            gfy_internal_Line1(x1, y1, x0, y0);
        } else {
            gfy_internal_Line1(x0, y0, x1, y1);
        }
    }
}

/* gfy_HorizLine */

void gfy_HorizLine(int24_t x, int24_t y, int24_t length) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (y < gfy_ClipYMin || y >= gfy_ClipYMax || x >= gfy_ClipXMax) {
        return;
    }
    if (x < gfy_ClipXMin) {
        length -= gfy_ClipXMin - x;
        x = 0;
    }
    length = (x + length > gfy_ClipXMax) ? (gfy_ClipXMax - x) : length;
    if (length <= 0) {
        return;
    }
    gfy_HorizLine_NoClip(x, y, length);
}

/* gfy_VertLine */

void gfy_VertLine(int24_t x, int24_t y, int24_t length) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (x < gfy_ClipXMin || x >= gfy_ClipXMax || y >= gfy_ClipYMax) {
        return;
    }
    if (y < gfy_ClipYMin) {
        length -= gfy_ClipYMin - y;
        y = 0;
    }
    length = (y + length > gfy_ClipYMax) ? (gfy_ClipYMax - y) : length;
    if (length <= 0) {
        return;
    }
    gfy_VertLine_NoClip(x, y, length);
}

/* gfy_Circle */

// https://zingl.github.io/bresenham.html
/** @todo make function pixel perfect */
void gfy_Circle(
    const int24_t x, const int24_t y, const uint24_t radius
) {
    int24_t r = radius;

    int24_t x_pos = -r;
    int24_t y_pos = 0;
    int24_t err = 2 - 2 * r;
    do {
        gfy_SetPixel_RegionClip(x - x_pos, y + y_pos, gfy_Color);
        gfy_SetPixel_RegionClip(x - y_pos, y - x_pos, gfy_Color);
        gfy_SetPixel_RegionClip(x + x_pos, y - y_pos, gfy_Color);
        gfy_SetPixel_RegionClip(x + y_pos, y + x_pos, gfy_Color);
        r = err;
        if (r <= y_pos) {
            err += ++y_pos * 2 + 1;
        }
        if (r > x_pos || err > y_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos < 0);
}

/* gfy_FillCircle */

// https://zingl.github.io/bresenham.html
/** @todo make function pixel perfect */
void gfy_FillCircle(
    const int24_t x, const int24_t y, const uint24_t radius
) {
    int24_t r = radius;

    int24_t x_pos = -r;
    int24_t y_pos = 0;
    int24_t err = 2 - 2 * r;
    do {
        gfy_VertLine(x - y_pos, y + x_pos, -x_pos * 2 + 1);
        gfy_VertLine(x + y_pos, y + x_pos, -x_pos * 2 + 1);
        r = err;
        if (r <= y_pos) {
            y_pos++;
            err += y_pos * 2 + 1;
        }
        if (r > x_pos || err > y_pos) {
            x_pos++;
            err += x_pos * 2 + 1;
        }
    } while (x_pos < 0);
}

/* gfy_Rectangle */

void gfy_Rectangle(int24_t x, int24_t y, int24_t width, int24_t height) {
    gfy_HorizLine(x            , y             , width );
    gfy_HorizLine(x            , y + height - 1, width );
    gfy_VertLine (x            , y             , height);
    gfy_VertLine (x + width - 1, y             , height);
}

/* gfy_FillRectangle */

void gfy_FillRectangle(int24_t x, int24_t y, int24_t width, int24_t height) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (x < gfy_ClipXMin) {
        width -= gfy_ClipXMin - x;
        x = 0;
    }
    if (y < gfy_ClipYMin) {
        height -= gfy_ClipYMin - y;
        y = 0;
    }
    width = (x + width > gfy_ClipXMax) ? (gfy_ClipXMax - x) : width;
    height = (y + height > gfy_ClipYMax) ? (gfy_ClipYMax - y) : height;
    if (width <= 0 || height <= 0) {
        return;
    }
    gfy_FillRectangle_NoClip(x, y, width, height);
}

/* gfy_Line_NoClip */

// Unoptimized routine
static void gfy_internal_Line0_NoClip(int24_t x0, int24_t y0, int24_t x1, int24_t y1) {
    int24_t dX = x1 - x0;
    int24_t dY = y1 - y0;
    int24_t yI = 1;
    if (dY < 0) {
        yI = -1;
        dY = -dY;
    }
    int24_t dD = 2 * dY - dX;
    const int24_t dD_jump = 2 * (dY - dX);
    dY *= 2;
    int24_t y = y0;
    for (int24_t x = x0; x < x1; x++) {
        gfy_SetPixel_NoClip(x, y, gfy_Color);
        if (dD > 0) {
            y += yI;
            dD += dD_jump;
        } else {
            dD += dY;
        }
    }
}

// Unoptimized routine
static void gfy_internal_Line1_NoClip(int24_t x0, int24_t y0, int24_t x1, int24_t y1) {
    int24_t dX = x1 - x0;
    int24_t dY = y1 - y0;
    int24_t xI = 1;
    if (dX < 0) {
        xI = -1;
        dX = -dX;
    }
    int24_t dD = (2 * dX) - dY;
    const int24_t dD_jump = 2 * (dX - dY);
    dX *= 2;
    int24_t x = x0;

    for (int24_t y = y0; y < y1; y++) {
        gfy_SetPixel_NoClip(x, y, gfy_Color);
        if (dD > 0) {
            x += xI;
            dD += dD_jump;
        } else {
            dD += dX;
        }
    }
}

void gfy_Line_NoClip(uint24_t x0, uint8_t y0, uint24_t x1, uint8_t y1) {
    // Unoptimized routine
    if (abs((int24_t)y1 - (int24_t)y0) < abs((int24_t)x1 - (int24_t)x0)) {
        if (x0 > x1) {
            gfy_internal_Line0_NoClip(x1, y1, x0, y0);
        } else {
            gfy_internal_Line0_NoClip(x0, y0, x1, y1);
        }
    } else {
        if (y0 > y1) {
            gfy_internal_Line1_NoClip(x1, y1, x0, y0);
        } else {
            gfy_internal_Line1_NoClip(x0, y0, x1, y1);
        }
    }
}

/* gfy_HorizLine_NoClip */

void gfy_HorizLine_NoClip(uint24_t x, uint8_t y, uint24_t length) { //x start, y postion, x length
    uint8_t *fill = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (x * GFY_LCD_HEIGHT) + (uint24_t)y;
    for (uint24_t x_cord = 0; x_cord < length; x_cord++) {
        *fill = gfy_Color;
        fill += GFY_LCD_HEIGHT;
    }
}

/* gfy_VertLine_NoClip */

void gfy_VertLine_NoClip(uint24_t x, uint8_t y, uint24_t length) { //x postion, y start, y length
    uint8_t *fill = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + ((uint24_t)y + (x * GFY_LCD_HEIGHT));
    memset(fill, gfy_Color, length);
}

/* gfy_FillCircle_NoClip */

// https://zingl.github.io/bresenham.html
/** @todo make function pixel perfect */
void gfy_FillCircle_NoClip(
    const uint24_t x, const uint8_t y, const uint24_t radius
) {
    int24_t r = radius;

    int24_t x_pos = -r;
    int24_t y_pos = 0;
    int24_t err = 2 - 2 * r;
    do {
        gfy_VertLine_NoClip(x - y_pos, (uint8_t)(y + x_pos), -x_pos * 2 + 1);
        gfy_VertLine_NoClip(x + y_pos, (uint8_t)(y + x_pos), -x_pos * 2 + 1);
        r = err;
        if (r <= y_pos) {
            y_pos++;
            err += y_pos * 2 + 1;
        }
        if (r > x_pos || err > y_pos) {
            x_pos++;
            err += x_pos * 2 + 1;
        }
    } while (x_pos < 0);
}

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
    uint8_t *fill = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + ((uint24_t)y + (x * GFY_LCD_HEIGHT));
    for (uint24_t x_cord = 0; x_cord < width; x_cord++) {
        memset(fill,gfy_Color, height);
        fill += GFY_LCD_HEIGHT;
    }
}

/* gfy_SetClipRegion */

#ifndef GRAPHY_SHARE_SYMBOLS
void gfy_SetClipRegion(int24_t xmin, int24_t ymin, int24_t xmax, int24_t ymax) {
    xmin = (xmin <  0) ? 0 : xmin; xmin = (xmin >= GFY_LCD_WIDTH ) ? (GFY_LCD_WIDTH -  1) : xmin;
    ymin = (ymin <  0) ? 0 : ymin; ymin = (ymin >= GFY_LCD_HEIGHT) ? (GFY_LCD_HEIGHT - 1) : ymin;
    xmax = (xmax <= 0) ? 1 : xmax; xmax = (xmax >  GFY_LCD_WIDTH ) ?  GFY_LCD_WIDTH       : xmax;
    ymax = (ymax <= 0) ? 1 : ymax; ymax = (ymax >  GFY_LCD_HEIGHT) ?  GFY_LCD_HEIGHT      : ymax;
    gfy_ClipXMin = xmin;
    gfy_ClipYMin = ymin;
    gfy_ClipXMax = xmax;
    gfy_ClipYMax = ymax;
}
#else
void gfy_SetClipRegion(int24_t, int24_t, int24_t, int24_t) __attribute__((alias("gfx_SetClipRegion")));
#endif

/* gfy_GetClipRegion */

/** @todo fix the stand-alone implementation */
bool gfy_GetClipRegion(gfy_region_t *region) {
    #ifdef GRAPHY_SHARE_SYMBOLS
        return gfx_GetClipRegion((gfx_region_t*)region);
    #else
        if (
            region->xmin >= GFY_LCD_WIDTH || region->ymin >= GFY_LCD_WIDTH ||
            region->xmax <= 0 || region->ymax <= 0
        ) {
            return false;
        }
        if (region->xmin < 0) { region->xmin = 0; }
        if (region->xmin < 0) { region->xmin = 0; }
        if (region->xmax > GFY_LCD_WIDTH) { region->xmax = GFY_LCD_WIDTH; }
        if (region->ymax > GFY_LCD_WIDTH) { region->ymax = GFY_LCD_WIDTH; }
        return true;
    #endif
}

/* gfy_ShiftDown */

void gfy_ShiftDown(uint8_t pixels) {
    if (pixels == 0) { return; }
    GFY_CLIPPING_ASSUMPTIONS();
    const uint8_t* src_buf = (const uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + gfy_ClipYMin + (gfy_ClipXMin * GFY_LCD_HEIGHT);
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (gfy_ClipYMin + (int8_t)pixels) + (gfy_ClipXMin * GFY_LCD_HEIGHT);
    const int24_t copySize = gfy_ClipXMax - gfy_ClipXMin - (int24_t)pixels;
    if (copySize <= 0) { return; }
    int24_t x0 = gfy_ClipXMin;
    int24_t x1 = gfy_ClipXMax;
    for (int24_t x = x0; x < x1; x++) {
        memmove(dst_buf, src_buf, (size_t)copySize); // memcpy would be UB
        src_buf += GFY_LCD_HEIGHT;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* gfy_ShiftUp */

void gfy_ShiftUp(uint8_t pixels) {
    if (pixels == 0) { return; }
    GFY_CLIPPING_ASSUMPTIONS();
    const uint8_t* src_buf = (const uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + gfy_ClipYMin + (gfy_ClipXMin * GFY_LCD_HEIGHT);
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (gfy_ClipYMin - (int8_t)pixels) + (gfy_ClipXMin * GFY_LCD_HEIGHT);
    const int24_t copySize = gfy_ClipXMax - gfy_ClipXMin - (int24_t)pixels;
    if (copySize <= 0) { return; }
    int24_t x0 = gfy_ClipXMin;
    int24_t x1 = gfy_ClipXMax;
    for (int24_t x = x0; x < x1; x++) {
        memmove(dst_buf, src_buf, (size_t)copySize); // memcpy would be UB
        src_buf += GFY_LCD_HEIGHT;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* gfy_ShiftLeft */

void gfy_ShiftLeft(uint24_t pixels) {
    if (pixels == 0) { return; }
    GFY_CLIPPING_ASSUMPTIONS();
    const uint8_t* src_buf = (const uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + gfy_ClipYMin + (gfy_ClipXMin * GFY_LCD_HEIGHT);
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + gfy_ClipYMin + ((gfy_ClipXMin - (int24_t)pixels) * GFY_LCD_HEIGHT);
    const size_t copySize = gfy_ClipYMax - gfy_ClipYMin;
    int24_t x0 = gfy_ClipXMin + pixels;
    int24_t x1 = gfy_ClipXMax;
    src_buf += pixels * GFY_LCD_HEIGHT;
    dst_buf += pixels * GFY_LCD_HEIGHT;
    for (int24_t x = x0; x < x1; x++) {
        memcpy(dst_buf, src_buf, copySize);
        src_buf += GFY_LCD_HEIGHT;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* gfy_ShiftRight */

void gfy_ShiftRight(uint24_t pixels) {
    if (pixels == 0) { return; }
    GFY_CLIPPING_ASSUMPTIONS();
    const uint8_t* src_buf = (const uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + gfy_ClipYMin + (gfy_ClipXMin * GFY_LCD_HEIGHT);
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + gfy_ClipYMin + ((gfy_ClipXMin - (int24_t)pixels) * GFY_LCD_HEIGHT);
    const size_t copySize = gfy_ClipYMax - gfy_ClipYMin;
    int24_t x0 = gfy_ClipXMin + pixels;
    int24_t x1 = gfy_ClipXMax;
    src_buf += pixels * GFY_LCD_HEIGHT;
    dst_buf += pixels * GFY_LCD_HEIGHT;
    for (int24_t x = x0; x < x1; x++) {
        memcpy(dst_buf, src_buf, copySize);
        src_buf += GFY_LCD_HEIGHT;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* internal tilemap functions */

static void gfy_Sprite_NoClip_Size2(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        memcpy(dst_buf, src_buf, 2);
        src_buf += 2;
        dst_buf += GFY_LCD_HEIGHT;
    }
}
static void gfy_Sprite_NoClip_Size4(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        memcpy(dst_buf, src_buf, 4);
        src_buf += 4;
        dst_buf += GFY_LCD_HEIGHT;
    }
}
static void gfy_Sprite_NoClip_Size8(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        memcpy(dst_buf, src_buf, 8);
        src_buf += 8;
        dst_buf += GFY_LCD_HEIGHT;
    }
}
static void gfy_Sprite_NoClip_Size16(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        memcpy(dst_buf, src_buf, 16);
        src_buf += 16;
        dst_buf += GFY_LCD_HEIGHT;
    }
}
static void gfy_Sprite_NoClip_Size32(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        memcpy(dst_buf, src_buf, 32);
        src_buf += 32;
        dst_buf += GFY_LCD_HEIGHT;
    }
}
static void gfy_Sprite_NoClip_Size64(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        memcpy(dst_buf, src_buf, 64);
        src_buf += 64;
        dst_buf += GFY_LCD_HEIGHT;
    }
}
static void gfy_Sprite_NoClip_Size128(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        memcpy(dst_buf, src_buf, 128);
        src_buf += 128;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* gfy_Tilemap */

void gfy_Tilemap(const gfy_tilemap_t* tilemap, uint24_t x_offset, uint24_t y_offset) {
    GFY_CLIPPING_ASSUMPTIONS();
    // Unoptimized and overclips a bit
    uint24_t map_row = x_offset / tilemap->tile_width;
    uint24_t map_col = y_offset / tilemap->tile_height;
    
    const uint8_t posX_offset = x_offset % tilemap->tile_width;
    const uint8_t posY_offset = y_offset % tilemap->tile_height;

    uint24_t posX0 = tilemap->x_loc - ((posX_offset == 0) ?
        0 : tilemap->tile_width + posX_offset);
    // posX0 = (posX0 < 0) ? 0 : posX0;
    uint24_t posY = tilemap->y_loc - ((posY_offset == 0) ?
        0 : tilemap->tile_height + posY_offset);
    // posY = (posY < 0) ? 0 : posY;
    
    uint8_t draw_sizeX = tilemap->draw_width;
    uint8_t draw_sizeY = tilemap->draw_height;
    if (x_offset % tilemap->tile_width != 0) {
        if (map_row > 0) {
            draw_sizeX++;
            map_row--;
        } else if (posX_offset != 0) {
            posX0 += tilemap->tile_width;
        }
        
    }
    if (y_offset % tilemap->tile_height != 0) {
        if (map_col > 0) {
            draw_sizeY++;
            map_col--;
        } else if (posY_offset != 0) {
            posY += tilemap->tile_height;
        }
    }

    if (draw_sizeX == 0 || draw_sizeY == 0) {
        return;
    }
    
    /* Debugging */ const uint24_t map_size = tilemap->width * tilemap->height;

    uint24_t map_index = map_row + (map_col * tilemap->width);
    const uint24_t map_jump = (tilemap->width - draw_sizeX);

    void (*plot_function)(const gfy_sprite_t*, uint24_t, uint8_t) = gfy_Sprite_NoClip;
    #ifndef USE_GRAPHX_SPRITE_DATA
        switch (tilemap->type_height) {
            case gfy_tile_2_pixel:
                plot_function = gfy_Sprite_NoClip_Size2;
                break;
            case gfy_tile_4_pixel:
                plot_function = gfy_Sprite_NoClip_Size4;
                break;
            case gfy_tile_8_pixel:
                plot_function = gfy_Sprite_NoClip_Size8;
                break;
            case gfy_tile_16_pixel:
                plot_function = gfy_Sprite_NoClip_Size16;
                break;
            case gfy_tile_32_pixel:
                plot_function = gfy_Sprite_NoClip_Size32;
                break;
            case gfy_tile_64_pixel:
                plot_function = gfy_Sprite_NoClip_Size64;
                break;
            case gfy_tile_128_pixel:
                plot_function = gfy_Sprite_NoClip_Size128;
                break;
        }
    #endif
    
    const uint24_t limitX = gfy_ClipXMax - tilemap->tile_width;
    const uint24_t limitY = gfy_ClipYMax - tilemap->tile_height;
    for (uint8_t draw_y = 0; draw_y < draw_sizeY; draw_y++) {
        uint24_t posX = posX0;
        if (posY < limitY) {
            for (uint8_t draw_x = 0; draw_x < draw_sizeX; draw_x++) {
                if (map_index >= map_size) {
                    return; // Optimize this out
                }
                if (posX < limitX) {
                    (*plot_function)(tilemap->tiles[tilemap->map[map_index]], posX, posY);
                } else {
                    gfy_Sprite(tilemap->tiles[tilemap->map[map_index]], posX, posY);
                }
                posX += tilemap->tile_width;
                map_index++;
            }
        } else {
            for (uint8_t draw_x = 0; draw_x < draw_sizeX; draw_x++) {
                if (map_index >= map_size) {
                    return; // Optimize this out
                }
                gfy_Sprite(tilemap->tiles[tilemap->map[map_index]], posX, posY);
                posX += tilemap->tile_width;
                map_index++;
            }
        }
        map_index += map_jump;
        posY += tilemap->tile_height;
    }
}


/* gfy_Tilemap_NoClip */

void gfy_Tilemap_NoClip(const gfy_tilemap_t *tilemap, uint24_t x_offset, uint24_t y_offset) {
    uint24_t map_row = x_offset / tilemap->tile_width;
    uint24_t map_col = y_offset / tilemap->tile_height;
    
    uint24_t map_index = map_row + (map_col * tilemap->width);
    const uint24_t map_jump = (tilemap->width - tilemap->draw_width);

    uint24_t posX0 = tilemap->x_loc - (x_offset % tilemap->tile_width);
    posX0 = (posX0 < 0) ? 0 : posX0;
    uint8_t posY = tilemap->y_loc - (y_offset % tilemap->tile_height);
    posY = (posY < 0) ? 0 : posY;

    void (*plot_function)(const gfy_sprite_t*, uint24_t, uint8_t) = gfy_Sprite_NoClip;

    #ifndef USE_GRAPHX_SPRITE_DATA
        switch (tilemap->type_height) {
            case gfy_tile_2_pixel:
                plot_function = gfy_Sprite_NoClip_Size2;
                break;
            case gfy_tile_4_pixel:
                plot_function = gfy_Sprite_NoClip_Size4;
                break;
            case gfy_tile_8_pixel:
                plot_function = gfy_Sprite_NoClip_Size8;
                break;
            case gfy_tile_16_pixel:
                plot_function = gfy_Sprite_NoClip_Size16;
                break;
            case gfy_tile_32_pixel:
                plot_function = gfy_Sprite_NoClip_Size32;
                break;
            case gfy_tile_64_pixel:
                plot_function = gfy_Sprite_NoClip_Size64;
                break;
            case gfy_tile_128_pixel:
                plot_function = gfy_Sprite_NoClip_Size128;
                break;
        }
    #endif

    for (uint8_t draw_y = 0; draw_y < tilemap->draw_height; draw_y++) {
        uint24_t posX = posX0;
        for (uint8_t draw_x = 0; draw_x < tilemap->draw_width; draw_x++) {
            (*plot_function)(tilemap->tiles[tilemap->map[map_index]], posX, posY);
            posX += tilemap->tile_width;
            map_index++;
        }
        map_index += map_jump;
        posY += tilemap->tile_height;
    }
}

/* gfy_TransparentTilemap */

void gfy_TransparentTilemap(const gfy_tilemap_t* tilemap, uint24_t x_offset, uint24_t y_offset) {
    GFY_CLIPPING_ASSUMPTIONS();
    // Unoptimized and overclips a bit
    uint24_t map_row = x_offset / tilemap->tile_width;
    uint24_t map_col = y_offset / tilemap->tile_height;
    
    const uint8_t posX_offset = x_offset % tilemap->tile_width;
    const uint8_t posY_offset = y_offset % tilemap->tile_height;

    uint24_t posX0 = tilemap->x_loc - ((posX_offset == 0) ?
        0 : tilemap->tile_width + posX_offset);
    // posX0 = (posX0 < 0) ? 0 : posX0;
    uint24_t posY = tilemap->y_loc - ((posY_offset == 0) ?
        0 : tilemap->tile_height + posY_offset);
    // posY = (posY < 0) ? 0 : posY;
    
    uint8_t draw_sizeX = tilemap->draw_width;
    uint8_t draw_sizeY = tilemap->draw_height;
    if (x_offset % tilemap->tile_width != 0) {
        if (map_row > 0) {
            draw_sizeX++;
            map_row--;
        } else if (posX_offset != 0) {
            posX0 += tilemap->tile_width;
        }
        
    }
    if (y_offset % tilemap->tile_height != 0) {
        if (map_col > 0) {
            draw_sizeY++;
            map_col--;
        } else if (posY_offset != 0) {
            posY += tilemap->tile_height;
        }
    }

    /* Debugging */ const uint24_t map_size = tilemap->width * tilemap->height;

    uint24_t map_index = map_row + (map_col * tilemap->width);
    const uint24_t map_jump = (tilemap->width - draw_sizeX);

    const uint24_t limitX = gfy_ClipXMax - tilemap->tile_width;
    const uint24_t limitY = gfy_ClipYMax - tilemap->tile_height;
    for (uint8_t draw_y = 0; draw_y < draw_sizeY; draw_y++) {
        uint24_t posX = posX0;
        if (posY < limitY) {
            for (uint8_t draw_x = 0; draw_x < draw_sizeX; draw_x++) {
                if (map_index >= map_size) {
                    return; // Optimize this out
                }
                if (posX < limitX) {
                    gfy_TransparentSprite_NoClip(tilemap->tiles[tilemap->map[map_index]], posX, posY);
                } else {
                    gfy_TransparentSprite(tilemap->tiles[tilemap->map[map_index]], posX, posY);
                }
                posX += tilemap->tile_width;
                map_index++;
            }
        } else {
            for (uint8_t draw_x = 0; draw_x < draw_sizeX; draw_x++) {
                if (map_index >= map_size) {
                    return; // Optimize this out
                }
                gfy_TransparentSprite(tilemap->tiles[tilemap->map[map_index]], posX, posY);
                posX += tilemap->tile_width;
                map_index++;
            }
        }
        map_index += map_jump;
        posY += tilemap->tile_height;
    }
}

/* gfy_TransparentTilemap_NoClip */

void gfy_TransparentTilemap_NoClip(const gfy_tilemap_t *tilemap, uint24_t x_offset, uint24_t y_offset) {
    uint24_t map_row = x_offset / tilemap->tile_width;
    uint24_t map_col = y_offset / tilemap->tile_height;
    uint24_t map_index = map_row + (map_col * tilemap->width);
    const uint24_t map_jump = (tilemap->width - tilemap->draw_width);

    uint24_t posX0 = tilemap->x_loc - (x_offset % tilemap->tile_width);
    posX0 = (posX0 < 0) ? 0 : posX0;
    uint8_t posY = tilemap->y_loc - (y_offset % tilemap->tile_height);
    posY = (posY < 0) ? 0 : posY;
    for (uint8_t draw_y = 0; draw_y < tilemap->draw_height; draw_y++) {
        uint24_t posX = posX0;
        for (uint8_t draw_x = 0; draw_x < tilemap->draw_width; draw_x++) {
            gfy_TransparentSprite_NoClip(tilemap->tiles[tilemap->map[map_index]], posX, posY);
            posX += tilemap->tile_width;
            map_index++;
        }
        map_index += map_jump;
        posY += tilemap->tile_height;
    }
}

/* gfy_TilePtr */

uint8_t *gfy_TilePtr(const gfy_tilemap_t *tilemap, uint24_t x_offset, uint24_t y_offset) {
    uint24_t map_row = x_offset / tilemap->tile_width;
    uint24_t map_col = y_offset / tilemap->tile_height;
    
    uint24_t map_index = map_row + (map_col * tilemap->width);
    return &(tilemap->map[map_index]);
}

/* gfy_TilePtrMapped */

uint8_t *gfy_TilePtrMapped(const gfy_tilemap_t *tilemap, uint8_t col, uint8_t row) {
    uint24_t map_index = row + (col * tilemap->width);
    return &(tilemap->map[map_index]);
}

/* gfy_Reserved */



/* gfy_AllocSprite */

gfy_sprite_t *gfy_AllocSprite(
    uint8_t width,
    uint8_t height,
    void *(*malloc_routine)(size_t)
) {
    #ifdef GRAPHY_STANDALONE
        return (gfy_sprite_t*)((*malloc_routine)(sizeof(gfy_sprite_t) + (width * height)));
    #else
        return (gfy_sprite_t*)gfx_AllocSprite(width, height, malloc_routine);
    #endif
}

/* gfy_Sprite */

void gfy_Sprite(const gfy_sprite_t *restrict sprite, int24_t x, int24_t y) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (
        x >= gfy_ClipXMax || y >= gfy_ClipYMax ||
        sprite->width == 0 || sprite->height == 0 ||
        x + sprite->width < gfy_ClipXMin ||
        y + sprite->height < gfy_ClipYMin
    ) {
        return;
    }
    if (
        x >= gfy_ClipXMin &&
        y >= gfy_ClipYMin &&
        x + sprite->width <= gfy_ClipXMax &&
        y + sprite->height <= gfy_ClipYMax
    ) {
        gfy_Sprite_NoClip(sprite, (uint24_t)x, (uint8_t)y);
        return;
    }
    const uint8_t min_clipX = (x < gfy_ClipXMin) ? (gfy_ClipXMin - x) : 0;
    const uint8_t min_clipY = (y < gfy_ClipYMin) ? (gfy_ClipYMin - y) : 0;
    const uint8_t max_clipX = ((x + sprite->width) > gfy_ClipXMax) ? ((x + sprite->width) - gfy_ClipXMax) : 0;
    const uint8_t max_clipY = ((y + sprite->height) > gfy_ClipYMax) ? ((y + sprite->height) - gfy_ClipYMax) : 0;

    uint8_t sizeX = sprite->width - (min_clipX + max_clipX);
    uint8_t sizeY = sprite->height - (min_clipY + max_clipY);
    
    const uint8_t* src_buf = sprite->data + min_clipY + (min_clipX * sprite->height);
    
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (y + min_clipY) + ((x + min_clipX) * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sizeX; x_cord++) {
        memcpy(dst_buf, src_buf, sizeY);
        src_buf += sprite->height;
        dst_buf += GFY_LCD_HEIGHT;
    }
}

/* gfy_TransparentSprite */

void gfy_TransparentSprite(const gfy_sprite_t *restrict sprite, int24_t x, int24_t y) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (
        x >= gfy_ClipXMax || y >= gfy_ClipYMax ||
        sprite->width == 0 || sprite->height == 0 ||
        x + sprite->width < gfy_ClipXMin ||
        y + sprite->height < gfy_ClipYMin
    ) {
        return;
    }
    if (
        x >= gfy_ClipXMin &&
        y >= gfy_ClipYMin &&
        x + sprite->width <= gfy_ClipXMax &&
        y + sprite->height <= gfy_ClipYMax
    ) {
        gfy_TransparentSprite_NoClip(sprite, (uint24_t)x, (uint8_t)y);
        return;
    }
    const uint8_t min_clipX = (x < gfy_ClipXMin) ? (gfy_ClipXMin - x) : 0;
    const uint8_t min_clipY = (y < gfy_ClipYMin) ? (gfy_ClipYMin - y) : 0;
    const uint8_t max_clipX = ((x + sprite->width) > gfy_ClipXMax) ? ((x + sprite->width) - gfy_ClipXMax) : 0;
    const uint8_t max_clipY = ((y + sprite->height) > gfy_ClipYMax) ? ((y + sprite->height) - gfy_ClipYMax) : 0;

    uint8_t sizeX = sprite->width - (min_clipX + max_clipX);
    uint8_t sizeY = sprite->height - (min_clipY + max_clipY);
    const uint8_t* src_buf = sprite->data + min_clipY + (min_clipX * sprite->height);
    const uint8_t clip_jumpY = min_clipY + max_clipY;

    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (y + min_clipY) + ((x + min_clipX) * GFY_LCD_HEIGHT);
    const uint24_t dst_jump = GFY_LCD_HEIGHT - sizeY;

    for (uint8_t x_cord = 0; x_cord < sizeX; x_cord++) {
        for (uint8_t y_cord = 0; y_cord < sizeY; y_cord++) {
            *dst_buf = (*src_buf != gfy_Transparent_Color) ? *src_buf : *dst_buf;
            src_buf++;
            dst_buf++;
        }
        src_buf += clip_jumpY;
        dst_buf += dst_jump;
    }
}

/* gfy_Sprite_NoClip */

void gfy_Sprite_NoClip(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    
    // Compare speed
    #define gfy_Sprite_NoClip_memcpy

    #ifdef gfy_Sprite_NoClip_memcpy
        const uint8_t* src_buf = sprite->data;
        uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
        
        for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
            memcpy(dst_buf, src_buf, sprite->height);
            src_buf += sprite->height;
            dst_buf += GFY_LCD_HEIGHT;
        }
    #else
        const uint8_t* src_buf = sprite->data;
        uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
        const uint24_t dst_jump = GFY_LCD_HEIGHT - sprite->height;
        
        for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
            for (uint8_t y_cord = 0; y_cord < sprite->height; y_cord++) {
                // Reduces size??
                    // *dst_buf++ = *src_buf++;
                // Original
                    *dst_buf = *src_buf;
                    src_buf++;
                    dst_buf++;
            }
            dst_buf += dst_jump;
        }
    #endif
}

/* gfy_TransparentSprite_NoClip */

void gfy_TransparentSprite_NoClip(const gfy_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    const uint24_t dst_jump = GFY_LCD_HEIGHT - sprite->height;
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        for (uint8_t y_cord = 0; y_cord < sprite->height; y_cord++) {
            *dst_buf = (*src_buf != gfy_Transparent_Color) ? *src_buf : *dst_buf;
            src_buf++;
            dst_buf++;
        }
        dst_buf += dst_jump;
    }
}

/* gfy_GetSprite */

gfy_sprite_t *gfy_GetSprite(gfy_sprite_t *sprite_buffer, int24_t x, int24_t y) {
    uint8_t* dst_buf = sprite_buffer->data;
    const uint8_t* src_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    
    for (uint8_t x_cord = 0; x_cord < sprite_buffer->width; x_cord++) {
        memcpy(dst_buf, src_buf, sprite_buffer->height);
        src_buf += GFY_LCD_HEIGHT;
        dst_buf += sprite_buffer->height;
    }
    return sprite_buffer;
}

/* gfy_ScaledSprite_NoClip */

void gfy_ScaledSprite_NoClip(
    const gfy_sprite_t *sprite,
    const uint24_t x,
    const uint8_t y,
    const uint8_t width_scale,
    const uint8_t height_scale
) {
    if (width_scale == 0 || height_scale == 0) {
        return;
    }
    const uint8_t* src_buf = sprite->data;
    uint8_t* buf_start = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    uint8_t* dst_buf = buf_start;
    const uint24_t dst_jump = (GFY_LCD_HEIGHT * width_scale) - (sprite->height * height_scale);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        for (uint8_t y_cord = 0; y_cord < sprite->height; y_cord++) {
            const uint8_t color = *src_buf;
            src_buf++;
            for (uint8_t v = 0; v < height_scale; v++) {
                *dst_buf = color;
                dst_buf++;
            }
        }
        dst_buf += dst_jump;
    }
    
    if (width_scale == 1) {
        return;
    }

    // memcpy columns
    uint8_t* dst_col = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    const uint24_t copySize = sprite->height * height_scale; // This could probably be uint8_t
    for (uint8_t col = 0; col < sprite->width; col++) {
        const uint8_t* const src_col = (uint8_t*)dst_col;
        dst_col += GFY_LCD_HEIGHT;
        for (uint8_t x_cord = 1; x_cord < width_scale; x_cord++) {
            memcpy(dst_col, src_col, copySize);
            dst_col += GFY_LCD_HEIGHT;
        }
    }
}

/* gfy_ScaledTransparentSprite_NoClip */

void gfy_ScaledTransparentSprite_NoClip(
    const gfy_sprite_t *sprite,
    const uint24_t x,
    const uint8_t y,
    const uint8_t width_scale,
    const uint8_t height_scale
) {
    if (width_scale == 0 || height_scale == 0) {
        return;
    }
    const uint8_t* src_buf = sprite->data;
    uint8_t* buf_start = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    uint8_t* dst_buf = buf_start;
    const uint24_t dst_jump = GFY_LCD_HEIGHT - (sprite->height * height_scale);
    
    for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
        for (uint8_t u = 0; u < width_scale; u++) {
            for (uint8_t y_cord = 0; y_cord < sprite->height; y_cord++) {
                const uint8_t color = *src_buf;
                src_buf++;
                if (color == gfy_Transparent_Color) {
                    dst_buf += height_scale;
                    continue;
                }
                for (uint8_t v = 0; v < height_scale; v++) {
                    *dst_buf = color;
                    dst_buf++;
                }
            }
            dst_buf += dst_jump;
            src_buf -= sprite->height;
        }
        src_buf += sprite->height;
    }
}

/* gfy_FlipSpriteY */

#ifndef GRAPHY_STANDALONE
gfy_sprite_t *gfy_FlipSpriteY(const gfy_sprite_t *sprite_in, gfy_sprite_t *sprite_out) {
    return (gfy_sprite_t*)gfx_FlipSpriteX((const gfx_sprite_t*)sprite_in, (gfx_sprite_t*)sprite_out);
}
#endif

/* gfy_FlipSpriteX */

#ifndef GRAPHY_STANDALONE
gfy_sprite_t *gfy_FlipSpriteX(const gfy_sprite_t* sprite_in, gfy_sprite_t* sprite_out) {
    return (gfy_sprite_t*)gfx_FlipSpriteY((const gfx_sprite_t*)sprite_in, (gfx_sprite_t*)sprite_out);
}
#endif

/* gfy_RotateSpriteC */

#ifndef GRAPHY_STANDALONE
gfy_sprite_t *gfy_RotateSpriteC(const gfy_sprite_t *sprite_in, gfy_sprite_t *sprite_out) {
     return (gfy_sprite_t*)gfx_RotateSpriteC((const gfx_sprite_t*)sprite_in, (gfx_sprite_t*)sprite_out);
}
#endif

/* gfy_RotateSpriteCC */

#ifndef GRAPHY_STANDALONE
gfy_sprite_t *gfy_RotateSpriteCC(const gfy_sprite_t *sprite_in, gfy_sprite_t *sprite_out) {
    return (gfy_sprite_t*)gfx_RotateSpriteCC((const gfx_sprite_t*)sprite_in, (gfx_sprite_t*)sprite_out);
}
#endif

/* gfy_RotateSpriteHalf */

gfy_sprite_t *gfy_RotateSpriteHalf(const gfy_sprite_t *sprite_in, gfy_sprite_t *sprite_out) {
    #ifndef GRAPHY_STANDALONE
        return (gfy_sprite_t*)gfx_RotateSpriteHalf((const gfx_sprite_t*)sprite_in, (gfx_sprite_t*)sprite_out);
    #else
        const uint8_t* src_buf = sprite_in->data;
        uint8_t* dst_buf = sprite_out->data + (sprite_in->width * sprite_in->height);
        while (dst_buf != sprite_out->data) {
            dst_buf--;
            *dst_buf = *src_buf;
            src_buf++;
        }
        sprite_out->width = sprite_in->width;
        sprite_out->height = sprite_in->height;
        return sprite_out;
    #endif
}

/* gfy_Polygon */

void gfy_Polygon(const int24_t *points, size_t num_points) {
    if (num_points < 2) {
        return;
    }
    gfy_Line(
        points[num_points * 2 - 2], points[num_points * 2 - 1],
        points[0], points[1]
    );
    for (size_t i = 2; i < 2 * num_points; i += 2) {
        gfy_Line(
            points[i + 0], points[i + 1],
            points[i + 2], points[i + 3]
        );
    }
}

/* gfy_Polygon_NoClip */

void gfy_Polygon_NoClip(const int24_t *points, size_t num_points) {
    if (num_points < 2) {
        return;
    }
    gfy_Line(
        points[num_points * 2 - 2], points[num_points * 2 - 1],
        points[0], points[1]
    );
    for (size_t i = 2; i < 2 * num_points; i += 2) {
        gfy_Line(
            points[i + 0], points[i + 1],
            points[i + 2], points[i + 3]
        );
    }
}

/* gfy_FillTriangle */

// y2 >= y1 >= y0
static void gfy_internal_triangle_sort(
    int24_t* x0, int24_t* y0,
    int24_t* x1, int24_t* y1,
    int24_t* x2, int24_t* y2
) {
    #define triangle_swap(a, b) \
        { \
            int24_t temp = *(a); \
            *(a) = *(b); \
            *(b) = temp; \
        }

    if (*y0 > *y1) {
        triangle_swap(x0, x1);
        triangle_swap(y0, y1);
    }
    if (*y1 > *y2) {
        triangle_swap(x1, x2);
        triangle_swap(y1, y2);
    }
    if (*y0 > *y1) {
        triangle_swap(x0, x1);
        triangle_swap(y0, y1);
    }

    #undef triangle_swap
}

void gfy_FillTriangle(
    int24_t x0, int24_t y0,
    int24_t x1, int24_t y1,
    int24_t x2, int24_t y2
) {
    gfy_internal_triangle_sort(
        &x0, &y0,
        &x1, &y1,
        &x2, &y2
    );
    if (y0 == y2) { // Degenerate Triangle
        int24_t x_min = (x0 < x1) ? x0 : x1;
        x_min = (x_min < x2) ? x_min : x2;
        int24_t x_max = (x0 > x1) ? x0 : x1;
        x_max = (x_max > x2) ? x_max : x2;
        gfy_HorizLine(x_min, y0, x_max - x_min + 1);
        return;
    }
    int24_t sa = 0;
    int24_t sb = 0;
    int24_t dx01 = x1 - x0;
    int24_t dx02 = x2 - x0;
    int24_t dy01 = y1 - y0;
    int24_t dy02 = y2 - y0;
    int24_t dx12 = x2 - x1;
    int24_t dy12 = y2 - y1;
    int24_t last;
    if (y1 == y2) {
        last = y1;
    } else {
        last = y1 - 1;
    }
    int24_t y;
    for (y = y0; y <= last; y++) {
        int24_t a = x0 + sa / dy01;
        int24_t b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if (b < a) { // Swap
            int24_t temp = a;
            a = b;
            b = temp;
        }
        gfy_HorizLine(a, y, b - a + 1);
    }
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y <= y2; y++) {
        int24_t a = x1 + sa / dy12;
        int24_t b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (b < a) { // Swap
            int24_t temp = a;
            a = b;
            b = temp;
        }
        gfy_HorizLine(a, y, b - a + 1);
    }
}

/* gfy_FillTriangle_NoClip */

void gfy_FillTriangle_NoClip(
    int24_t x0, int24_t y0,
    int24_t x1, int24_t y1,
    int24_t x2, int24_t y2
) {
    gfy_internal_triangle_sort(
        &x0, &y0,
        &x1, &y1,
        &x2, &y2
    );
    if (y0 == y2) { // Degenerate Triangle
        int24_t x_min = (x0 < x1) ? x0 : x1;
        x_min = (x_min < x2) ? x_min : x2;
        int24_t x_max = (x0 > x1) ? x0 : x1;
        x_max = (x_max > x2) ? x_max : x2;
        gfy_HorizLine_NoClip(x_min, (uint8_t)y0, x_max - x_min + 1);
        return;
    }
    int24_t sa = 0;
    int24_t sb = 0;
    int24_t dx01 = x1 - x0;
    int24_t dx02 = x2 - x0;
    int24_t dy01 = y1 - y0;
    int24_t dy02 = y2 - y0;
    int24_t dx12 = x2 - x1;
    int24_t dy12 = y2 - y1;
    int24_t last;
    if (y1 == y2) {
        last = y1;
    } else {
        last = y1 - 1;
    }
    int24_t y;
    for (y = y0; y <= last; y++) {
        int24_t a = x0 + sa / dy01;
        int24_t b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if (b < a) { // Swap
            int24_t temp = a;
            a = b;
            b = temp;
        }
        gfy_HorizLine_NoClip(a, (uint8_t)y, b - a + 1);
    }
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y <= y2; y++) {
        int24_t a = x1 + sa / dy12;
        int24_t b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (b < a) { // Swap
            int24_t temp = a;
            a = b;
            b = temp;
        }
        gfy_HorizLine_NoClip(a, (uint8_t)y, b - a + 1);
    }
}

//------------------------------------------------------------------------------
// v2 functions
//------------------------------------------------------------------------------

/* gfy_Deprecated */



/* gfy_SetTextScale */

#ifndef GRAPHY_SHARE_SYMBOLS
void gfy_SetTextScale(uint8_t width_scale, uint8_t height_scale) {
    gfy_TextWidthScale = width_scale;
    gfy_TextHeightScale = height_scale;
}
#else
void gfy_SetTransparentColor(uint8_t, uint8_t) __attribute__((alias("gfx_SetTextScale")));
#endif

//------------------------------------------------------------------------------
// v3 functions
//------------------------------------------------------------------------------

/* gfy_SetTransparentColor */

#ifndef GRAPHY_SHARE_SYMBOLS
uint8_t gfy_SetTransparentColor(uint8_t index) {
    const uint8_t prev_Color = gfy_Transparent_Color;
    gfy_Transparent_Color = index;
    return prev_Color;
}
#else
uint8_t gfy_SetTransparentColor(uint8_t) __attribute__((alias("gfx_SetTransparentColor")));
#endif

/* gfy_ZeroScreen */

#ifdef GRAPHY_STANDALONE
void gfy_ZeroScreen(void) {
    gfy_FillScreen(0x00);
}
#else
void gfy_ZeroScreen(void) __attribute__((alias("gfx_ZeroScreen")));
#endif

/* gfy_SetTextConfig */

#ifndef GRAPHY_SHARE_SYMBOLS
void gfy_SetTextConfig(uint8_t config) {
    gfy_PrintChar_Clip = config;
}
#else
void gfy_SetTextConfig(uint8_t) __attribute__((alias("gfx_SetTextConfig")));
#endif

/* gfy_GetSpriteChar */

gfy_sprite_t *gfy_GetSpriteChar(__attribute__((unused)) char c) {
    #ifndef GRAPHY_STANDALONE
        return (gfy_sprite_t*)gfx_GetSpriteChar(c);
    #else
        const uint8_t *bitImage = gfy_TextData + GFY_MAXIMUM_FONT_HEIGHT * (uint24_t)((unsigned char)c);
        uint8_t *fillPtr = gfy_TmpCharSprite.data;
        
        const uint8_t CharWidth = gfy_GetCharWidth(c);
        
        for (uint8_t y = 0; y < gfy_FontHeight; y++) {
            uint8_t b = 1;
            uint8_t x = 0;
            for (; x < CharWidth; x++) {
                *fillPtr = (*bitImage & b) ? gfy_Text_FG_Color : gfy_Text_BG_Color;
                fillPtr++;
                b <<= 1;
            }
            for (; x < GFY_MAXIMUM_FONT_WIDTH; x++) {
                *fillPtr = gfy_Text_BG_Color;
                fillPtr++;
            }
            bitImage++;	
        }
        memset(fillPtr, gfy_Text_BG_Color,
            (GFY_MAXIMUM_FONT_HEIGHT - gfy_FontHeight) * GFY_MAXIMUM_FONT_WIDTH
        );
        return &gfy_TmpCharSprite;
    #endif
}

//------------------------------------------------------------------------------
// v4 functions
//------------------------------------------------------------------------------

/* gfy_Lighten */

#ifdef GRAPHY_STANDALONE
uint16_t gfy_Lighten(uint16_t color, uint8_t amount) {
    return ~gfy_Darken(~color, amount);
}
#else
uint16_t gfy_Lighten(uint16_t, uint8_t) __attribute__((alias("gfx_Lighten")));
#endif

/* gfy_Darken */

#ifdef GRAPHY_STANDALONE
uint16_t gfy_Darken(uint16_t color, uint8_t amount) {
    uint8_t r = (uint8_t)(color & 0x1F);
    uint8_t g = (uint8_t)((color & 0x3E0) >> 4) + ((color & 0x8000) ? 1 : 0);
    uint8_t b = (uint8_t)((color & 0x7C00) >> 10);
    r = (r * amount + 128) / 256;
    g = (g * amount + 128) / 256;
    b = (b * amount + 128) / 256;
    return ((g & 0x1) ? 0x8000 : 0x0000) | (r << 10) | ((g >> 1) << 5) | b;
}
#else
uint16_t gfy_Darken(uint16_t, uint8_t) __attribute__((alias("gfx_Darken")));
#endif

//------------------------------------------------------------------------------
// v5 functions
//------------------------------------------------------------------------------

/* gfy_SetFontHeight */

#ifndef GRAPHY_SHARE_SYMBOLS
uint8_t gfy_SetFontHeight(uint8_t height) {
    // The assembly doesn't appear to do any input validation
    uint8_t temp = gfy_FontHeight;
    gfy_FontHeight = height;
    return temp;
}
#else
uint8_t gfy_SetFontHeight(uint8_t) __attribute__((alias("gfx_SetFontHeight")));
#endif

/* gfy_ScaleSprite */

gfy_sprite_t *gfy_ScaleSprite(const gfy_sprite_t *sprite_in, gfy_sprite_t *sprite_out) {
    #ifndef GRAPHY_STANDALONE
        return (gfy_sprite_t*)gfx_ScaleSprite((const gfx_sprite_t*)sprite_in, (gfx_sprite_t*)sprite_out);
    #else
        uint8_t* dst_ptr = sprite_out->data;
        for (uint8_t y = 0; y < sprite_out->height; y++) {
            const uint8_t* src_ptr = sprite_in->data + ((y * sprite_in->height) / sprite_in->height) * sprite_out->width;
            for (uint8_t x = 0; x < sprite_out->width; x++) {
                *dst_ptr = *(src_ptr + (x * sprite_out->width) / sprite_in->width);
                dst_ptr++;
            }
        }
        return sprite_out;
    #endif
}

/* gfy_FloodFill */

// It is probably better (and safer) to reimplement this routine
#if 0

    // Debug printf statements
    // #define FLOODFILL_DEBUG

    // Disables FLOODFILL_DEBUG when compiling for the ti84ce
    #ifdef FLOODFILL_DEBUG
        #ifdef _EZ80
            #undef FLOODFILL_DEBUG
        #endif
    #endif

    // compress_stack() will run a little bit faster, this can also alter how many pixels are written before a soft stack-overflow
    #define FLOODFILL_FAST_COMPRESS

    // malloc and free ff_stack each time
    #define FLOODFILL_MALLOC_STACK

    #define FLOODFILL_STACK_SIZE (3224)

    typedef struct FloodFillCord {
        int24_t x;
        uint8_t y;
    } FloodFillCord;

    #ifndef FLOODFILL_MALLOC_STACK
        #define Maximum_FloodFill_Stack_Depth (FLOODFILL_STACK_SIZE / sizeof(FloodFillCord))

        static FloodFillCord ff_stack[Maximum_FloodFill_Stack_Depth];
    #endif

    // static int16_t ff_ClipXMin = 0;
    // static int16_t ff_ClipXMax = GFY_LCD_WIDTH;
    // static uint8_t ff_ClipYMin = 0;
    // static uint8_t ff_ClipYMax = GFY_LCD_HEIGHT;

    static bool ffcord_redundant(
        #ifdef FLOODFILL_MALLOC_STACK
            FloodFillCord* const ff_stack,
        #endif
        const uint24_t index, const uint8_t replace,
        const uint8_t ff_ClipYMax, const uint8_t ff_ClipYMin
    ) {
        const FloodFillCord ff_cord = ff_stack[index];

        uint8_t* test = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (ff_cord.x * GFY_LCD_HEIGHT + ff_cord.y);
        test++; // Down 1
        if (
            ff_cord.y + 1 < ff_ClipYMax &&
            *test == replace
        ) {
            return false;
        }

        test -= 2; // Up 2
        if (
            ff_cord.y != 0 &&
            ff_cord.y - 1 >= ff_ClipYMin &&
            *test == replace
        ) {
            return false;
        }

        test += GFY_LCD_HEIGHT + 1; // Right 1, Down 1
        if (
            ff_cord.x + 1 < gfy_ClipXMax &&
            *test == replace
        ) {
            return false;
        }

        test -= 2 * GFY_LCD_HEIGHT; // Left 2
        if (
            ff_cord.x - 1 >= gfy_ClipXMin &&
            *test == replace
        ) {
            return false;
        }
        return true;
    }

    // Returns new stack size
    static uint24_t compress_stack(
        #ifdef FLOODFILL_MALLOC_STACK
            FloodFillCord* const ff_stack,
        #endif
        const uint24_t stack_size, const uint8_t replace,
        const uint8_t ff_ClipYMax, const uint8_t ff_ClipYMin
    ) {
        #ifdef FLOODFILL_FAST_COMPRESS
            uint24_t compressed_size = stack_size;
            for (uint24_t i = 0; i < compressed_size; i++) {
                while (
                    ffcord_redundant(
                        #ifdef FLOODFILL_MALLOC_STACK
                            ff_stack,
                        #endif
                        i, replace, ff_ClipYMax, ff_ClipYMin
                    ) && i < compressed_size
                ) {
                    compressed_size--;
                    ff_stack[i] = ff_stack[compressed_size];
                }
            }
            #ifdef FLOODFILL_DEBUG
                printf("comp: %4u --> %4u\n", (uint32_t)stack_size, (uint32_t)compressed_size);
            #endif
            return compressed_size;
        #else
            uint24_t dst_index = 0;
            for (uint24_t src_index = 0; src_index < stack_size; src_index++) {
                if (ffcord_redundant(
                    #ifdef FLOODFILL_MALLOC_STACK
                        ff_stack,
                    #endif
                    src_index, replace, ff_ClipYMax, ff_ClipYMin
                )) {
                    continue;
                }
                ff_stack[dst_index] = ff_stack[src_index];
                dst_index++;
            }
            #ifdef FLOODFILL_DEBUG
                printf("comp: %4u --> %4u\n", (uint32_t)stack_size, (uint32_t)dst_index);
            #endif
            return dst_index;
        #endif
    }

    void gfy_FloodFill(uint24_t x, uint8_t y, const uint8_t color) {
        GFY_CLIPPING_ASSUMPTIONS();
        if (
            (int24_t)x < gfy_ClipXMin || (int24_t)x >= gfy_ClipXMax ||
            (int24_t)y < gfy_ClipYMin || (int24_t)y >= gfy_ClipYMax
        ) {
            return;
        }
        const uint8_t ff_ClipYMin = (uint8_t)gfy_ClipYMin;
        const uint8_t ff_ClipYMax = (uint8_t)gfy_ClipYMax;

        uint8_t* pixel = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + ((x * GFY_LCD_HEIGHT) + y);
        const uint8_t replace = *pixel;
        if (replace == color) {
            return; // Same color
        }

        #ifdef FLOODFILL_MALLOC_STACK
            uint24_t Maximum_FloodFill_Stack_Depth = FLOODFILL_STACK_SIZE;
            FloodFillCord* ff_stack = malloc(Maximum_FloodFill_Stack_Depth * sizeof(FloodFillCord));
            if (ff_stack == NULL) {
                // Tries to malloc half as much instead
                Maximum_FloodFill_Stack_Depth = FLOODFILL_STACK_SIZE / 2;
                ff_stack = malloc(Maximum_FloodFill_Stack_Depth * sizeof(FloodFillCord));
                if (ff_stack == NULL) {
                    return;
                }
            }
        #endif

        *pixel = color;

        uint24_t stack_size = 1;
        ff_stack[0] = (FloodFillCord){(int24_t)x, y};
        #ifdef FLOODFILL_DEBUG
            uint24_t max_s = 0;
            size_t pixels_written = 1;
        #endif
        do {
            #ifdef FLOODFILL_DEBUG
                if (stack_size > max_s) {
                    max_s = stack_size;
                }
            #endif

            stack_size--;
            const FloodFillCord ff_cord = ff_stack[stack_size];


            uint8_t* test = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (ff_cord.x * GFY_LCD_HEIGHT + ff_cord.y);
            test++; // Down 1
            if (
                ff_cord.y + 1 < ff_ClipYMax &&
                *test == replace
            ) {
                ff_stack[stack_size] = (FloodFillCord){
                    ff_cord.x, ff_cord.y + 1
                };
                stack_size++;
                *test = color;
                #ifdef FLOODFILL_DEBUG
                    pixels_written++;
                #endif
            }

            test += GFY_LCD_HEIGHT - 1; // Right 1, Up 1
            if (
                ff_cord.x + 1 < gfy_ClipXMax &&
                *test == replace
            ) {
                ff_stack[stack_size] = (FloodFillCord){
                    ff_cord.x + 1, ff_cord.y
                };
                stack_size++;
                *test = color;
                #ifdef FLOODFILL_DEBUG
                    pixels_written++;
                #endif
            }

            test -= GFY_LCD_HEIGHT * 2; // Left 2
            if (
                ff_cord.x - 1 >= gfy_ClipXMin &&
                *test == replace
            ) {
                ff_stack[stack_size] = (FloodFillCord){
                    ff_cord.x - 1, ff_cord.y
                };
                stack_size++;
                *test = color;
                #ifdef FLOODFILL_DEBUG
                    pixels_written++;
                #endif
            }

            test += GFY_LCD_HEIGHT - 1; // Right 1, Up 1
            if (
                ff_cord.y != 0 &&
                ff_cord.y - 1 >= ff_ClipYMin &&
                *test == replace
            ) {
                ff_stack[stack_size] = (FloodFillCord){
                    ff_cord.x, ff_cord.y - 1
                };
                stack_size++;
                *test = color;
                #ifdef FLOODFILL_DEBUG
                    pixels_written++;
                #endif
            }
            if (stack_size > Maximum_FloodFill_Stack_Depth - 4) {
                #ifdef FLOODFILL_DEBUG
                    printf("pixels_written: %5zu ", pixels_written);
                #endif
                stack_size = compress_stack(
                    #ifdef FLOODFILL_MALLOC_STACK
                        ff_stack,
                    #endif
                    stack_size, replace, ff_ClipYMax, ff_ClipYMin
                );
            }
        } while (
            stack_size > 0 &&
            stack_size <= Maximum_FloodFill_Stack_Depth - 4
        );
        #ifdef FLOODFILL_DEBUG
            printf("Maximum stack_size: %u pixels_written: %zu\n", (uint32_t)max_s, pixels_written);
            if (stack_size > Maximum_FloodFill_Stack_Depth - 4) {
                printf("Error: stack_size: %u\n", (uint32_t)stack_size);
            }
        #endif
        #ifdef FLOODFILL_MALLOC_STACK
            free(ff_stack);
            ff_stack = NULL;
        #endif
    }
#endif

//------------------------------------------------------------------------------
// v6 functions
//------------------------------------------------------------------------------

/* gfy_RLETSprite */

void gfy_RLETSprite(const gfy_rletsprite_t *sprite, const int24_t x, const int24_t y) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (
        /* Out of Bounds */
        x >= gfy_ClipXMax || y >= gfy_ClipYMax ||
        /* Degenerate */
        sprite->width == 0 || sprite->height == 0
    ) {
        return;
    }
    if (
        x >= gfy_ClipXMin &&
        y >= gfy_ClipYMin &&
        x + sprite->width <= gfy_ClipXMax &&
        y + sprite->height <= gfy_ClipYMax
    ) {
        // If fully inside the clipping region
        gfy_RLETSprite_NoClip(sprite, (uint24_t)x, (uint8_t)y);
        return;
    }

    // Naive clipping method
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (y + (GFY_LCD_HEIGHT * x));
    const uint24_t dst_jump = GFY_LCD_HEIGHT - sprite->width;
    for (uint8_t posY = 0; posY < sprite->height; posY++) {
        uint8_t posX = 0;
        if (
            x + posY >= gfy_ClipXMin &&
            x + posY < gfy_ClipXMax
        ) {
            while (posX < sprite->width) {
                const uint8_t jump_TP = *src_buf;
                posX += jump_TP;
                dst_buf += jump_TP;
                src_buf++;
                
                if (posX >= sprite->width) {
                    break;
                }

                const uint8_t len = *src_buf;
                src_buf++;

                for(uint8_t r = 0; r < len; r++) {
                    if (
                        y + posX >= gfy_ClipYMin &&
                        y + posX < gfy_ClipYMax
                    ) {
                        *dst_buf = *src_buf;
                    }
                    src_buf++;
                    posX++;
                    dst_buf++;
                }
            }
            dst_buf += dst_jump;
        } else {
            if (x + posY >= gfy_ClipXMax) {
                return;
            }
            // Fast forward through the decompression
            while (posX < sprite->width) {
                posX += *src_buf;
                src_buf++;
                
                if (posX >= sprite->width) {
                    break;
                }

                const uint8_t len = *src_buf;
                src_buf += len + 1;
                posX += len;
            }
            dst_buf += GFY_LCD_HEIGHT;
        }
    }
}

/* gfy_RLETSprite_NoClip */

void gfy_RLETSprite_NoClip(const gfy_rletsprite_t *sprite, const uint24_t x, const uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (y + (GFY_LCD_HEIGHT * x));
    const uint24_t dst_jump = GFY_LCD_HEIGHT - sprite->width;
    
    for (uint8_t posY = 0; posY < sprite->height; posY++) {
        uint8_t posX = 0;
        while (posX < sprite->width) {
            const uint8_t jump_TP = *src_buf;
            posX += jump_TP;
            dst_buf += jump_TP;
            src_buf++;
            
            if (posX >= sprite->width) {
                break;
            }

            const uint8_t len = *src_buf;
            src_buf++;
            posX += len;
            for(uint8_t r = 0; r < len; r++) {
                *dst_buf = *src_buf;
                src_buf++;
                dst_buf++;
            }
        }
        dst_buf += dst_jump;
    }
}

/* gfy_ConvertFromRLETSprite */

/** @todo test PortCE routine */
gfy_sprite_t *gfy_ConvertFromRLETSprite(const gfy_rletsprite_t *sprite_in, gfy_sprite_t *sprite_out) {
    #ifndef GRAPHY_STANDALONE
        return (gfy_sprite_t*)gfx_ConvertFromRLETSprite((const gfx_rletsprite_t*)sprite_in, (gfx_sprite_t*)sprite_out);
    #else
        sprite_out->width = sprite_in->width;
        sprite_out->height = sprite_in->height;
        const uint8_t* src_buf = sprite_in->data;
        uint8_t* dst_buf = sprite_out->data;
        const uint24_t dst_jump = sprite_in->width - 1;
        
        for (uint8_t posY = 0; posY < sprite_in->height; posY++) {
            uint8_t posX = 0;
            while (posX < sprite_in->width) {
                const uint8_t jump_TP = *src_buf;
                posX += jump_TP;
                memset(dst_buf, gfy_Transparent_Color, jump_TP);
                dst_buf += jump_TP;
                src_buf++;

                if (posX >= sprite_in->width) {
                    break;
                }

                const uint8_t len = *src_buf;
                src_buf++;
                posX += len;
                memcpy(dst_buf, src_buf, len);
                src_buf += len;
                dst_buf += len;
            }
            dst_buf -= dst_jump;
        }
        return sprite_out;
    #endif
}

/* gfy_ConvertToRLETSprite */

gfy_rletsprite_t *gfy_ConvertToRLETSprite(const gfy_sprite_t *sprite_in, gfy_rletsprite_t *sprite_out) {
    #ifndef GRAPHY_STANDALONE
        return (gfy_rletsprite_t*)gfx_ConvertToRLETSprite((const gfx_sprite_t*)sprite_in, (gfx_rletsprite_t*)sprite_out);
    #else
        sprite_out->width = sprite_in->width;
        sprite_out->height = sprite_in->height;

        const uint8_t* src_ptr = sprite_in->data;
        uint8_t* dst_ptr = sprite_out->data;

        for (uint8_t y = 0; y < sprite_in->height; y++) {
            uint8_t x = 0;
            
            while (x < sprite_in->width) {
                uint8_t transparent_run_length = 0;
                while (x < sprite_in->width && *src_ptr == gfy_Transparent_Color) {
                    transparent_run_length++;
                    x++;
                    src_ptr++;
                }
                *dst_ptr = transparent_run_length;
                dst_ptr++;

                if (x >= sprite_in->width) {
                    break;
                }
                
                uint8_t* const opaque_element = dst_ptr;
                dst_ptr++;
                uint8_t opaque_run_length = 0;
                while (x < sprite_in->width && *src_ptr != gfy_Transparent_Color) {
                    opaque_run_length++;
                    x++;
                    *dst_ptr = *src_ptr;
                    src_ptr++;
                    dst_ptr++;
                }
                *opaque_element = opaque_run_length;
            }
        }
        return sprite_out;
    #endif
}

/* gfy_ConvertToNewRLETSprite */

gfy_rletsprite_t *gfy_ConvertToNewRLETSprite(const gfy_sprite_t *sprite_in, void *(*malloc_routine)(size_t)) {
    #ifndef GRAPHY_STANDALONE
        return (gfy_rletsprite_t*)gfx_ConvertToNewRLETSprite((const gfx_sprite_t*)sprite_in, malloc_routine);
    #else

        size_t rlet_size = sizeof(gfy_sprite_t);
        const uint8_t* src_ptr = sprite_in->data;

        // Calculates rlet_size
        for (uint8_t y = 0; y < sprite_in->height; y++) {
            uint8_t x = 0;
            
            while (x < sprite_in->width) {
                while (x < sprite_in->width && *src_ptr == gfy_Transparent_Color) {
                    x++;
                    src_ptr++;
                }
                rlet_size++;

                if (x >= sprite_in->width) {
                    break;
                }
                
                rlet_size++;
                while (x < sprite_in->width && *src_ptr != gfy_Transparent_Color) {
                    x++;
                    src_ptr++;
                    rlet_size++;
                }
            }
        }

        gfy_rletsprite_t* sprite_out = (*malloc_routine)(rlet_size);
        if (sprite_out == NULL) {
            return NULL; // Allocation failure
        }
        return gfy_ConvertToRLETSprite(sprite_in, sprite_out);
    #endif
}

//------------------------------------------------------------------------------
// v7 functions
//------------------------------------------------------------------------------

/* gfy_RotateScaleSprite */

#ifndef GRAPHY_STANDALONE
gfy_sprite_t *gfy_RotateScaleSprite(
    const gfy_sprite_t *sprite_in,
    gfy_sprite_t *sprite_out,
    uint8_t angle,
    uint8_t scale
) {
    return (gfy_sprite_t*)gfx_RotateScaleSprite(
        (const gfx_sprite_t*)sprite_in,
        (gfx_sprite_t*)sprite_out,
        angle,
        scale
    );
}
#endif

/* gfy_RotatedScaledTransparentSprite_NoClip */



/* gfy_RotatedScaledSprite_NoClip */



//------------------------------------------------------------------------------
// v8 functions
//------------------------------------------------------------------------------

/* gfy_SetCharData */

#ifdef GRAPHY_STANDALONE
uint8_t *gfy_SetCharData(uint8_t index, const uint8_t *data) {
    return memcpy(&((uint8_t*)gfy_TextData)[index * 8], data, 8 * 8);
}
#else
uint8_t *gfy_SetCharData(uint8_t, const uint8_t*) __attribute__((alias("gfx_SetCharData")));
#endif

//------------------------------------------------------------------------------
// v9 functions
//------------------------------------------------------------------------------

/* gfy_Wait */

#ifndef GRAPHY_STANDALONE
void gfy_Wait(void) {
    return;
}
#else
void gfy_Wait(void) __attribute__((alias("gfx_Wait")));
#endif

//------------------------------------------------------------------------------
// v10 functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// v11 functions
//------------------------------------------------------------------------------

/* gfy_CopyRectangle */

void gfy_CopyRectangle(
    gfy_location_t src, gfy_location_t dst,
    uint24_t src_x, uint8_t src_y,
    uint24_t dst_x, uint8_t dst_y,
    uint24_t width, uint8_t height
) {
    const uint8_t* src_buf = (src == gfy_screen) ?
        (uint8_t*)RAM_ADDRESS(lcd_UpBase) : (uint8_t*)RAM_ADDRESS(lcd_LpBase);
    uint8_t* dst_buf = (dst == gfy_screen) ?
        (uint8_t*)RAM_ADDRESS(lcd_UpBase) : (uint8_t*)RAM_ADDRESS(lcd_LpBase);
    const bool buf_overlap = (src_buf == dst_buf) ? true : false;
    src_buf += src_y + (src_x * GFY_LCD_HEIGHT);
    dst_buf += dst_y + (dst_x * GFY_LCD_HEIGHT);
    const uint24_t jump = GFY_LCD_HEIGHT - height;
    if (buf_overlap == true) {
        for (uint24_t x_cord = 0; x_cord < width; x_cord++) {
            memmove(dst_buf, src_buf, height);
            src_buf += jump;
            dst_buf += jump;
        }
        return;
    }
    for (uint24_t x_cord = 0; x_cord < width; x_cord++) {
        memcpy(dst_buf, src_buf, height);
        src_buf += jump;
        dst_buf += jump;
    }
}

//------------------------------------------------------------------------------
// v12 functions
//------------------------------------------------------------------------------

/* gfy_internal_Ellipse */

static void gfy_internal_Ellipse_dual_point(
    int24_t x, int24_t y, int24_t xc, int24_t yc
) {
    gfy_SetPixel_RegionClip(x - xc, y - yc, gfy_Color);
    gfy_SetPixel_RegionClip(x + xc, y - yc, gfy_Color);
    gfy_SetPixel_RegionClip(x - xc, y + yc, gfy_Color);
    gfy_SetPixel_RegionClip(x + xc, y + yc, gfy_Color);
}

static void gfy_internal_Ellipse_dual_point_NoClip(
    int24_t x, int24_t y, int24_t xc, int24_t yc
) {
    gfy_SetPixel_NoClip(x - xc, y - yc, gfy_Color);
    gfy_SetPixel_NoClip(x + xc, y - yc, gfy_Color);
    gfy_SetPixel_NoClip(x - xc, y + yc, gfy_Color);
    gfy_SetPixel_NoClip(x + xc, y + yc, gfy_Color);
}

static void gfy_internal_Ellipse_dual_line(
    int24_t x, int24_t y, int24_t xc, int24_t yc
) {
    gfy_HorizLine(x - xc, y - yc, 2 * xc);
    gfy_HorizLine(x - xc, y + yc, 2 * xc);
}

static void gfy_internal_Ellipse_dual_line_NoClip(
    int24_t x, int24_t y, int24_t xc, int24_t yc
) {
    gfy_HorizLine_NoClip(x - xc, (uint8_t)(y - yc), 2 * xc);
    gfy_HorizLine_NoClip(x - xc, (uint8_t)(y + yc), 2 * xc);
}

// Derived from graphx.asm
static void gfy_internal_Ellipse(
    int24_t x, int24_t y, uint24_t a, uint24_t b,
    void (*plot_function)(int24_t, int24_t, int24_t, int24_t)
) {	
    int24_t a2 = a * a;
    int24_t fa2 = 4 * a2;
    if (a == 0 || b == 0) {
        return;
    }
    int24_t yc = b;
    int24_t sigma_add_1 = fa2 * (1 - b);
    int24_t b2 = b * b;
    int24_t fb2 = 4 * b2;
    int24_t xc = 0;
    int24_t sigma = 2 * b2 + a2 * (1 - 2 * b);
    int24_t sigma_add_2 = fb2 * (1 - a);

    while(b2 * xc <= a2 * yc) { /* .main_loop1 */
        (*plot_function)(x, y, xc, yc);
        if (sigma >= 0) {
            sigma += sigma_add_1;
            sigma_add_1 += fa2;
            yc--;
        }

        sigma += b2 * (4 * xc + 6);
        xc++;
    }

    xc = a;
    yc = 0;
    sigma = 2 * a2 + b2 * (1 - 2 * a);

    while(a2 * yc <= b2 * xc) {
        (*plot_function)(x, y, xc, yc);
        if (sigma >= 0) {
            sigma += sigma_add_2;
            sigma_add_2 += fb2;
            xc--;
        }

        sigma += a2 * (4 * yc + 6);
        yc++;
    } 
}

/* gfy_Ellipse */

void gfy_Ellipse(int24_t x, int24_t y, uint24_t a, uint24_t b) {
    gfy_internal_Ellipse(
        x, y, a, b,
        gfy_internal_Ellipse_dual_point
    );
}

/* gfy_Ellipse_NoClip */

void gfy_Ellipse_NoClip(uint24_t x, uint24_t y, uint8_t a, uint8_t b) {
    gfy_internal_Ellipse(
        (int24_t)x, (int24_t)y, (uint24_t)a, (uint24_t)b,
        gfy_internal_Ellipse_dual_point_NoClip
    );
}

/* gfy_FillEllipse */

void gfy_FillEllipse(int24_t x, int24_t y, uint24_t a, uint24_t b) {
    gfy_internal_Ellipse(
        x, y, a, b,
        gfy_internal_Ellipse_dual_line
    );
}

/* gfy_FillEllipse_NoClip */

void gfy_FillEllipse_NoClip(uint24_t x, uint24_t y, uint8_t a, uint8_t b) {
    gfy_internal_Ellipse(
        (int24_t)x, (int24_t)y, (uint24_t)a, (uint24_t)b,
        gfy_internal_Ellipse_dual_line_NoClip
    );
}

//------------------------------------------------------------------------------
// compatibility (transposed) routines
//------------------------------------------------------------------------------

/**
 * @brief These routines will take a gfx_sprite_t instead of a gfy_sprite_t.
 * However, these routines are much slower since the GraphX sprites cannot
 * be drawn with a memcpy in Column-Major mode.
 */

/* gfy_Transpose_Sprite */

 void gfy_Transpose_Sprite(const gfx_sprite_t *restrict sprite, int24_t x, int24_t y) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (
        x >= gfy_ClipXMax || y >= gfy_ClipYMax ||
        sprite->width == 0 || sprite->height == 0 ||
        x + sprite->width < gfy_ClipXMin ||
        y + sprite->height < gfy_ClipYMin
    ) {
        return;
    }
    if (
        x >= gfy_ClipXMin &&
        y >= gfy_ClipYMin &&
        x + sprite->width <= gfy_ClipXMax &&
        y + sprite->height <= gfy_ClipYMax
    ) {
        gfy_Sprite_NoClip(sprite, (uint24_t)x, (uint8_t)y);
        return;
    }
    const uint8_t min_clipX = (x < gfy_ClipXMin) ? (gfy_ClipXMin - x) : 0;
    const uint8_t min_clipY = (y < gfy_ClipYMin) ? (gfy_ClipYMin - y) : 0;
    const uint8_t max_clipX = ((x + sprite->width) > gfy_ClipXMax) ? ((x + sprite->width) - gfy_ClipXMax) : 0;
    const uint8_t max_clipY = ((y + sprite->height) > gfy_ClipYMax) ? ((y + sprite->height) - gfy_ClipYMax) : 0;

    uint8_t sizeX = sprite->width - (min_clipX + max_clipX);
    uint8_t sizeY = sprite->height - (min_clipY + max_clipY);
    const uint8_t* src_buf = sprite->data + min_clipX + (min_clipY * sprite->width);
    const uint8_t clip_jumpX = min_clipX + max_clipX;

    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (y + min_clipY) + ((x + min_clipX) * GFY_LCD_HEIGHT);
    const uint24_t dst_jump = (GFY_LCD_HEIGHT * (sprite->width - clip_jumpX)) - 1;
    
    for (uint8_t y_cord = 0; y_cord < sizeY; y_cord++) {
        for (uint8_t x_cord = 0; x_cord < sizeX; x_cord++) {
            *dst_buf = *src_buf;
            src_buf++;
            dst_buf += GFY_LCD_HEIGHT;
        }
        src_buf += clip_jumpX;
        dst_buf -= dst_jump;
    }
}

/* gfy_Transpose_TransparentSprite */

void gfy_Transpose_TransparentSprite(const gfx_sprite_t *restrict sprite, int24_t x, int24_t y) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (
        x >= gfy_ClipXMax || y >= gfy_ClipYMax ||
        sprite->width == 0 || sprite->height == 0 ||
        x + sprite->width < gfy_ClipXMin ||
        y + sprite->height < gfy_ClipYMin
    ) {
        return;
    }
    if (
        x >= gfy_ClipXMin &&
        y >= gfy_ClipYMin &&
        x + sprite->width <= gfy_ClipXMax &&
        y + sprite->height <= gfy_ClipYMax
    ) {
        gfy_TransparentSprite_NoClip(sprite, (uint24_t)x, (uint8_t)y);
        return;
    }
    const uint8_t min_clipX = (x < gfy_ClipXMin) ? (gfy_ClipXMin - x) : 0;
    const uint8_t min_clipY = (y < gfy_ClipYMin) ? (gfy_ClipYMin - y) : 0;
    const uint8_t max_clipX = ((x + sprite->width) > gfy_ClipXMax) ? ((x + sprite->width) - gfy_ClipXMax) : 0;
    const uint8_t max_clipY = ((y + sprite->height) > gfy_ClipYMax) ? ((y + sprite->height) - gfy_ClipYMax) : 0;

    uint8_t sizeX = sprite->width - min_clipX - max_clipX;
    uint8_t sizeY = sprite->height - min_clipY - max_clipY;
    const uint8_t* src_buf = sprite->data + min_clipX + (min_clipY * sprite->width);
    const uint8_t clip_jumpX = min_clipX + max_clipX;

    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (y + min_clipY) + ((x + min_clipX) * GFY_LCD_HEIGHT);
    const uint24_t dst_jump = (GFY_LCD_HEIGHT * (sprite->width - clip_jumpX)) - 1;

    for (uint8_t y_cord = 0; y_cord < sizeY; y_cord++) {
        for (uint8_t x_cord = 0; x_cord < sizeX; x_cord++) {
            *dst_buf = (*src_buf != gfy_Transparent_Color) ? *src_buf : *dst_buf;
            src_buf++;
            dst_buf += GFY_LCD_HEIGHT;
        }
        src_buf += clip_jumpX;
        dst_buf -= dst_jump;
    }
}

/* gfy_Transpose_Sprite_NoClip */

void gfy_Transpose_Sprite_NoClip(const gfx_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    const uint24_t dst_jump = (GFY_LCD_HEIGHT * sprite->width) - 1;
    
    for (uint8_t y_cord = 0; y_cord < sprite->height; y_cord++) {
        for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
            *dst_buf = *src_buf;
            src_buf++;
            dst_buf += GFY_LCD_HEIGHT;
        }
        dst_buf -= dst_jump;
    }
}

/* gfy_Transpose_TransparentSprite_NoClip */

void gfy_Transpose_TransparentSprite_NoClip(const gfx_sprite_t *restrict sprite, uint24_t x, uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    const uint24_t dst_jump = (GFY_LCD_HEIGHT * sprite->width) - 1;
    
    for (uint8_t y_cord = 0; y_cord < sprite->height; y_cord++) {
        for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
            *dst_buf = (*src_buf != gfy_Transparent_Color) ? *src_buf : *dst_buf;
            src_buf++;
            dst_buf += GFY_LCD_HEIGHT;
        }
        dst_buf -= dst_jump;
    }
}

/* gfy_Transpose_GetSprite */

gfx_sprite_t *gfy_Transpose_GetSprite(gfx_sprite_t *sprite_buffer, int24_t x, int24_t y) {
    uint8_t* dst_buf = sprite_buffer->data;
    const uint8_t* src_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    const uint24_t src_jump = (GFY_LCD_HEIGHT * sprite_buffer->width) - 1;
    
    for (uint8_t y_cord = 0; y_cord < sprite_buffer->height; y_cord++) {
        for (uint8_t x_cord = 0; x_cord < sprite_buffer->width; x_cord++) {
            *dst_buf = *src_buf;
            dst_buf++;
            src_buf += GFY_LCD_HEIGHT;
        }
        src_buf -= src_jump;
    }
    return sprite_buffer;
}

/* gfy_Transpose_ScaledSprite_NoClip */

void gfy_Transpose_ScaledSprite_NoClip(
    const gfx_sprite_t *sprite,
    const uint24_t x,
    const uint8_t y,
    const uint8_t width_scale,
    const uint8_t height_scale
) {
    if (width_scale == 0 || height_scale == 0) {
        return;
    }
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    const uint24_t dst_jumpY = (GFY_LCD_HEIGHT * (sprite->width * width_scale)) - 1;
    const uint24_t dst_jumpX = GFY_LCD_HEIGHT * width_scale;
        
    for (uint8_t y_cord = 0; y_cord < sprite->height; y_cord++) {
        const uint8_t* src_buf_line = src_buf;
        for (uint8_t v = 0; v < height_scale; v++) {
            src_buf = src_buf_line;
            for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
                *dst_buf = *src_buf;
                dst_buf += dst_jumpX;
                src_buf++;
            }
            dst_buf -= dst_jumpY;
        }
    }
    
    if (width_scale == 1) {
        return;
    }

    // memcpy columns
    uint8_t* dst_col = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    const uint24_t copySize = sprite->height * height_scale; // This could probably be uint8_t
    for (uint8_t col = 0; col < sprite->width; col++) {
        const uint8_t* const src_col = (uint8_t*)dst_col;
        dst_col += GFY_LCD_HEIGHT;
        for (uint8_t x_cord = 1; x_cord < width_scale; x_cord++) {
            memcpy(dst_col, src_col, copySize);
            dst_col += GFY_LCD_HEIGHT;
        }
    }
}

/* gfy_Transpose_ScaledTransparentSprite_NoClip */

void gfy_Transpose_ScaledTransparentSprite_NoClip(
    const gfx_sprite_t *sprite,
    const uint24_t x,
    const uint8_t y,
    const uint8_t width_scale,
    const uint8_t height_scale
) {
    if (width_scale == 0 || height_scale == 0) {
        return;
    }
    const uint8_t* src_buf = sprite->data;
    uint8_t* buf_start = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + y + (x * GFY_LCD_HEIGHT);
    uint8_t* dst_buf = buf_start;
    const uint24_t dst_jumpY = (GFY_LCD_HEIGHT * (sprite->width * width_scale)) - 1;
    const uint24_t dst_jumpX = GFY_LCD_HEIGHT * width_scale;
    
    for (uint8_t y_cord = 0; y_cord < sprite->height; y_cord++) {
        const uint8_t* src_buf_line = src_buf;
        for (uint8_t v = 0; v < height_scale; v++) {
            src_buf = src_buf_line;
            for (uint8_t x_cord = 0; x_cord < sprite->width; x_cord++) {
                const uint8_t color = *src_buf;
                src_buf++;
                if (color == gfy_Transparent_Color) {
                    dst_buf += dst_jumpX;
                    continue;
                }
                for (uint8_t u = 0; u < width_scale; u++) {
                    *dst_buf = color;
                    dst_buf += GFY_LCD_HEIGHT;
                }
            }
            dst_buf -= dst_jumpY;
        }
    }
}

/* gfy_Transpose_RLETSprite */

/**
 * @todo Move the decompression fast-forward loop outside the drawing loop
 */
void gfy_Transpose_RLETSprite(const gfx_rletsprite_t *sprite, const int24_t x, const int24_t y) {
    GFY_CLIPPING_ASSUMPTIONS();
    if (
        /* Out of Bounds */
        x >= gfy_ClipXMax || y >= gfy_ClipYMax ||
        /* Degenerate */
        sprite->width == 0 || sprite->height == 0
    ) {
        return;
    }
    if (
        x >= gfy_ClipXMin &&
        y >= gfy_ClipYMin &&
        x + sprite->width <= gfy_ClipXMax &&
        y + sprite->height <= gfy_ClipYMax
    ) {
        // If fully inside the clipping region
        gfy_RLETSprite_NoClip(sprite, (uint24_t)x, (uint8_t)y);
        return;
    }

    // Naive clipping method
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (y + (GFY_LCD_HEIGHT * x));
    const uint24_t dst_jump = (sprite->width * GFY_LCD_HEIGHT) - 1;
    for (uint8_t posY = 0; posY < sprite->height; posY++) {
        uint8_t posX = 0;
        if (
            y + posY >= gfy_ClipYMin &&
            y + posY < gfy_ClipYMax
        ) {
            while (posX < sprite->width) {
                const uint8_t jump_TP = *src_buf;
                posX += jump_TP;
                dst_buf += jump_TP * GFY_LCD_HEIGHT;
                src_buf++;
                
                if (posX >= sprite->width) {
                    break;
                }

                const uint8_t len = *src_buf;
                src_buf++;

                for(uint8_t r = 0; r < len; r++) {
                    if (
                        x + posX >= gfy_ClipXMin &&
                        x + posX < gfy_ClipXMax
                    ) {
                        *dst_buf = *src_buf;
                    }
                    src_buf++;
                    posX++;
                    dst_buf += GFY_LCD_HEIGHT;
                }
            }
            dst_buf -= dst_jump;
        } else {
            if (y + posY >= gfy_ClipYMax) {
                return;
            }
            // Fast forward through the decompression
            while (posX < sprite->width) {
                posX += *src_buf;
                src_buf++;
                
                if (posX >= sprite->width) {
                    break;
                }

                const uint8_t len = *src_buf;
                src_buf += len + 1;
                posX += len;
            }
            dst_buf++;
        }
    }
}

/* gfy_Transpose_RLETSprite_NoClip */

void gfy_Transpose_RLETSprite_NoClip(const gfx_rletsprite_t *sprite, const uint24_t x, const uint8_t y) {
    const uint8_t* src_buf = sprite->data;
    uint8_t* dst_buf = (uint8_t*)RAM_ADDRESS(gfy_CurrentBuffer) + (y + (GFY_LCD_HEIGHT * x));
    const uint24_t dst_jump = (sprite->width * GFY_LCD_HEIGHT) - 1;
    
    for (uint8_t posY = 0; posY < sprite->height; posY++) {
        uint8_t posX = 0;
        while (posX < sprite->width) {
            const uint8_t jump_TP = *src_buf;
            posX += jump_TP;
            dst_buf += jump_TP * GFY_LCD_HEIGHT;
            src_buf++;
            
            if (posX >= sprite->width) {
                break;
            }

            const uint8_t len = *src_buf;
            src_buf++;
            posX += len;
            for(uint8_t r = 0; r < len; r++) {
                *dst_buf = *src_buf;
                src_buf++;
                dst_buf += GFY_LCD_HEIGHT;
            }
        }
        dst_buf -= dst_jump;
    }
}
