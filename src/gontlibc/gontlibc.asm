;-------------------------------------------------------------------------------
include '../include/library.inc'
include '../include/include_library.inc'
;-------------------------------------------------------------------------------

library GONTLIBC, 0

;-------------------------------------------------------------------------------
; Dependencies
;-------------------------------------------------------------------------------
include_library '../graphy/graphy.asm'

;-------------------------------------------------------------------------------
; v1 functions
;-------------------------------------------------------------------------------
	export gontlib_SetWindow
	export gontlib_SetWindowFullScreen
	export gontlib_GetWindowXMin
	export gontlib_GetWindowYMin
	export gontlib_GetWindowWidth
	export gontlib_GetWindowHeight
	export gontlib_SetCursorPosition
	export gontlib_GetCursorX
	export gontlib_GetCursorY
	export gontlib_ShiftCursorPosition
	export gontlib_SetFont
	export gontlib_SetForegroundColor
	export gontlib_SetBackgroundColor
	export gontlib_SetColors
	export gontlib_GetForegroundColor
	export gontlib_GetBackgroundColor
	export gontlib_SetTransparency
	export gontlib_GetTransparency
	export gontlib_SetLineSpacing
	export gontlib_GetSpaceAbove
	export gontlib_GetSpaceBelow
	export gontlib_SetItalicSpacingAdjustment
	export gontlib_GetItalicSpacingAdjustment
	export gontlib_GetCurrentFontHeight
	export gontlib_ValidateCodePoint
	export gontlib_GetTotalGlyphs
	export gontlib_GetFirstGlyph
	export gontlib_SetNewlineCode
	export gontlib_GetNewlineCode
	export gontlib_SetAlternateStopCode
	export gontlib_GetAlternateStopCode
	export gontlib_SetFirstPrintableCodePoint
	export gontlib_GetFirstPrintableCodePoint
	export gontlib_SetDrawIntCodePoints
	export gontlib_GetDrawIntMinus
	export gontlib_GetDrawIntZero
	export gontlib_GetGlyphWidth
	export gontlib_GetStringWidth
	export gontlib_GetStringWidthL
	export gontlib_GetLastCharacterRead
	export gontlib_DrawGlyph
	export gontlib_DrawString
	export gontlib_DrawStringL
	export gontlib_DrawInt
	export gontlib_DrawUInt
	export gontlib_ClearEOL
	export gontlib_ClearWindow
	export gontlib_Newline
	export gontlib_SetNewlineOptions
	export gontlib_GetNewlineOptions
;-------------------------------------------------------------------------------
; v1 font pack functions
;-------------------------------------------------------------------------------
	export gontlib_GetFontPackName
	export gontlib_GetFontByIndex
	export gontlib_GetFontByIndexRaw
	export gontlib_GetFontByStyle
	export gontlib_GetFontByStyleRaw
;-------------------------------------------------------------------------------
; v2 functions
;-------------------------------------------------------------------------------
	export gontlib_ScrollWindowDown
	export gontlib_ScrollWindowUp
	export gontlib_Home
	export gontlib_HomeUp


;-------------------------------------------------------------------------------
CurrentBuffer	:= ti.mpLcdLpbase
TEXT_FG_COLOR	:= 0
TEXT_BG_COLOR	:= 255
local2		:= -9
local1		:= -6
local0		:= -3
arg00		:= 0
arg0		:= 3
arg1		:= 6
arg2		:= 9
arg3		:= 12
arg4		:= 15
arg5		:= 18
arg6		:= 21
chFirstPrintingCode := $10
chNewline	:= $0A
bEnableAutoWrap	:= 0
mEnableAutoWrap	:= 1 shl bEnableAutoWrap
bAutoClearToEOL	:= 1
mAutoClearToEOL	:= 1 shl bAutoClearToEOL
bPreclearNewline := 2
mPreclearNewline := 1 shl bPreclearNewline
bWasNewline	:= 7
mWasNewline	:= 1 shl bWasNewline
bAutoScroll	:= 3
mAutoScroll	:= 1 shl bAutoScroll


;-------------------------------------------------------------------------------
; Declare the structure of a raw font
struc strucFont
	label .: 15
	.version			rb	1
	.height				rb	1
	.totalGlyphs			rb	1
	.firstGlyph			rb	1
	.widthsTablePtr			rl	1
	.bitmapsTablePtr		rl	1
	.italicSpaceAdjust		rb	1
	.spaceAbove			rb	1
	.spaceBelow			rb	1
	.weight				rb	1
	.style				rb	1
end struc
virtual at 0
strucFont strucFont
end virtual
strucFont.fontPropertiesSize := strucFont.italicSpaceAdjust
struc strucFontPackHeader
	label .: 13
	.identifier			rb	8
	.metadatOffset			rl	1
	.fontCount			rb	1
	.fontList			rl	1
end struc
virtual at 0
strucFontPackHeader strucFontPackHeader
end virtual


;-------------------------------------------------------------------------------
macro mIsHLLessThanDE?
	or	a, a
	sbc	hl, de
	add	hl, hl
	jp	po, $+5
	ccf
end macro
macro mIsHLLessThanBC?
	or	a, a
	sbc	hl, bc
	add	hl, hl
	jp	po, $+5
	ccf
end macro
macro s8 op, imm
	local i
	i = imm
	assert i >= -128 & i < 128
	op, i
end macro

;-------------------------------------------------------------------------------
macro setSmcBytes name*
	local addr, data
	postpone
		virtual at addr
			irpv each, name
				if % = 1
					db %%
				end if
				assert each >= addr + 1 + 2*%%
				dw each - $ - 2
			end irpv
			load data: $-$$ from $$
		end virtual
	end postpone

	call	_SetSmcBytes
addr	db	data
end macro

macro setSmcBytesFast name*
	local temp, list
	postpone
		temp equ each
		irpv each, name
			temp equ temp, each
		end irpv
		list equ temp
	end postpone

	pop	de			; de = return vetor
	ex	(sp), hl		; l = byte
	ld	a, l			; a = byte
	match expand, list
		iterate expand
			if % = 1
				ld	hl, each
				ld	c, (hl)
				ld	(hl), a
			else
				ld	(each), a
			end if
		end iterate
	end match
	ld	a, c			; a = old byte
	ex	de, hl			; hl = return vector
	jp	(hl)
