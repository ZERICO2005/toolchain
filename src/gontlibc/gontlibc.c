#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "gontlibc.h"
#include <graphy.h>

#define CurrentBuffer (*(uint8_t * volatile *)0xE30014)

extern int TextXMin;
extern int TextXMax;
extern int TextX;

extern uint8_t TextYMin;
extern uint8_t TextYMax;
extern uint8_t TextY;

#define TextYMin ((int)TextYMin)
#define TextYMax ((int)TextYMax)
#define TextY ((int)TextY)

extern uint8_t TextStraightBackgroundColor;
extern uint8_t TextStraightForegroundColor;

extern uint8_t TextTransparentMode;
extern uint8_t TextNewlineControl;
extern const char* TextLastCharacterRead;
extern size_t TempCharactersLeft;
extern uint8_t TextAlternateStopCode;
extern uint8_t TextFirstPrintableCodePoint;
extern uint8_t TextNewLineCode;

extern uint8_t* root;
extern gontlib_font_t conf;

static bool is_newline(char c) {
    return (c == TextNewlineControl);
}

static bool is_printable(char c) {
    /* TEMP */ if (c == '\0') {
        return false;
    }
    if (c < TextFirstPrintableCodePoint) {
        return false;
    }
    if (!gontlib_ValidateCodePoint(c)) {
        return false;
    }
    return true;
}

unsigned int gontlib_DrawStringL(char const *__restrict str, size_t length_remaining) {
    if (*str == '\0') {
        goto finish;
    }

    const bool auto_wrap = (TextNewlineControl & FONTLIB_ENABLE_AUTO_WRAP);

    uint8_t const *__restrict const width_table_ptr = (uint16_t)conf.widths_table + root;
    for (;;) {
        if (length_remaining == 0) {
            goto finish;
        }
        char c = *str++;
        length_remaining--;
        if (!is_printable(c)) {
            goto finish;
        }
        if (is_newline(c)) {
            if (gontlib_Newline()) {
                // text window is full
                goto finish;
            }
        } else {
            const uint8_t width = width_table_ptr[(unsigned char)c];
            if (TextX + width > TextXMax) {
                if (!auto_wrap) {
                    // can't print character
                    goto finish;
                }
                if (gontlib_Newline()) {
                    // text window is full
                    goto finish;
                }
            }
            gontlib_DrawGlyph(c);
        }
    }
finish:
    TextLastCharacterRead = str;
    TempCharactersLeft = length_remaining;
    return TextX;
}

unsigned int gontlib_DrawString(const char *str) {
    return gontlib_DrawStringL(str, SIZE_MAX);
}

// renders row-major fonts
unsigned int gontlib_DrawGlyph(unsigned char c) {
    if (!gontlib_ValidateCodePoint(c)) {
        return TextX;
    }

    c -= conf.first_glyph;

    uint8_t const *__restrict const width_table_ptr = (uint16_t)conf.widths_table + root;
    const uint8_t width = width_table_ptr[c];

    const uint8_t space_above = conf.space_above;
    const uint8_t space_below = conf.space_below;
    const uint8_t height = conf.height;

    const uint8_t bg_color = TextStraightBackgroundColor;
    const uint8_t fg_color = TextStraightForegroundColor;
    const bool is_transparent = (TextTransparentMode != 0);

    int x_pos = TextX;
    int y_pos = TextY;
    x_pos -= conf.italic_space_adjust;
    uint8_t *__restrict const buf = (uint8_t *__restrict)(CurrentBuffer + (x_pos * GFY_LCD_HEIGHT) + y_pos);

    uint16_t const *__restrict const font_table_ptr = (uint16_t)conf.bitmaps + (uint16_t *__restrict)root;
    uint8_t const *__restrict src = *font_table_ptr + (uint8_t *__restrict)root;

    #if 0
        const uint8_t font_jump = ((uint8_t)(width - 1) >> 3) + 1;
    #else
        const uint8_t font_jump = 1;
    #endif

    TextX += (width - conf.italic_space_adjust);

    gfy_Wait();

    if (is_transparent) {
        uint8_t *__restrict dst = buf;
        // const size_t line_jump = GFY_LCD_HEIGHT - height;
        const size_t line_jump = 1 - (GFY_LCD_HEIGHT * width);
        dst += space_above;

        for (uint8_t y = 0; y < height; y++) {
            uint24_t HL = *(uint24_t const *__restrict)src;
            for (uint8_t x = 0; x < width; x++) {
                if (HL & 0x800000) {
                    *dst = fg_color;
                }
                HL <<= 1;
                dst += GFY_LCD_HEIGHT;
            }
            src += font_jump;
            dst += line_jump;
        }
        return TextX;
    }
    /* opaque */ {
        uint8_t *__restrict dst = buf;
        // const size_t line_jump = GFY_LCD_HEIGHT - height;
        const size_t line_jump = 1 - (GFY_LCD_HEIGHT * width);
        dst += space_above;
        for (uint8_t y = 0; y < height; y++) {
            uint24_t HL = *(uint24_t const *__restrict)src;
            for (uint8_t x = 0; x < width; x++) {
                if (HL & 0x800000) {
                    *dst = fg_color;
                } else {
                    *dst = bg_color;
                }
                HL <<= 1;
                dst += GFY_LCD_HEIGHT;
            }
            src += font_jump;
            dst += line_jump;
        }
    }
    /* draw empty lines above */ {
        uint8_t *__restrict above = buf;
        for (uint8_t x = 0; x < width; x++) {
            memset(above, bg_color, space_above);
            above += GFY_LCD_HEIGHT;
        }
    }
    /* draw empty lines below */ {
        uint8_t *__restrict below = buf + space_above + height;
        for (uint8_t x = 0; x < width; x++) {
            memset(below, bg_color, space_below);
            below += GFY_LCD_HEIGHT;
        }
    }
    return TextX;
}

void gontlib_ScrollWindowUp(void) {
    const size_t width = gontlib_GetWindowWidth();
    const uint8_t height = gontlib_GetCurrentFontHeight();
    int lines = (TextYMax - TextYMin - height);
    if (lines <= 0) {
        return;
    }
    if (lines - (int)height < 0) {
        return;
    }
    const size_t line_count = (size_t)(lines);
    const size_t x_pos = (size_t)TextXMin;
    const uint8_t y_pos = (uint8_t)TextYMin;

    uint8_t const * src = CurrentBuffer + (x_pos * GFY_LCD_HEIGHT) + y_pos;
    uint8_t * dst = (uint8_t*)src + height;
    gfy_Wait();
    for (size_t i = 0; i < width; i++) {
        memmove(dst, src, line_count);
        dst += GFY_LCD_HEIGHT;
        src += GFY_LCD_HEIGHT;
    }
}

void gontlib_ScrollWindowDown(void) {
    const size_t width = gontlib_GetWindowWidth();
    const uint8_t height = gontlib_GetCurrentFontHeight();
    int lines = (TextYMax - TextYMin - height);
    if (lines <= 0) {
        return;
    }
    if (lines - (int)height < 0) {
        return;
    }
    const size_t line_count = (size_t)(lines);
    const size_t x_pos = (size_t)TextXMin;
    const uint8_t y_pos = (uint8_t)TextYMin;

    uint8_t * dst = CurrentBuffer + (x_pos * GFY_LCD_HEIGHT) + y_pos;
    uint8_t const * src = (uint8_t const *)dst + height;
    gfy_Wait();
    for (size_t i = 0; i < width; i++) {
        memmove(dst, src, line_count);
        dst += GFY_LCD_HEIGHT;
        src += GFY_LCD_HEIGHT;
    }
}