end macro

macro smcByte name*, addr: $-1
	local link
	link := addr
	name equ link
end macro

;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
gontlib_SetWindowFullScreen: ; COPIED_FROM_FONTLIBC
; Sets the bounds of the box all text will appear in to be the full screen
; Arguments:
;  None
; Returns:
;  Nothing
	ld	hl, _TextDefaultWindow
	ld	de, _TextXMin
	jp	ti.Mov8b


;-------------------------------------------------------------------------------
gontlib_SetWindow: ; COPIED_FROM_FONTLIBC
; Sets the bounds of the box all text will appear in
; Arguments:
;  arg0: X min
;  arg1: Y min
;  arg2: width
;  arg3: height
; Returns:
;  Nothing
	pop	de			; de = return vector
	pop	hl			; hl = xmin
	ld	(_TextXMin), hl
	pop	bc			; c = ymin
	ld	a, c			; a = ymin
	ld	(_TextYMin), a
	pop	bc			; bc = width
	add	hl, bc			; hl = xmin + width = xmax
	ld	(_TextXMax), hl
	ex	(sp), hl		; l = height
	add	a, l			; a = ymin + height = ymax
	ld	(_TextYMax), a
	push	hl
	push	hl
	push	hl			; sp -> arg0
	ex	de, hl			; hl = return vector
	jp	(hl)


;-------------------------------------------------------------------------------
gontlib_GetWindowXMin: ; COPIED_FROM_FONTLIBC
; Returns the starting column of the current text window
; Arguments:
;  None
; Returns:
;  Data
	ld	hl, (_TextXMin)
	ret


;-------------------------------------------------------------------------------
gontlib_GetWindowYMin: ; COPIED_FROM_FONTLIBC
; Returns the starting row of the current text window
; Arguments:
;  None
; Returns:
;  Data
	ld	a, (_TextYMin)
	ret


;-------------------------------------------------------------------------------
gontlib_GetWindowWidth: ; COPIED_FROM_FONTLIBC
; Returns the width of the current text window
; Arguments:
;  None
; Returns:
;  Data
; Should preserve DE, as window scrolling needs DE preserved
	ld	hl, (_TextXMax)
	ld	bc, (_TextXMin)
	or	a, a
	sbc	hl, bc
	ret


;-------------------------------------------------------------------------------
gontlib_GetWindowHeight: ; COPIED_FROM_FONTLIBC
; Returns the height of the current text window
; Arguments:
;  None
; Returns:
;  Data
	ld	a, (_TextYMax)
	ld	hl, _TextYMin
	sub	a, (hl)
	ret


;-------------------------------------------------------------------------------
gontlib_SetCursorPosition: ; COPIED_FROM_FONTLIBC
; Sets the cursor position for text drawing
; Arguments:
;  arg0: X
;  arg1: Y
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	de, _TextX
	ld	bc, 4
	ldir
	ret


;-------------------------------------------------------------------------------
gontlib_GetCursorX: ; COPIED_FROM_FONTLIBC
; Gets the cursor column
; Arguments:
;  None
; Returns:
;  Column
	ld	hl, (_TextX)
	ret


;-------------------------------------------------------------------------------
gontlib_GetCursorY: ; COPIED_FROM_FONTLIBC
; Gets the cursor row
; Arguments:
;  None
; Returns:
;  Row
	ld	a, (_TextY)
	ret


;-------------------------------------------------------------------------------
gontlib_ShiftCursorPosition: ; COPIED_FROM_FONTLIBC
; Shifts the cursor position by a given signed delta.
; By limiting x to 0xFFFF and y to 0xFF, the cursor can't get far enough out of
; VRAM to cause corruption of any other regions of memory.
; Arguments:
;  arg0: delta X
;  arg1: delta Y
; Returns:
;  Nothing
	pop	de			; de = return vector
; Shift x
	pop	bc			; bc = dx
	ld	hl, (_TextX)		; hl = x
	add.s	hl, bc			; hl = (x + dx) & 0xFFFF
	ld	(_TextX), hl		; x = x + dx
; Shift y
	pop	bc			; bc = dy
	ld	hl, _TextY
	ld	a, (hl)			; a = y
	add	a, c			; a = (y + dy) & 0xFF
	ld	(hl), a			; y = (y + dy) & 0xFF
; Finish
	push	hl
	push	hl			; sp -> arg0
	ex	de, hl			; hl = return vector
	jp	(hl)


;-------------------------------------------------------------------------------
gontlib_HomeUp: ; COPIED_FROM_FONTLIBC
; Moves the cursor to the upper left corner of the text window
; Arguments:
;  None
; Returns:
;  Nothing
	ld	a, (_TextYMin)
	ld	(_TextY), a
; Fall through to gontlib_Home
assert $ = gontlib_Home


;-------------------------------------------------------------------------------
gontlib_Home: ; COPIED_FROM_FONTLIBC
; Moves the cursor back to the start of the current line
; Arguments:
;  None
; Returns:
;  Nothing
	ld	hl, (_TextXMin)
	ld	(_TextX), hl
	ret


;-------------------------------------------------------------------------------
gontlib_SetFont: ; COPIED_FROM_FONTLIBC
; Sets the current font to the data at the pointer given
; Arguments:
;  arg0: Pointer to font
;  arg1: Load flags
; Returns:
;  bool:
;     - true if font loaded successfully
;     - false on failure (invalid font, or you tried to use the version byte)
	ld	hl, arg0
	add	hl, sp
	ld	hl, (hl)
; Verify version byte is zero like it's supposed to be
; The literal only reason there's any validation here at all is to
; enforce keeping the version byte reserved.
	xor	a, a
	cp	a, (hl)
	ret	nz
; Load font data
	ld	(_CurrentFontRoot), hl
	push	hl
	ld	de, _CurrentFontProperties
	ld	bc, strucFont.fontPropertiesSize
	ldir
	pop	bc
	ld	iy, _CurrentFontProperties
; Verify height at least looks semi-reasonable
	or	a, (iy + strucFont.height)
	ret	z			; Also unreasonable: a zero-height font
	rla
	jr	c, .false		; font height >= 128
	ld	a, 63
	cp	a, (iy + strucFont.spaceAbove)
	jr	c, .false
	cp	a, (iy + strucFont.spaceBelow)
	jr	c, .false
.validateOffsets:
; Now convert offsets into actual pointers
; Validate that offset is at least semi-reasonable
	xor	a, a
	ld	de, $ff00		; Maximum reasonable font data size
	ld	hl, (iy + strucFont.widthsTablePtr)
	sbc	hl, de			; Doesn't really matter if we're off-by-one here
	ret	nc
	add	hl, de
	add	hl, bc
	ld	(iy + strucFont.widthsTablePtr), hl
	ld	hl, (iy + strucFont.bitmapsTablePtr)
	sbc	hl, de			; C reset from ADD HL, BC above
	ret	nc			; (we're in Crazytown if there was carry)
	add	hl, de
	add	hl, bc
	ld	(iy + strucFont.bitmapsTablePtr), hl
; Check for the ignore line spacing flag
	ld	hl, arg1
	add	hl, sp
	or	a, (hl)
	jr	z, .true		; (HL) == 0
	lea	hl, iy + strucFont.spaceAbove
	ld	(hl), a
	inc	hl
	ld	(hl), a
.true:
	inc	a
	ret
.false:
	xor	a, a
	ret


;-------------------------------------------------------------------------------
; gontlib_DrawGlyph:


;-------------------------------------------------------------------------------
; util.DrawGlyphRaw:


;-------------------------------------------------------------------------------
; util.DrawGlyphRawKnownWidth:

	ld	c,TEXT_FG_COLOR		; SMCd to have correct foreground color
smcByte _TextStraightForegroundColor

	ld	a,TEXT_BG_COLOR		; SMCd to have correct background color
smcByte _TextStraightBackgroundColor

;-------------------------------------------------------------------------------
; gontlib_DrawString:


;-------------------------------------------------------------------------------
; gontlib_DrawStringL:


;-------------------------------------------------------------------------------
gontlib_DrawInt: ; COPIED_FROM_FONTLIBC
; Places an int at the current cursor position
; Shamelessly ripped from GraphY, which admittedly adapted it from Z80 Bits
; Arguments:
;  arg0 : Number to print
;  arg1 : Number of characters to print
; Returns:
;  None
	pop	de
	pop	hl
	push	hl
	push	de
	add	hl, hl
	db	$3E			; xor a, a -> ld a, *

;-------------------------------------------------------------------------------
gontlib_DrawUInt: ; COPIED_FROM_FONTLIBC
; Places an unsigned int at the current cursor position
; Arguments:
;  arg0 : Number to print
;  arg1 : Minimum number of characters to print
; Returns:
;  None
	xor	a, a
	pop	de
	pop	hl			; hl = uint
	pop	bc			; c = min num chars
	push	bc
	push	hl
	push	de
	jr	nc, .begin		; c ==> actually a negative int
	ex	de, hl
	or	a, a
	sbc	hl, hl
	sbc	hl, de			; hl = -int
	ld	a, '-'
smcByte _DrawIntMinus
	call	.printchar
	dec	c
	jr	nz, .begin
	inc	c
.begin:
	ld	de, -10000000
	call	.num1
	ld	de, -1000000
	call	.num1
	ld	de, -100000
	call	.num1
	ld	de, -10000
	call	.num1
	ld	de, -1000
	call	.num1
	ld	de, -100
	call	.num1
	ld	de, -10
	call	.num1
	ld	de, -1
.num1:
	xor	a, a
.num2:
	inc	a
	add	hl, de
	jr	c, .num2
	sbc	hl, de
	dec	a			; a = next digit
	jr	nz, .printdigit		; z ==> digit is zero, maybe don't print
	ld	a, c
	inc	c
	cp	a, 8
	ret	c			; nc ==> a digit has already been
					;        printed, or must start printing
					;        to satisfy min num chars
	xor	a, a
.printdigit:
	add	a, '0'
smcByte _DrawIntZero
	ld	c, a			; mark that a digit has been printed
.printchar:
	push	bc
	push	hl
	ld	c, a
	push	bc
	call	gontlib_DrawGlyph
	pop	bc
	pop	hl
	pop	bc
	ret


;-------------------------------------------------------------------------------
gontlib_SetForegroundColor: ; COPIED_FROM_FONTLIBC
; Sets the foreground color
; Arguments:
;  arg0: Color
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_TextStraightForegroundColor), a
	ret


;-------------------------------------------------------------------------------
gontlib_SetBackgroundColor: ; COPIED_FROM_FONTLIBC
; Sets the background color
; Arguments:
;  arg0: Color
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_TextStraightBackgroundColor), a
	ret


;-------------------------------------------------------------------------------
gontlib_SetColors: ; COPIED_FROM_FONTLIBC
; Sets both foreground and background color
; Arguments:
;  arg0: Foreground color
;  arg1: Background color
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_TextStraightForegroundColor), a
	inc	hl
	inc	hl
	inc	hl
	ld	a, (hl)
	ld	(_TextStraightBackgroundColor), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetForegroundColor: ; COPIED_FROM_FONTLIBC
; Gets the foreground color
; Arguments:
;  None
; Returns:
;  Current color
	ld	a, (_TextStraightForegroundColor)
	ret


;-------------------------------------------------------------------------------
gontlib_GetBackgroundColor: ; COPIED_FROM_FONTLIBC
; Gets the background color
; Arguments:
;  None
; Returns:
;  Current color
	ld	a, (_TextStraightBackgroundColor)
	ret


;-------------------------------------------------------------------------------
gontlib_SetTransparency: ; COPIED_FROM_FONTLIBC
; Controls whether transparent background mode is used
; Arguments:
;  arg0: Non-zero for transparent mode, zero for opaque
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	sub	a, 1			; Set carry if A = 0
	sbc	a, a			; 0 => -1, else => 0
	inc	a			; 0 => 0, else => 1
	ld	(_TextTransparentMode), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetTransparency: ; COPIED_FROM_FONTLIBC
; Returns whether transparent background mode is being used
; Arguments:
;  None
; Returns:
;  1 if transparent, 0 if opaque
	ld	a, (_TextTransparentMode)
	ret


;-------------------------------------------------------------------------------
gontlib_SetLineSpacing: ; COPIED_FROM_FONTLIBC
; Sets line spacing
; Arguments:
;  arg0: Space above
;  arg1: Space below
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_CurrentFontProperties.spaceAbove), a
	inc	hl
	inc	hl
	inc	hl
	ld	a, (hl)
	ld	(_CurrentFontProperties.spaceBelow), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetSpaceAbove: ; COPIED_FROM_FONTLIBC
; Returns current padding space above
; Arguments:
;  None
; Returns:
;  Padding space above
	ld	a, (_CurrentFontProperties.spaceAbove)
	ret


;-------------------------------------------------------------------------------
gontlib_GetSpaceBelow: ; COPIED_FROM_FONTLIBC
; Returns current padding space below
; Arguments:
;  None
; Returns:
;  Padding space below
	ld	a, (_CurrentFontProperties.spaceBelow)
	ret


;-------------------------------------------------------------------------------
gontlib_SetItalicSpacingAdjustment: ; COPIED_FROM_FONTLIBC
; Sets current spacing adjustment for italic text.  This causes the cursor to be
; moved back a certain number of pixels after every glyph is drawn.  This is
; only useful if transparency mode is set.
; Arguments:
;  arg0: Pixels to move cursor backward after each glyph
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_CurrentFontProperties.italicSpaceAdjust), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetItalicSpacingAdjustment: ; COPIED_FROM_FONTLIBC
; Returns current spacing adjustment for italic text
; Arguments:
;  None
; Returns:
;  Spacing adjustment value
	ld	a, (_CurrentFontProperties.italicSpaceAdjust)
	ret


;-------------------------------------------------------------------------------
gontlib_ValidateCodePoint: ; COPIED_FROM_FONTLIBC
; Returns true if the given code point is present in the current font.
; Arguments:
;  arg0: Glyph index
; Returns:
;  true (1) if present, false (0) if not
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	hl, _CurrentFontProperties.firstGlyph
	sub	a, (hl)
	jr	c, .ret_zero
	ld	hl, _CurrentFontProperties.totalGlyphs
; Check if totalGlyphs is zero (indicating 256 glyphs)
	ld	b, (hl)
	inc	b
	dec	b
	jr	z, .ret_one
	sub	a, (hl)
	ccf
.ret_zero:
.ret_one:
	sbc	a, a
	inc	a
	ret


;-------------------------------------------------------------------------------
gontlib_GetTotalGlyphs: ; COPIED_FROM_FONTLIBC
; Returns the total number of printable glyphs in the font.
; This can return 256.
; Arguments:
;  None
; Returns:
;  Total number of printable glyphs
	or	a, a
	sbc	hl, hl
	ld	a, (_CurrentFontProperties.totalGlyphs)
	ld	l, a
	or	a, a
	ret	nz
	inc	h
	ret


;-------------------------------------------------------------------------------
gontlib_GetFirstGlyph: ; COPIED_FROM_FONTLIBC
; Returns the code point of the first printable glyph.
; Arguments:
;  None
; Returns:
;  Total number of printable glyphs
	ld	a, (_CurrentFontProperties.firstGlyph)
	ret


;-------------------------------------------------------------------------------
gontlib_SetNewlineCode: ; COPIED_FROM_FONTLIBC
; Set the code point that is recognized as being a newline code.
; Arguments:
;  arg0: New code point to use
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_TextNewLineCode), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetNewlineCode: ; COPIED_FROM_FONTLIBC
; Returns the code point that is currently recognized as being a newline.
; Arguments:
;  None
; Returns:
;  Code point that is currently recognized as being a newline
	ld	a, (_TextNewLineCode)
	ret


;-------------------------------------------------------------------------------
gontlib_SetAlternateStopCode: ; COPIED_FROM_FONTLIBC
; Set the code point that is recognized as being an alternate stop code.
; Arguments:
;  arg0: New code point to use
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_TextAlternateStopCode), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetAlternateStopCode: ; COPIED_FROM_FONTLIBC
; Returns the code point that is currently recognized as being an alternate stop
; code.
; Arguments:
;  None
; Returns:
;  Code point that is currently recognized as being an alternate stop code
	ld	a, (_TextAlternateStopCode)
	ret


;-------------------------------------------------------------------------------
gontlib_SetFirstPrintableCodePoint: ; COPIED_FROM_FONTLIBC
; Set the code point that is recognized as being an alternate stop code.
; Arguments:
;  arg0: New code point to use
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_TextFirstPrintableCodePoint), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetFirstPrintableCodePoint: ; COPIED_FROM_FONTLIBC
; Returns the first code point that is currently recognized as being printable.
; code.
; Arguments:
;  None
; Returns:
;  Code point that is currently recognized as being the first printable code
;    point.
	ld	a, (_TextFirstPrintableCodePoint)
	ret


;-------------------------------------------------------------------------------
gontlib_SetDrawIntCodePoints: ; COPIED_FROM_FONTLIBC
; Changes the code points printed using DrawInt/DrawUInt
; Arguments:
;  arg0: Negative symbol code point
;  arg1: Zero glyph code point
; Returns:
;  Nothing
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_DrawIntMinus), a
	inc	hl
	inc	hl
	inc	hl
	ld	a, (hl)
	ld	(_DrawIntZero), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetDrawIntMinus: ; COPIED_FROM_FONTLIBC
; Returns the minus code point used by DrawInt/DrawUInt
; Arguments:
;  None
; Returns:
;  Minus
	ld	a, (_DrawIntMinus)
	ret


;-------------------------------------------------------------------------------
gontlib_GetDrawIntZero: ; COPIED_FROM_FONTLIBC
; Returns the '0' code point used by DrawInt/DrawUInt
; Arguments:
;  None
; Returns:
;  '0'
	ld	a, (_DrawIntZero)
	ret


;-------------------------------------------------------------------------------
gontlib_GetGlyphWidth: ; COPIED_FROM_FONTLIBC
; Returns the width of a given glyph.
; Arguments:
;  arg0: Codepoint
; Returns:
;  Width of glyph
;    Zero if invalid index
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
;	jp	util.GetGlyphWidth
assert $ = util.GetGlyphWidth


;-------------------------------------------------------------------------------
util.GetGlyphWidth: ; COPIED_FROM_FONTLIBC
; Internal-use version
; Input:
;  A: Codepoint
; Output:
;  A: Width
;  C if invalid codepoint, NC if valid
; Destroys:
;  DE
;  HL
; Subtract out firstGlyph
	ld	hl, _CurrentFontProperties.firstGlyph
	sub	a, (hl)
; Validate that the glyph index is actually valid
	jr	nc, .checkMaxIndex
.invalidIndex:
	xor	a, a
	scf
	ret
.checkMaxIndex:
	ld	hl, _CurrentFontProperties.totalGlyphs
	cp	a, (hl)
	jr	c, .invalidIndex
; Look up width
	or	a, a
	sbc	hl, hl
	ld	l, a
	ld	de, (_CurrentFontProperties.widthsTablePtr)
	add	hl, de
	ld	a, (hl)
	ret


;-------------------------------------------------------------------------------
gontlib_GetStringWidth: ; COPIED_FROM_FONTLIBC
; Returns the width of a string.
; Stops when it encounters any control code or a codepoint not in the current
; font.
; Arguments:
;  arg0: Pointer to string
; Returns:
;  Width of string
	pop	hl
	pop	bc
	push	bc
	push	hl
	scf
	sbc	hl, hl
	jr	_GetStringWidthL


;-------------------------------------------------------------------------------
gontlib_GetStringWidthL: ; COPIED_FROM_FONTLIBC
; Returns the width of a string.
; Stops when it encounters any control code or a codepoint not in the current
; font, or when it reaches the maximum number of characters to process.
; Arguments:
;  arg0: Pointer to string
;  arg1: Maximum number of characters to process
; Returns:
;  Width of string
	ld	hl, arg0
	add	hl, sp
	ld	bc, (hl)
	inc	hl
	inc	hl
	inc	hl
	ld	hl, (hl)
_GetStringWidthL:
	push	ix
	ld	ix, DataBaseAddr
	ld	iy, 0
	ld	(ix + charactersLeft), hl
	ld	de, (ix + strucFont.widthsTablePtr)
	ld	a, (bc)
	or	a, a
	jr	z, .exitFast
.loop:
; Check that we haven't exceeded our glyph printing limit
	ld	hl, (ix + charactersLeft)
	add	hl, bc
	or	a, a
	sbc	hl, bc
	jr	z, .exit
	dec	hl
	ld	(ix + charactersLeft), hl
; Fetch next item
	ld	a, (bc)
	cp	a, (ix + firstPrintableCodePoint)
	jr	c, .exit
	or	a, a
	jr	z, .exit
	cp	a, (ix + alternateStopCode)
	jr	z, .exit
	sub	a, (ix + strucFont.firstGlyph)
	jr	c, .exit
	cp	a, (ix + strucFont.totalGlyphs)
	jr	c, .validCodepoint
	ld	(ix + readCharacter), a
	ld	a, (ix + strucFont.totalGlyphs)
	or	a, a
	jr	nz, .exit
	ld	a, (ix + readCharacter)
.validCodepoint:
	inc	bc
	or	a, a
	sbc	hl, hl
	ld	l, a
	add	hl, de
	ld	a, (hl)
	sub	a, (ix + strucFont.italicSpaceAdjust) ; So if this results in a negative number
	sbc	hl, hl			; then this too will become negative,
	ld	l, a			; which gives the intended result, I guess
	ex	de, hl
	add	iy, de
	ex	de, hl
	jr	.loop
.exit:
	ld	a, (ix + strucFont.italicSpaceAdjust)
	neg
	jr	z, .exitFast
	ld	de, -1
	ld	e, a
	add	iy, de
.exitFast:
	ld	(ix + strReadPtr), bc
	lea	hl, iy + 0
	pop	ix
	ret


;-------------------------------------------------------------------------------
gontlib_GetLastCharacterRead: ; COPIED_FROM_FONTLIBC
; Returns the address of the last character printed by DrawString or processed
; by GetStrWidth.
; Arguments:
;  None
; Returns:
;  Pointer to last character read
	ld	hl, (_TextLastCharacterRead)
	ret


;-------------------------------------------------------------------------------
gontlib_GetCharactersRemaining: ; COPIED_FROM_FONTLIBC
; Allows you to figure out whether DrawStringL or GetStringWidthL returned due
; to having finished processed max_characters.
; Arguments:
;  None
; Returns:
;  Last internal value of _TempCharactersLeft, taken from max_characters param
;  to GetStringWidth and DrawString.
	ld	hl, (_TempCharactersLeft)
	ret


;-------------------------------------------------------------------------------
gontlib_ClearWindow: ; COPIED_FROM_FONTLIBC
; Erases the entire text window.
; Arguments:
;  None
; Returns:
;  Nothing
	ld	hl, (_TextX)
	push	hl
	ld	a, (_TextY)
	push	af
	ld	hl, (_TextXMax)
	ld	de, (_TextXMin)
	ld	(_TextX), de
	or	a, a
	sbc	hl, de
	ex	de, hl
	ld	a, (_TextYMin)
	ld	(_TextY), a
	ld	b, a
	ld	a, (_TextYMax)
	sub	a, b
	ld	b, a
	call	util.ClearRect
	pop	af
	ld	(_TextY), a
	pop	hl
	ld	(_TextX), hl
	ret


;-------------------------------------------------------------------------------
gontlib_SetNewlineOptions: ; COPIED_FROM_FONTLIBC
; Sets options for controlling newline behavior
; Arguments:
;  arg0: Flags for newline behavior
; Returns:
;  None
	ld	hl, arg0
	add	hl, sp
	ld	a, (hl)
	ld	(_TextNewlineControl), a
	ret


;-------------------------------------------------------------------------------
gontlib_GetNewlineOptions: ; COPIED_FROM_FONTLIBC
; Returns current newline flags
; Arguments:
;  None
; Returns:
;  Current newline flags
	ld	a, (_TextNewlineControl)
	ret


;-------------------------------------------------------------------------------
gontlib_Newline: ; COPIED_FROM_FONTLIBC
; Prints a newline, may trigger pre/post clear and scrolling
; Arguments:
;  None
; Returns:
;  A = 0 on success
;  A = 1 if the text window is full
	ld	iy, DataBaseAddr
	bit	bAutoClearToEOL, (iy + newlineControl)
; I hate how nearly every time I think CALL cc or RET cc would be useful
; it turns out I need to do other stuff that prevents me from using it.
	call	nz, gontlib_ClearEOL
	ld	iy, DataBaseAddr
	ld	hl, (iy + textXMin)
	ld	(iy + textX), hl
	ld	a, (iy + strucFont.height)
	add	a, (iy + strucFont.spaceAbove)
	add	a, (iy + strucFont.spaceBelow)
	ld	b, a
	add	a, a
	jr	c, .noScroll		; Carry = definitely went past YMax
	add	a, (iy + textY)		; And scrolling is not valid if height > 127
	jr	c, .outOfSpace
	cp	a, (iy + textYMax)
	jr	z, .writeCursorY
	jr	c, .writeCursorY
.outOfSpace:
	bit	bAutoScroll, (iy + newlineControl)
	jr	z, .noScroll
	call	gontlib_ScrollWindowDown
	ld	iy, DataBaseAddr	; Don't need to write textY---cursor
	jr	.checkPreClear		; didn't actually move!
.noScroll:
	ld	a, 1
	bit	bEnableAutoWrap, (iy + newlineControl)
	ret	z
	ld	b, (iy + textYMin)
	ld	(iy + textY), b
	ret
.writeCursorY:
	sub	a, b
	ld	(iy + textY), a
.checkPreClear:
	xor	a, a
	bit	bPreclearNewline, (iy + newlineControl)
	ret	z
; Fall through to ClearEOL
assert $ = gontlib_ClearEOL


;-------------------------------------------------------------------------------
gontlib_ClearEOL: ; COPIED_FROM_FONTLIBC
; Erases everything from the cursor to the right side of the text window.
; Arguments:
;  None
; Returns:
;  A = 0
; Compute the rectangle size to clear
	ld	de, (_TextX)
	ld	hl, (_TextXMax)
	xor	a, a
	sbc	hl, de
	ret	c
	ret	z
	ex	de, hl
	ld	a, (_CurrentFontProperties.height)
	ld	hl, _CurrentFontProperties.spaceAbove
	add	a, (hl)
	inc	hl
	add	a, (hl)
	ld	b, a
; Fall through to ClearRect
assert $ = util.ClearRect


;-------------------------------------------------------------------------------
util.ClearRect:
	xor	a, a ; DEBUG
	ret	; DEBUG
; Internal routine that erases a rectangle at the current cursor location.
; Arguments:
;  B: Height
;  DE: Width
; Destroys:
;  AF, BC, DE, HL, IY

	ld	a, e
	or	a, d
	; dont care what UDE is
	ret	z			; make sure width is not 0
	ld	a, b
	or	a, a
	ret	z			; make sure height is not 0
	push	de
	ld	a, (_TextY)
	ld	e, a			; e = y coordinate
	ld	a, b
	pop	bc
	ld	hl, (_TextX)		; hl = x coordinate
	ld	d, h			; maybe ld d, 0
	dec	h			; tests if x >= 256
	ld	h, ti.lcdHeight
	jr	nz, .x_lt_256
	ld	d, h			; ld d, ti.lcdHeight * 256
.x_lt_256:
	mlt	hl
	ex.s	de, hl			; clear upper byte of DE
	add	hl, de			; add y cord
	ld	de, (CurrentBuffer)
	add	hl, de			; add buffer offset
	ex	de, hl			; de -> place to begin drawing
	push	de

	ld	hl, _TextStraightBackgroundColor
	; divide (width-1) by 2, and set Z flag
	dec	bc
	srl	b
	rr	c
	ld	iyl, c
	ld	c, a
	ld	b, 0
	; iyl = (width-1)/2
	; carry = (width-1)%2
	; zero = iyl==0
	; a = bc = height
	call	gfy_Wait
	ldi				; check if we only need to draw 1 pixel
	pop	hl
	jp	po, .skip
	ldir
.skip:
	jr	z, .final
	push	af			; save carry
.loop:
	ld	c, ti.lcdHeight - 1
	ex	de, hl
	add	hl, bc
	ex	de, hl
	ld	c, a
	lddr
	inc	hl
	ld	c, ti.lcdHeight + 1
	ex	de, hl
	add	hl, bc
	ex	de, hl
	ld	c, a
	ldir
	dec	hl
	dec	iyl
	jr	nz, .loop
	pop	af			; restore carry
.final:
	ret	nc
	ld	c, ti.lcdHeight - 1
	ex	de, hl
	add	hl, bc
	ex	de, hl
	ld	c, a
	lddr
	ret

;-------------------------------------------------------------------------------
; gontlib_ScrollWindowUp:

;-------------------------------------------------------------------------------
; gontlib_ScrollWindowDown:

;-------------------------------------------------------------------------------
gontlib_GetCurrentFontHeight: ; COPIED_FROM_FONTLIBC
; Returns the height of the current font
; Arguments:
;  None
; Returns:
;  Height
	ld	a,(_CurrentFontProperties.height)
	ld	hl,_CurrentFontProperties.spaceAbove
	add	a,(hl)
	inc	hl
	add	a,(hl)
	ret


;-------------------------------------------------------------------------------
; Font Pack Support
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
util.GetFontPackData: ; COPIED_FROM_FONTLIBC
; Attempts to get a pointer to a font pack's data based on its appvar's name.
; Arguments:
;  HL: Pointer to name string
; Returns:
;  HL: Pointer to byte after "FONTPACK", or NULL on failure
;  DE: Pointer to start of "FONTPACK", or garbage on failure
;  Carry set if appvar not found, or not a font pack; NC on success
	dec	hl
	ld	iy, ti.flags
	call	ti.Mov9ToOP1
	ld	a, ti.AppVarObj
	ld	(ti.OP1), a
	call	ti.ChkFindSym
	jr	c, .error
	ex	de, hl
	ld	a, b
	cp	a, $D0
	jr	nc, .headerCheck
; Sadly, TI doesn't kindly provide us with a direct pointer; we have to account
; for the archive header ourselves.
	ld	de, 9
	add	hl, de
	ld	e, (hl)
	inc	hl
	add	hl, de
.headerCheck:
; Check header
	inc	hl
	inc	hl
	push	hl
	call	util.VerifyHeader
	pop	de
	ret	z
.error:
	or	a, a
	sbc	hl, hl
	scf
	ret


;-------------------------------------------------------------------------------
util.VerifyHeader: ; COPIED_FROM_FONTLIBC
; Verify that HL points to something that looks somewhat like a font pack.
; Arguments:
;  HL: Pointer to supposed font pack
; Returns:
;  Z if the check passes, NZ if not
;  HL points to byte after 'K'
;  Carry = cleared
; Destroys:
;  A, B, DE
	ld	de, _FontPackHeaderString
	ld	b, 8
.loop:
	ld	a, (de)
	xor	a, (hl)
	ret	nz
	inc	de
	inc	hl
	djnz	.loop
	ret


;-------------------------------------------------------------------------------
gontlib_GetFontPackName: ; COPIED_FROM_FONTLIBC
; Returns a pointer to the font pack's name string.  Useful in a loop using
; ti_Detect().
; Arguments:
;  arg0: Pointer to font pack appvar name
; Returns:
;  Pointer, or NULL if no name
	ld	hl, arg0
	add	hl, sp
	ld	hl, (hl)
	call	util.GetFontPackData
	ret	c
; Check metadata offset field
	ld	bc, 0
	ld	hl, (hl)
	sbc	hl, bc
	ret	z
; Check name field
	add	hl, de
	inc	hl
	inc	hl
	inc	hl
	ld	hl, (hl)
	sbc	hl, bc
	ret	z
	add	hl, de
	ret


;-------------------------------------------------------------------------------
gontlib_GetFontByIndex: ; COPIED_FROM_FONTLIBC
; Returns a pointer to a font in a font pack, based on an index.
; Arguments:
;  arg0: Pointer to font pack's name
;  arg1: Index
; Returns:
;  Pointer, or NULL if error
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + arg0)
	push	iy
	call	util.GetFontPackData
	pop	iy
	; ld	(iy + arg0), de
	jr	nc, _GetFontByIndexRaw
	ret


;-------------------------------------------------------------------------------
gontlib_GetFontByIndexRaw: ; COPIED_FROM_FONTLIBC
; Returns a pointer to a font in a font pack, based on an index.
; Arguments:
;  arg0: Pointer to first data byte of font pack
;  arg1: Index
; Returns:
;  Pointer, or NULL if error
	ld	iy, 0
	add	iy, sp
	ld	de, (iy + arg0)
_GetFontByIndexRaw:
; Get font count
	ld	hl, strucFontPackHeader.fontCount
	add	hl, de
; Validate index
	ld	a, (iy + arg1)
	cp	a, (hl)
	jr	nc, .error
; Get offset to font
	inc	hl
	ld	c, a
	ld	b, 3
	mlt	bc
	add	hl, bc
	ld	hl, (hl)
	add	hl, de
	ret

.error:
	sbc	hl, hl
	ret


;-------------------------------------------------------------------------------
gontlib_GetFontByStyle: ; COPIED_FROM_FONTLIBC
; Searches for a font in a font pack given a set of properties.
; Arguments:
;  arg0: Pointer to font pack's name
;  arg1: Minimum acceptable size
;  arg2: Maximum acceptable size
;  arg3: Minimum acceptable weight
;  arg4: Maximum acceptable weight
;  arg5: Style bits that must be set
;  arg6: Style bits that must be reset
; Returns:
;  Pointer, or NULL if error
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + arg0)
	push	iy
	call	util.GetFontPackData
	pop	iy
	; ld	(iy + arg0), de
	jr	nc, _GetFontByStyleRaw
	ret

;-------------------------------------------------------------------------------
gontlib_GetFontByStyleRaw: ; COPIED_FROM_FONTLIBC
; Searches for a font in a font pack given a set of properties.
; Arguments:
;  arg0: Pointer to first data byte of font pack
;  arg1: Minimum acceptable size
;  arg2: Maximum acceptable size
;  arg3: Minimum acceptable weight
;  arg4: Maximum acceptable weight
;  arg5: Style bits that must be set
;  arg6: Style bits that must be reset
; Returns:
;  Pointer, or NULL if error
	ld	iy, 0
	add	iy, sp
	ld	de, (iy + arg0)
_GetFontByStyleRaw:
	; DE = Cached pointer to font pack start
	push	ix
	; Get pointer to fonts table
	ld	hl, strucFontPackHeader.fontCount
	add	hl, de
	ld	b, (hl)
	inc	hl
.checkLoop:
	ld	ix, (hl)
	add	ix, de
	call	.checkStyle
	inc	hl
	inc	hl
	inc	hl
	djnz	.checkLoop
.badFont:
	pop	ix
	or	a, a
	sbc	hl, hl
	ret

.checkStyle:
	ld	a, (ix + strucFont.height)
	cp	a, (iy + arg1)
	ret	c
	cp	a, (iy + arg2)
	jr	z, .sizeOK
	ret	nc
.sizeOK:
	ld	a, (ix + strucFont.weight)
	cp	a, (iy + arg3)
	ret	c
	cp	a, (iy + arg4)
	jr	z, .weightOK
	ret	nc
.weightOK:
	ld	c, (ix + strucFont.style)
	ld	a, (iy + arg5)
	and	a, c
	xor	a, c
	ret	nz
	ld	a, (iy + arg6)
	and	a, c
	ret	z
.goodFont:
	pop	bc			; reset SP
	lea	hl, ix + 0
	pop	ix
	ret


;-------------------------------------------------------------------------------
; Data
_FontPackHeaderString:
	db	"FONTPACK"
_TextDefaultWindow:
textDefaultWindow := _TextDefaultWindow - DataBaseAddr
	dl	0
	db	0
	dl	ti.lcdWidth
	db	ti.lcdHeight
_TextXMin:
textXMin := _TextXMin - DataBaseAddr
	dl	0
_TextYMin:
textYMin := _TextYMin - DataBaseAddr
	db	0
_TextXMax:
textXMax := _TextXMax - DataBaseAddr
	dl	ti.lcdWidth
_TextYMax:
textYMax := _TextYMax - DataBaseAddr
	db	ti.lcdHeight
_TextX:
textX := _TextX - DataBaseAddr
	dl	0
_TextY:
textY := _TextY - DataBaseAddr
	db	0
_TextTransparentMode:
textTransparentMode := _TextTransparentMode - DataBaseAddr
	db	0
_TextNewlineControl:
newlineControl := _TextNewlineControl - DataBaseAddr
	db	mEnableAutoWrap or mAutoClearToEOL
_TextLastCharacterRead:
strReadPtr := _TextLastCharacterRead - DataBaseAddr
	dl	0
_TempCharactersLeft:
charactersLeft := _TempCharactersLeft - DataBaseAddr
	dl	0
_TextAlternateStopCode:
alternateStopCode := _TextAlternateStopCode - DataBaseAddr
	db	0
_TextFirstPrintableCodePoint:
firstPrintableCodePoint := _TextFirstPrintableCodePoint - DataBaseAddr
	db	chFirstPrintingCode
_TextNewLineCode:
newLineCode := _TextNewLineCode - DataBaseAddr
	db	chNewline
tempRandom:
readCharacter := tempRandom - DataBaseAddr
	db	0
_CurrentFontRoot:
currentFontRoot := _CurrentFontRoot - DataBaseAddr
	dl	0
DataBaseAddr:
; Embed the current font's properties as library variables
_CurrentFontProperties strucFont

;-------------------------------------------------------------------------------
; aliases
;-------------------------------------------------------------------------------

_conf := _CurrentFontProperties
_root := _CurrentFontRoot

;-------------------------------------------------------------------------------
; libc
;-------------------------------------------------------------------------------

__bshl      := $000100
__bshru     := $000104
__ishl      := $000174
__ishru     := $000184
__iand      := $000134
__ior       := $000168
__ixor      := $000198
__inot      := $000164
__ineg      := $000160
__idivs     := $00013C
__idivu     := $000140
__iremu     := $000170
__irems     := $00016C
; __imulu     := $000154

; _memcpy     := $0000A4
; _memmove    := $0000A8
; _memset     := $0000AC

__setflag   := $000218
__frameset  := $00012C
__frameset0 := $000130

; for debugging
_boot_sprintf := $0000BC

;-------------------------------------------------------------------------------
; inlined routines
;-------------------------------------------------------------------------------

_memcpy:
	ld	iy, -1
	add	iy, sp
	ld	bc, (iy + 10)  ; Load count
	sbc	hl, hl
	add	hl, bc
	jr	nc, .zero
	ld	de, (iy + 4)  ; Load destination
	ld	hl, (iy + 7)  ; Load source
	ldir
.zero:
	ld	hl, (iy + 4)  ; Return the destination pointer
	ret

; Optimized for when src != dst
_memmove:
	; src > dst | LDIR | 32F + 15R + 1
	; src < dst | LDDR | 35F + 12R + 2
	; src = dst | LDDR | 35F + 12R + 2
	; zero size |      | 24F + 12R + 2

	ld	iy, -1
	add	iy, sp
	ld	bc, (iy + 10)
	sbc	hl, hl
	add	hl, bc
	jr	nc, .zero
	ld	hl, (iy + 7)
	ld	de, (iy + 4)
	sbc	hl, de
	; src <= dst
	jr	c, .copy_backwards
	; src > dst
; .copy_forwards:
	add	hl, de
	inc	hl
	ldir
.zero:
	ld	hl, (iy + 4)
	ret

.copy_backwards:
	; move HL and DE to the end
	ex	de, hl
	add	hl, bc
	ex	de, hl	; HL = src - dst - 1, DE = dst + size
	add	hl, de	; HL = src + size - 1
	dec	de	; DE = dst + size - 1
	lddr
	ex	de, hl
	inc	hl
	ret

_memset:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 3)
	ld	bc, (iy + 9)
	cpi
	add	hl, bc
	ret	c
	dec	hl
	ld	e, (iy + 6)
	ld	(hl), e
	ret	po
	push	hl
	pop	de
	dec	de
	lddr
	ret

__imulu:
	push	af
	push	de
	ld	d, b
	ld	e, h
	mlt	de
	ld	a, e
	dec	sp
	push	hl
	push	bc
	inc	sp
	pop	de
	ld	e, l
	mlt	de
	add	a, e
	pop	de
	ld	e, c
	mlt	de
	add	a, e
	ld	e, l
	ld	l, c
	mlt	hl
	add	a, h
	ld	h, a
	ld	a, e
	ld	d, b
	mlt	de
	add	hl, de
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	ld	d, a
	ld	e, c
	mlt	de
	add	hl, de
	pop	de
	pop	af
	ret

__set_bc_and_mul_hl_by_240:
	push	hl
	pop	bc
	add	hl, hl	; 2
	add	hl, bc	; 3
	add	hl, hl	; 6
	add	hl, bc	; 7
	add	hl, hl	; 14
	add	hl, bc	; 15
	add	hl, hl	; 30
	add	hl, hl	; 60
	add	hl, hl	; 120
	add	hl, hl	; 240
	ld	bc, 240
	ret

;-------------------------------------------------------------------------------
; gontlibc.c.src
;-------------------------------------------------------------------------------

include 'gontlibc.c.src'
