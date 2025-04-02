;-------------------------------------------------------------------------------

include '../include/library.inc'

;-------------------------------------------------------------------------------

library GRAPHY, 13

;-------------------------------------------------------------------------------
; no dependencies
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; v1 functions
;-------------------------------------------------------------------------------
	export gfy_Begin		
	export gfy_End
	export gfy_SetColor
	export gfy_SetDefaultPalette
	export gfy_SetPalette
	export gfy_FillScreen
	export gfy_SetPixel
	export gfy_GetPixel
	export gfy_GetDraw
	export gfy_SetDraw
	export gfy_SwapDraw
	export gfy_Blit
	export gfy_BlitColumns
	export gfy_BlitRectangle
	export gfy_PrintChar
	export gfy_PrintInt
	export gfy_PrintUInt
	export gfy_PrintString
	export gfy_PrintStringXY
	export gfy_SetTextXY
	export gfy_SetTextBGColor
	export gfy_SetTextFGColor
	export gfy_SetTextTransparentColor
	export gfy_SetFontData
	export gfy_SetFontSpacing
	export gfy_SetMonospaceFont
	export gfy_GetStringWidth
	export gfy_GetCharWidth
	export gfy_GetTextX
	export gfy_GetTextY
	export gfy_Line
	export gfy_HorizLine
	export gfy_VertLine
	export gfy_Circle
	export gfy_FillCircle
	export gfy_Rectangle
	export gfy_FillRectangle
	export gfy_Line_NoClip
	export gfy_HorizLine_NoClip
	export gfy_VertLine_NoClip
	export gfy_FillCircle_NoClip
	export gfy_Rectangle_NoClip
	export gfy_FillRectangle_NoClip
	export gfy_SetClipRegion
	export gfy_GetClipRegion
	export gfy_ShiftDown
	export gfy_ShiftUp
	export gfy_ShiftLeft
	export gfy_ShiftRight
	export gfy_Tilemap
	export gfy_Tilemap_NoClip
	export gfy_TransparentTilemap
	export gfy_TransparentTilemap_NoClip
	export gfy_TilePtr
	export gfy_TilePtrMapped
	export gfy_Reserved
	export gfy_AllocSprite
	export gfy_Sprite
	export gfy_TransparentSprite
	export gfy_Sprite_NoClip
	export gfy_TransparentSprite_NoClip
	export gfy_GetSprite
	export gfy_ScaledSprite_NoClip
	export gfy_ScaledTransparentSprite_NoClip
	export gfy_FlipSpriteY
	export gfy_FlipSpriteX
	export gfy_RotateSpriteC
	export gfy_RotateSpriteCC
	export gfy_RotateSpriteHalf
	export gfy_Polygon
	export gfy_Polygon_NoClip
	export gfy_FillTriangle
	export gfy_FillTriangle_NoClip
;-------------------------------------------------------------------------------
; v2 functions
;-------------------------------------------------------------------------------
	export gfy_Deprecated
	export gfy_SetTextScale
;-------------------------------------------------------------------------------
; v3 functions
;-------------------------------------------------------------------------------
	export gfy_SetTransparentColor
	export gfy_ZeroScreen
	export gfy_SetTextConfig
	export gfy_GetSpriteChar
;-------------------------------------------------------------------------------
; v4 functions
;-------------------------------------------------------------------------------
	export gfy_Lighten
	export gfy_Darken
;-------------------------------------------------------------------------------
; v5 functions
;-------------------------------------------------------------------------------
	export gfy_SetFontHeight
	export gfy_ScaleSprite
	export gfy_FloodFill
;-------------------------------------------------------------------------------
; v6 functions
;-------------------------------------------------------------------------------
	export gfy_RLETSprite
	export gfy_RLETSprite_NoClip
	export gfy_ConvertFromRLETSprite
	export gfy_ConvertToRLETSprite
	export gfy_ConvertToNewRLETSprite
;-------------------------------------------------------------------------------
; v7 functions
;-------------------------------------------------------------------------------
	export gfy_RotateScaleSprite
	export gfy_RotatedScaledTransparentSprite_NoClip
	export gfy_RotatedScaledSprite_NoClip
;-------------------------------------------------------------------------------
; v8 functions
;-------------------------------------------------------------------------------
	export gfy_SetCharData
;-------------------------------------------------------------------------------
; v9 functions
;-------------------------------------------------------------------------------
	export gfy_Wait
;-------------------------------------------------------------------------------
; v10 functions
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; v11 functions
;-------------------------------------------------------------------------------
	export gfy_CopyRectangle
;-------------------------------------------------------------------------------
; v12 functions
;-------------------------------------------------------------------------------
	export gfy_Ellipse
	export gfy_Ellipse_NoClip
	export gfy_FillEllipse
	export gfy_FillEllipse_NoClip

;-------------------------------------------------------------------------------
LcdSize            := ti.lcdWidth*ti.lcdHeight
; minimum stack size to provide for interrupts if moving the stack
InterruptStackSize := 4000
CurrentBuffer      := ti.mpLcdLpbase
TRASPARENT_COLOR   := 0
TEXT_FG_COLOR      := 0
TEXT_BG_COLOR      := 255
TEXT_TP_COLOR      := 255
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
macro mIsHLLessThanDE?
	or	a,a
	sbc	hl,de
	add	hl,hl
	jp	po,$+5
	ccf
end macro
macro mIsHLLessThanBC?
	or	a,a
	sbc	hl,bc
	add	hl,hl
	jp	po,$+5
	ccf
end macro
macro s8 op, imm
	local i
 	i = imm
	assert i >= -128 & i < 128
	op, i
end macro

;-------------------------------------------------------------------------------
wait_quick.usages_counter = 0

macro wait_quick?
	call	_WaitQuick
	wait_quick.usages_counter = wait_quick.usages_counter + 1
end macro

postpone
	wait_quick.usages := wait_quick.usages_counter
end postpone

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
	ex	(sp),hl			; l = byte
	ld	a,l			; a = byte
	match expand, list
		iterate expand
			if % = 1
				ld	hl,each
				ld	c,(hl)
				ld	(hl),a
			else
				ld	(each),a
			end if
		end iterate
	end match
	ld	a,c			; a = old byte
	ex	de,hl			; hl = return vector
	jp	(hl)
end macro

macro setSmcBytesInline name*
	local temp, list
	postpone
		temp equ each
		irpv each, name
			temp equ temp, each
		end irpv
		list equ temp
	end postpone

	match expand, list
		iterate expand
			ld	(each),a
		end iterate
	end match
end macro

macro setSmcWordsInline name*
	local temp, list
	postpone
		temp equ each
		irpv each, name
			temp equ temp, each
		end irpv
		list equ temp
	end postpone

	match expand, list
		iterate expand
			ld	(each),hl
		end iterate
	end match
end macro

macro smcByte name*, addr: $-1
	local link
	link := addr
	name equ link
end macro

macro smcWord name*, addr: $-3
	local link
	link := addr
	name equ link
end macro

;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; gfy_Begin:

;-------------------------------------------------------------------------------
gfy_internal_Begin:
; Sets up the graphics canvas
; Arguments:
;  arg0: bpp mode to start in
; Returns:
;  None
	call	ti.boot.ClearVRAM	; clear the screen
lcdGraphxMode := ti.lcdWatermark+ti.lcdIntFront+ti.lcdPwr+ti.lcdBgr+ti.lcdBpp8
	ld	de,lcdGraphxMode
	ld	hl,CurrentBuffer
SetGfy:
	ld	bc,ti.vRam
	ld	(hl),bc			; set the current draw to the screen
assert CurrentBuffer and -$100 = ti.mpLcdRange
	ld	l,ti.lcdCtrl
	ld	(hl),de			; set lots of control parameters
	ld	l,ti.lcdTiming0+1
	ld	de,_LcdTiming
assert ti.vRam and $FF = 0
	ld	b,8+1			; +1 because c = 0, so first ldi will
					; decrement b
.ExchangeTimingLoop:			; exchange stored and active timing
	ld	a,(de)
	ldi
	dec	hl
	ld	(hl),a
	inc	hl
	djnz	.ExchangeTimingLoop
;	jp	gfy_SetDefaultPalette	; setup the default palette
assert $ = gfy_SetDefaultPalette

;-------------------------------------------------------------------------------
gfy_SetDefaultPalette: ; COPIED_FROM_GRAPHX
; Sets up the default palette where H=L
; Arguments:
;  None
; Returns:
;  None
	ld	de,ti.mpLcdPalette	; address of mmio palette
	ld	b,e			; b = 0
.loop:
	ld	a,b
	rrca
	xor	a,b
	and	a,224
	xor	a,b
	ld	(de),a
	inc	de
	ld	a,b
	rla
	rla
	rla
	ld	a,b
	rra
	ld	(de),a
	inc	de
	inc	b
	jr	nz,.loop		; loop for 256 times to fill palette
	ret

;-------------------------------------------------------------------------------
; gfy_End:

;-------------------------------------------------------------------------------
gfy_internal_End:
; Closes the graphics library and sets up for the TI-OS
; Arguments:
;  None
; Returns:
;  None
	call	ti.boot.ClearVRAM	; clear the screen
	ld	de,ti.lcdNormalMode
	ld	hl,ti.mpLcdBase
	jr	SetGfy			; restore the screen mode

;-------------------------------------------------------------------------------
gfy_AllocSprite: ; COPIED_FROM_GRAPHX
; Allocates space for a new sprite
; Arguments:
;  arg0 : width
;  arg1 : height
;  arg2 : pointer to malloc routine
; Returns:
;  Pointer to allocated sprite, first byte width, second height
	ld	bc,3
	push	bc
	pop	hl
	add	hl,sp
	ld	e,(hl)			; e = width
	add	hl,bc
	ld	d,(hl)			; d = height
	add	hl,bc
	ld	hl,(hl)			; hl = malloc
	push	de
	mlt	de			; de = width * height
	inc	de			; +2 to store width and height
	inc	de			; de = width * height + 2
	push	de
	call	_indcallHL		; hl = malloc(width * height + 2)
	pop	de			; de = width * height + 2
	add	hl,de			; this should never carry
	sbc	hl,de			; check if malloc failed (hl == 0)
	pop	de			; e = width, d = height
	ret	z			; abort if malloc failed
	ld	(hl),de			; store width and height
	ret

;-------------------------------------------------------------------------------
gfy_SetClipRegion: ; COPIED_FROM_GRAPHX
; Sets the clipping region for clipped routines
; Arguments:
;  arg0 : Xmin
;  arg1 : Ymin
;  arg2 : Xmax
;  arg3 : Ymax
; Returns:
;  None
	; clip against the actual LCD screen
	xor	a,a
	sbc	hl,hl
	ld	(_ClipRegion.XMin),hl
	inc	h
	ld	l,ti.lcdWidth-256
	ld	(_ClipRegion.XMax),hl
	ld	(_ClipRegion.YMin),a
	ld	a,ti.lcdHeight
	ld	(_ClipRegion.YMax),a
	ld	iy,0
	add	iy,sp
	call	_ClipRegion		; iy points to the start of the arguments
	ld	hl,(iy+3)
	ld	c,(iy+6)
	ld	de,(iy+9)
	ld	a,(iy+12)
	jr	nc,.apply
	xor	a,a
	ld	c,a
	sbc	hl,hl
	ld	de,ti.lcdWidth
	ld	a,ti.lcdHeight
.apply:
	setSmcWordsInline _XMin
	ex	de,hl
	setSmcWordsInline _XMax
	dec	hl
	setSmcWordsInline _XMaxMinus1
	inc	hl
	sbc	hl,de
	setSmcWordsInline _XSpan
	setSmcBytesInline _YMax
	dec	a
	setSmcBytesInline _YMaxMinus1
	inc	a
	sub	a,c
	setSmcBytesInline _YSpan
	ld	a,c
	setSmcBytesInline _YMin
	ret

;-------------------------------------------------------------------------------
gfy_Lighten: ; COPIED_FROM_GRAPHX
; Lightens a 16 bit 1555 color (0 = white, 255 = same color)
; Arguments:
;  arg0 : 16 bit color
;  arg1 : 8 bit change amount
; Returns:
;  16 bit color value
	pop	de			; de = return vector
	pop	bc			; bc = color
	ex	(sp),hl			; l = amt
	push	bc
	push	de
					; Strategy: lighten(color, amt) = ~darken(~color, amt)
					; Darken the inverted color
	ld	a,c
	cpl
	ld	c,a
	ld	a,b
	cpl
	ld	b,a			; bc = ~color
	call	_Darken			; hl = darken(~color, amt)
	ld	a,l			; Invert the darken result for the lighten result
	cpl
	ld	l,a
	ld	a,h
	cpl
	ld	h,a			; hl = ~darken(~color, amt) = lighten(color, amt)
	ret

;-------------------------------------------------------------------------------
gfy_Darken: ; COPIED_FROM_GRAPHX
; Darkens a 16 bit 1555 color (0 = black, 255 = same color)
; Arguments:
;  arg0 : 16 bit color
;  arg1 : 8 bit change amount
; Returns:
;  16 bit color value
	pop	de			; de = return vector
	pop	bc			; bc = color
	ex	(sp),hl			; l = amt
	push	bc
	push	de			; Comments assume 1555 RGB color
_Darken:
	push	bc			; Calculate the output blue value
	ld	a,c			; a = color & $FF
	ld	c,l			; c = amt
	and	a,31
	ld	h,a			; h = blue
	mlt	hl			; hl = blue * amt
	ld	de,128			; de = 128
	add	hl,de			; hl = blue * amt + 128
	ld	l,h
	ld	h,d			; hl = (blue * amt + 128) / 256 = blue_out
	ex	(sp),hl			; hl = color, tmp1 = blue_out
					; Isolate the input red value
	ld	a,h			; a = color >> 8
	rra				; a = color >> 9
	and	a,62
	ld	b,a			; b = red << 1
					; Calculate the output green value
	add.s	hl,hl
	rla				; a & 1 = green & 1
	add	hl,hl
	add	hl,hl			; hl = color << 3
	rra
	ld	a,h
	rla
	and	a,63
	ld	h,a			; h = green
	ld	l,c			; l = amt
	mlt	hl			; hl = green * amt
	add	hl,de			; hl = green * amt + 128
	ld	l,h			; l = (green * amt + 128) / 256 = green_out
					; Calculate the output red value
	mlt	bc			; bc = red * amt << 1
	inc	b			; b = (red * amt + 128 << 1) / 256
	srl	b			; b = (red * amt + 128) / 256 = red_out
					; Position the output red and green bits
	add	hl,hl
	add	hl,hl			; l = green_out << 2
	ld	h,b			; h = red_out
	add	hl,hl
	add	hl,hl			; hl = (red_out << 10) | (green_out << 4)
	bit	4,l
	jr	z,.out
	set	7,h
	res	4,l			; hl = (green_out & 1 << 15) | (red_out << 10) | (green_out >> 1 << 5)
.out:
					; Add the output blue value (no positioning necessary) for the final output color
	pop	bc			; bc = blue_out
	add	hl,bc			; hl = color_out
	ret

;-------------------------------------------------------------------------------
gfy_SetColor: ; COPIED_FROM_GRAPHX
; Sets the global color index for all routines
; Arguments:
;  arg0 : Global color index
; Returns:
;  Previous global color index
	setSmcBytesFast _Color

;-------------------------------------------------------------------------------
gfy_SetTransparentColor: ; COPIED_FROM_GRAPHX
; Sets the global transparent color index for all routines
; Arguments:
;  arg0 : Transparent color index
; Returns:
;  Previous transparent color index

	setSmcBytes _TransparentColor

;-------------------------------------------------------------------------------
gfy_FillScreen: ; COPIED_FROM_GRAPHX
; Fills the screen with the specified color index
; Arguments:
;  arg0 : Color index
; Returns:
;  None

FillScreen_PushesPerIter := 115		; see fillscreen.xlsx for derivation
FillScreen_NumIters      := (LcdSize-InterruptStackSize)/(FillScreen_PushesPerIter*3)
FillScreen_BytesToPush   := FillScreen_PushesPerIter*3*FillScreen_NumIters
FillScreen_BytesToLddr   := LcdSize-FillScreen_BytesToPush

	ld	iy,0
	add	iy,sp			; iy = original sp
	ld	hl,FillScreen_FastCode_SrcEnd-1
	ld	de,FillScreen_FastCode_DestEnd-1
	ld	bc,FillScreen_FastCode_SrcSize
	lddr				; copy fast code after push run
					; de = pointer second to last push
					; bc = 0
	push	de
	pop	hl
	inc	hl			; hl  = pointer to last push (already copied)
	ld	c,FillScreen_PushesPerIter-1
	lddr				; fill push run
	ld	a,$E1
	ld	(de),a			; write initial pop hl
	ld	hl,(CurrentBuffer)
	ld	de,LcdSize
	add	hl,de			; hl = end (exclusive) of buffer
	ld	de,(iy+1)		; deu = color
	ld	d,(iy+3)		; d = color
	ld	e,d			; e = color
	ld	b,FillScreen_NumIters	; b = number of fast code iterations
	call	gfy_Wait
	ld	sp,hl			; sp = end (exclusive) of buffer
	call	_FillScreen_FastCode_Dest ; do fast fill
	sbc	hl,hl
	add	hl,sp			; hl = pointer to last byte fast-filled
	ld	sp,iy			; sp = original sp
	push	hl
	pop	de
	dec	de			; de = pointer to first byte to slow-fill
	ld	bc,FillScreen_BytesToLddr
	lddr				; finish with slow fill
	ret

_FillScreen_FastCode_Src:
	org	ti.mpLcdCrsrImage
_FillScreen_FastCode_Dest:
	org	$ + 1
.loop:
	push	de
	org	$ + FillScreen_PushesPerIter - 1
	djnz	.loop
	jp	(hl)

FillScreen_FastCode_DestEnd  := $
FillScreen_FastCode_DestSize := FillScreen_FastCode_DestEnd-_FillScreen_FastCode_Dest
FillScreen_FastCode_SrcSize  := FillScreen_FastCode_DestSize-(FillScreen_PushesPerIter-1+1)
	org _FillScreen_FastCode_Src+FillScreen_FastCode_SrcSize
FillScreen_FastCode_SrcEnd   := $

;-------------------------------------------------------------------------------
gfy_ZeroScreen: ; COPIED_FROM_GRAPHX
; Fills the screen with color index 0
; Arguments:
;  None
; Returns:
;  None
	ld	l,0
	push	hl
	call	gfy_FillScreen
	pop	hl
	ret

;-------------------------------------------------------------------------------
gfy_SetPalette: ; COPIED_FROM_GRAPHX
; Sets the palette colors
; Arguments:
;  arg0 : Pointer to palette
;  arg1 : Size of palette in bytes
;  arg2 : Palette index to insert at
; Returns:
;  None
	pop	iy			; iy = return vector
	pop	de			; de = src
	pop	bc			; bc = size
	ex	(sp),hl			; l = offset
	push	bc
	push	de
	ld	a,l
assert ti.mpLcdPalette and 1 = 0
	ld	hl,ti.mpLcdPalette shr 1
	ld	l,a			; hl = (palette >> 1) + offset
	add	hl,hl			; hl = &palette[offset] = dest
	ex	de,hl			; de = dest, hl = src
	ldir
	jp	(iy)

;-------------------------------------------------------------------------------
gfy_GetPixel:	; (uint24_t x, uint8_t y)
; Gets the color index of a pixel
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
; Returns:
;  Color index of X,Y coordinate
	ld	hl, 3
	add	hl, sp		; hl = &x
	inc.s	de
	ld	c, (hl)
	inc	hl
	ld	b, (hl)		; bc = (uint16_t)x
	inc	hl
	inc	hl		; hl = &y
	ld	e, (hl)		; e = y
	ld	d, 0
_GetPixel:
	; UBC = 0xx
	; E = 00y
	dec	b		; tests if x >= 256
	jr	nz, .x_lt_256
	ld	d, $F0		; ti.lcdHeight * 256
.x_lt_256:
	ld	b, ti.lcdHeight
	mlt	bc
	ld	hl, (CurrentBuffer)
	add	hl, bc		; add x cord
	add	hl, de		; add y cord
; No clipping is necessary, because if the pixel is offscreen, the result is
; undefined. All that is necessary is to ensure that there are no side effects
; of reading outside of the buffer. In this case, the largest possible offset
; into the buffer is (256 + 255) * lcdHeight + 255 = 122895 bytes. Even in the case
; that the current buffer is the second half of VRAM, the largest that this
; pointer can be is $D52C00 + 122895 = $D70C0F. This goes beyond the end of
; mapped RAM, but only into unmapped memory with no read side effects.
	ld	a,(hl)	; a = buffer[y][x]
	ret

;-------------------------------------------------------------------------------
gfy_SetPixel:
; Sets the color pixel to the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
; Returns:
;  None

	or	a, a
_NoCarry_gfy_SetPixel:
	sbc	hl, hl
_HLZero_gfy_SetPixel:
	ld	de, 3
	add	hl, de
	add	hl, sp
	ld	bc, (hl)	; bc = x coordinate
	add	hl, de		; move to next argument
	ld	e, (hl)		; e = y coordinate		
_SetPixel:
	wait_quick
_SetPixel_NoWait:
	ld	hl, -ti.lcdWidth
	add	hl, bc
	ret	c			; return if out of bounds
	ld	hl, -ti.lcdHeight
	add	hl, de
	ret	c			; return if out of bounds
_SetPixel_NoClip_NoWait:
	ld	hl, (CurrentBuffer)
	
	dec	b		; tests if x >= 256
	jr	nz, .x_lt_256
	ld	d, $F0		; ti.lcdHeight * 256
.x_lt_256:
	ld	b, ti.lcdHeight
	mlt	bc
	ld	hl, (CurrentBuffer)
	add	hl, bc		; add x cord
	add	hl, de		; add y cord
	ld	(hl), 0		; get the actual pixel
smcByte _Color
	ret

;-------------------------------------------------------------------------------
gfy_Rectangle_NoClip:
; Draws an unclipped rectangle outline with the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Width
;  arg3 : Height
; Returns:
;  None
	ld	iy,0
	add	iy,sp
	ld	a,(iy+12)		; a = height
	or	a,a
	ret	z			; abort if height == 0
	ld	bc,(iy+9)		; bc = width
	sbc	hl,hl
	adc	hl,bc
	ret	z			; abort if width == 0
	push	bc
	call	_HorizLine_NoClip_NotDegen_StackXY ; draw top horizontal line
						   ; hl = &buf[y][x+width-1]
	ld	b,a			; b = height
	call	_VertLine_NoClip_Draw	; draw right vertical line
	ld	b,(iy+12)		; b = height
	ld	e,(iy+6)		; e = y
	call	_VertLine_NoClip_NotDegen_StackX ; draw left vertical line
						 ; hl = &buf[y+height][x]
						 ; de = ti.lcdWidth
	sbc	hl,de			; hl = &buf[y+height-1][x]
	pop	bc			; bc = width
	jp	_HorizLine_NoClip_Draw	; draw bottom horizontal line

;-------------------------------------------------------------------------------
gfy_Rectangle: ; COPIED_FROM_GRAPHX
; Draws an clipped rectangle outline with the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Width
;  arg3 : Height
; Returns:
;  None
	push	ix			; need to use ix because lines use iy
	ld	ix,0
	add	ix,sp
	ld	hl,(ix+6)
	ld	de,(ix+9)
	ld	bc,(ix+12)
	push	bc
	push	de
	push	hl
	call	gfy_HorizLine		; top horizontal line
	ld	hl,(ix+6)
	ld	de,(ix+9)
	ld	bc,(ix+15)
	push	bc
	push	de
	push	hl
	call	gfy_VertLine		; left vertical line
	ld	hl,(ix+6)
	ld	de,(ix+9)
	ld	bc,(ix+12)
	add	hl,bc			; add x and width
	dec	hl
	ld	bc,(ix+15)
	push	bc
	push	de
	push	hl
	call	gfy_VertLine		; right vertical line
	ld	de,(ix+6)
	ld	hl,(ix+9)
	ld	bc,(ix+15)
	add	hl,bc
	dec	hl			; add y and height
	ld	bc,(ix+12)
	push	bc
	push	hl
	push	de
	call	gfy_HorizLine		; bottom horizontal line
	ld	sp,ix
	pop	ix
	ret

;-------------------------------------------------------------------------------
if 0
gfy_FillRectangle: ; COPIED_FROM_GRAPHX
; Draws a clipped rectangle with the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Width
;  arg3 : Height
; Returns:
;  None
	ld	iy,0
	add	iy,sp
	ld	hl,(iy+9)		; hl = width
	ld	de,(iy+3)		; de = x coordinate
	add	hl,de
	ld	(iy+9),hl
	ld	hl,(iy+12)		; hl = height
	ld	de,(iy+6)		; de = y coordinate
	add	hl,de
	ld	(iy+12),hl
	call	_ClipRegion
	ret	c			; return if offscreen or degenerate
	ld	de,(iy+3)
	ld	hl,(iy+9)
	sbc	hl,de
	push	hl
	ld	de,(iy+6)
	ld	hl,(iy+12)
	sbc	hl,de
	pop	bc			; bc = new width
	ld	a,l			; a = new height
	ld	hl,(iy+3)		; hl = new x, de = new y
	jr	_FillRectangle_NoClip

;-------------------------------------------------------------------------------
gfy_FillRectangle_NoClip:
; Draws an unclipped rectangle with the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Width
;  arg3 : Height
; Returns:
;  None
	ld	iy, 0
	add	iy, sp
	ld	a, (iy+12)		; a = height
	or	a, a
	ret	z			; make sure height is not 0
	ld	bc, (iy+9)		; bc = width
	sbc	hl, hl
	adc	hl, bc
	ret	z			; make sure width is not 0
	ld	hl, (iy+3)		; hl = x coordinate
	ld	e, (iy+6)		; e = y coordinate
_FillRectangle_NoClip:
	ld	d, h		; maybe ld d, 0
	dec	h		; tests if x >= 256
	ld	h, ti.lcdHeight
	jr	nz, .x_lt_256
	ld	d, h		; ld d, ti.lcdHeight * 256
.x_lt_256:
	mlt	hl
	ex.s	de, hl		; clear upper byte of DE
	add	hl, de		; add y cord
	ld	de, (CurrentBuffer)
	add	hl, de		; add buffer offset
	ex	de, hl			; de -> place to begin drawing
	push	de
	
	ld	(.height1), a
	ld	(.height2), a
	ld	hl, _Color
	; swap width and height
	ld	b, c
	ld	c, a
	ld	a, b
	ld	b, 0
	; a = width
	; bc = height
	wait_quick
	ldi				; check if we only need to draw 1 pixel
	pop	hl
	jp	po, .skip
	ldir
.skip:
	dec	a
	ret	z
	ld	c, ti.lcdHeight
.loop:
	add	hl, bc
	dec	de
	ex	de, hl
.height1 = $ + 1
	ld	bc, 0
	lddr
	dec	a
	ret	z
	; ld bc, (2 * ti.lcdHeight) + 1
	inc	b
	ld	c, $E1
	add	hl, bc
	inc	de
	ex	de, hl
	
.height2 = $ + 1
	ld	bc, 0
	ldir
	; ld bc, (2 * ti.lcdHeight) - 1
	inc	b
	ld	c, $DF
	dec	a
	jr	nz, .loop

	ld	a, (iy+10)
	dec	a
	ret	nz	; width < 256
	xor	a, a
	jr	.loop

end if

;-------------------------------------------------------------------------------
gfy_HorizLine:
; Draws an clipped horizontal line with the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Length
; Returns:
;  None
	ld	iy,0
	add	iy,sp
	ld	hl,(iy+6)
	ld	de,ti.lcdHeight
smcWord _YMax
	sbc	hl,de			; subtract maximum y
	ld	de,ti.lcdHeight		; add y bounds span
smcWord _YSpan
	add	hl,de
	ret	nc			; return if not within y bounds
	ld	hl,(iy+9)
	ld	de,(iy+3)
	add	hl,de
	push	hl
	ld	hl,0
smcWord _XMin
	call	_Maximum		; get minimum x
	ex	(sp),hl
	ld	de,ti.lcdWidth
smcWord _XMax
	call	_Minimum		; get maximum x
	pop	de
	scf
	sbc	hl,de
	ret	c
	inc	hl
	push	hl
	pop	bc			; bc = length
	ex	de,hl
	jr	_HorizLine_NoClip_NotDegen_StackY

;-------------------------------------------------------------------------------
gfy_HorizLine_NoClip:
; Draws an unclipped vertical line with the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Length
; Returns:
;  None
	ld	iy, 0
	add	iy, sp
	ld	bc, (iy+9)		; b = length
_HorizLine_NoClip_StackXY:
	xor	a, a
	or	a, b
_HorizLine_NoClip_MaybeDegen_StackXY:
	ret	z			; abort if length == 0
_HorizLine_NoClip_NotDegen_StackXY:
	ld	hl,(iy+3)		; hl = x
_HorizLine_NoClip_NotDegen_StackY:
	ld	e, (iy+6)		; e = y
_HorizLine_NoClip_NotDegen:
	ld	d, h		; maybe ld d, 0
	dec	h		; tests if x >= 256
	ld	h, ti.lcdHeight
	jr	nz, .x_lt_256
	ld	d, h		; ld d, ti.lcdHeight * 256
.x_lt_256:
	mlt	hl
	ex.s	de, hl		; clear upper byte of DE
	add	hl, de		; add y cord
	ld	de, (CurrentBuffer)
	add	hl, de		; add buffer offset
	
_HorizLine_NoClip_Draw:
	ld	de, ti.lcdHeight
	; swap b and c
	ld	a, b
	ld	b, c
	ld	c, a

	ld	a, 0
smcByte _Color
	wait_quick
.loop:
	ld	(hl), a			; loop for width
	add	hl, de
	djnz	.loop
	dec	c
	ret	nz	; length < 256
	ld	b, 0
	jr	.loop
	
;-------------------------------------------------------------------------------
gfy_VertLine:
; Draws an clipped vertical line with the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Length
; Returns:
;  None
	ld	iy,0
	add	iy,sp
	ld	hl,(iy+3)
	ld	de,ti.lcdWidth
smcWord _XMax
	sbc	hl,de			; subtract maximum x
	ld	de,ti.lcdWidth
smcWord _XSpan
	add	hl,de			; add x bounds span
	ret	nc			; return if not within x bounds
	ld	hl,(iy+9)
	ld	de,(iy+6)
	add	hl,de
	push	hl
	ld	hl,0
smcWord _YMin
	call	_Maximum		; get minimum y
	ex	(sp),hl
	ld	de,ti.lcdHeight
smcWord _YMax
	call	_Minimum		; get maximum y
	pop	de
	ld	a,l
	sub	a,e
	ret	c			; return if not within y bounds
	ld	bc, 0
	ld	c, a
	jr	_VertLine_NoClip_MaybeDegen_StackX	; from GraphX

;-------------------------------------------------------------------------------
; gfy_VertLine_NoClip:

gfy_VertLine_NoClip:
; Draws an unclipped horizontal line with the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Length
; Returns:
;  None
	ld	iy, 0
	add	iy, sp
	ld	bc, 0
	ld	c, (iy+9)		; bc = length
_VertLine_NoClip_StackXY:
	sbc	hl, hl
	ex	de, hl
	sbc	hl, hl
	adc	hl, bc
	ld	e, (iy+6)		; e = y
_VertLine_NoClip_MaybeDegen_StackX:
	ret	z			; abort if length == 0
_VertLine_NoClip_NotDegen_StackX:
	ld	hl, (iy+3)		; hl = x
_VertLine_NoClip_NotDegen:
	wait_quick
_VertLine_NoClip_NotDegen_NoWait:
	ld	d, h		; maybe ld d, 0
	ld	d, 0	; DEBUG
	dec	h		; tests if x >= 256
	ld	h, ti.lcdHeight
	jr	nz, .x_lt_256
	ld	d, h		; ld d, ti.lcdHeight * 256
.x_lt_256:
	mlt	hl
	ex.s	de, hl		; clear upper byte of DE
	add	hl, de		; add y cord
	ld	de, (CurrentBuffer)
	add	hl, de		; add buffer offset

_VertLine_NoClip_Draw:
	ld	(hl), 0
smcByte _Color
	cpi
	ex	de, hl
	ld	hl, -1
	add	hl, de
; Delay 1-wide early return for consistent register output values.
	ret	po
	ldir
	ret

;-------------------------------------------------------------------------------
gfy_SetDraw: ; COPIED_FROM_GRAPHX
; Forces drawing routines to operate on the offscreen buffer
; or to operate on the visible screen
; Arguments:
;  arg0: buffer or screen
; Returns:
;  None
	pop	de
	ex	(sp),hl
	ld	a,l
	or	a,a
	ld	hl,(ti.mpLcdBase)	; get current base
	ld	bc,ti.vRam
	jr	z,.match
	sbc	hl,bc
	jr	nz,.swap		; if not the same, swap
.set:
	ld	bc,ti.vRam + LcdSize
.swap:
	ld	(CurrentBuffer),bc
	ex	de,hl
	jp	(hl)
.match:
	sbc	hl,bc
	jr	z,.swap			; if the same, swap
	jr	.set

;-------------------------------------------------------------------------------
gfy_GetDraw: ; COPIED_FROM_GRAPHX
; Gets the current drawing state
; Arguments:
;  None
; Returns:
;  Returns true if drawing on the buffer
	ld	a,(ti.mpLcdBase+2)	; comparing upper byte only is sufficient
	ld	hl,CurrentBuffer+2
	xor	a,(hl)			; always 0 or 1
	ret

;-------------------------------------------------------------------------------
_WaitQuick:
	ex	(sp),hl			; hl saved, hl = return vector
	push	de			; de saved
	ld	de,gfy_Wait
	dec	hl
	dec	hl
	dec	hl
	ld	(hl),de			; call _WaitQuick -> call _Wait
	dec	hl			; hl = callee
	ex	de,hl			; de = callee
	ld	hl,_WriteWaitQuickSMC
.WriteWaitsTail = $-3
	ld	(hl),$22		; ld (callee),hl
	inc	hl
	ld	(hl),de
	inc	hl
	inc	hl
	inc	hl
	ld	(.WriteWaitsTail),hl
	ex	de,hl			; hl = callee
	pop	de			; de restored
	ex	(sp),hl			; return vector = callee, hl restored
; Fall through to gfy_Wait, but don't let it return immediately. Even if it ends
; up not waiting, it will re-write the quick wait SMC, including for the callee.
	push	hl
;	jr	gfy_Wait+1		; emulated by dummifying next instruction:
	db	$2E			; ret || push hl -> ld l,*

;-------------------------------------------------------------------------------
gfy_Wait: ; COPIED_FROM_GRAPHX
; Waits for the screen buffer to finish being displayed after gfy_SwapDraw
; Arguments:
;  None
; Returns:
;  None
	ret				; will be SMC'd into push hl
	push	af
	ld	a,(ti.mpLcdRis)
	bit	ti.bLcdIntVcomp,a
	jr	nz,.WaitDone
	push	de
.WaitLoop:
.ReadLcdCurr:
	ld	a,(ti.mpLcdCurr + 2)	; a = *mpLcdCurr>>16
	ld	hl,(ti.mpLcdCurr + 1)	; hl = *mpLcdCurr>>8
	sub	a,h
	jr	nz,.ReadLcdCurr		; nz ==> lcdCurr may have updated
					;        mid-read; retry read
	ld	de,(CurrentBuffer + 1)
	sbc	hl,de
	ld	de,-LcdSize shr 8
	add	hl,de
	jr	nc,.WaitLoop
	pop	de
.WaitDone:
	ld	a,$C9			; ret
	ld	(gfy_Wait),a		; disable wait logic
	pop	af
	ld	hl,$0218		; jr $+4
_WriteWaitQuickSMC:
repeat wait_quick.usages
; Each call _WaitQuick will replace the next unmodified 4-byte entry with
; ld (_WaitQuick_callee_x),hl.
	pop	hl
	ret
	nop
	nop
end repeat
	pop	hl
	ret

;-------------------------------------------------------------------------------
gfy_SwapDraw: ; COPIED_FROM_GRAPHX
; Swaps the roles of the screen and drawing buffers
; Arguments:
;  None
; Returns:
;  None
	ld	iy,ti.mpLcdRange
.WaitLoop:
	bit	ti.bLcdIntLNBU,(iy + ti.lcdRis)
	jr	z,.WaitLoop
assert ti.vRam and $FF = 0
assert LcdSize and $FF = 0
	ld	bc,(iy-ti.mpLcdRange+CurrentBuffer+1) ; bc = old_draw>>8
.LcdSizeH := (LcdSize shr 8) and $FF
assert .LcdSizeH and ti.lcdIntVcomp
assert .LcdSizeH and ti.lcdIntLNBU
	ld	a,.LcdSizeH		; a = LcdSize>>8
	ld	(iy+ti.lcdBase+1),bc	; screen = old_draw
	ld	(iy+ti.lcdIcr),a	; clear interrupt statuses to wait for
	xor	a,c			; a = (old_draw>>8)^(LcdSize>>8)
	ld	c,a			; c = (old_draw>>8)^(LcdSize>>8)
	inc	b
	res	1,b			; b = (old_draw>>16)+1&-2
					; assuming !((old_draw>>16)&2):
					;   = (old_draw>>16)^1
					;   = (old_draw>>16)^(LcdSize>>16)
					; bc = (old_draw>>8)^(LcdSize>>8)
					;    = new_draw>>8
	ld	(iy-ti.mpLcdRange+CurrentBuffer+1),bc
	ld	hl,gfy_Wait
	ld	(hl),$E5		; push hl; enable wait logic
	push	hl
	dec	sp
	pop	hl
	ld	l,$CD			; call *
					; hl = first 3 bytes of call _Wait
	dec	sp
	dec	sp			; sp -= 3 to match pop hl later
	jr	_WriteWaitQuickSMC

;-------------------------------------------------------------------------------
gfy_FillEllipse_NoClip: ; COPIED_FROM_GRAPHX
	ld	hl,gfy_HorizLine_NoClip
	db	$FD			; ld hl,* -> ld iy,*

;-------------------------------------------------------------------------------
gfy_FillEllipse: ; COPIED_FROM_GRAPHX
	ld	hl,gfy_HorizLine
	ld	(_ellipse_line_routine_1),hl
	ld	(_ellipse_line_routine_2),hl
	ld	hl,_ellipse_draw_line
	ld	(_ellipse_loop_draw_2),hl
	ld	(_ellipse_loop_draw_3),hl
	ld	hl,_ellipse_ret
	ld	(_ellipse_loop_draw_1),hl
	jr	_Ellipse
	
;-------------------------------------------------------------------------------
gfy_Ellipse_NoClip: ; COPIED_FROM_GRAPHX
	ld	hl,_SetPixel_NoClip_NoWait
	db	$FD		; ld hl,* -> ld iy,*

;-------------------------------------------------------------------------------
gfy_Ellipse: ; COPIED_FROM_GRAPHX
	ld	hl,_SetPixel_NoWait
	ld	(_ellipse_pixel_routine_1),hl
	ld	(_ellipse_pixel_routine_2),hl
	ld	(_ellipse_pixel_routine_3),hl
	ld	(_ellipse_pixel_routine_4),hl
	ld	hl,_ellipse_draw_pixels
	ld	(_ellipse_loop_draw_1),hl
	ld	(_ellipse_loop_draw_3),hl
	ld	hl,_ellipse_ret
	ld	(_ellipse_loop_draw_2),hl

el_x		:= 3		; Current X coordinate of the ellipse
el_y		:= 6		; Current Y coordinate of the ellipse
el_a2		:= 9		; X radius squared
el_b2		:= 12		; Y radius squared
el_fa2		:= 15		; X radius squared * 4
el_fb2		:= 18		; Y radius squared * 4
el_sigma	:= 21		; Sigma
el_sigma_1	:= 24		; Offset to be added to sigma in loop 1
el_sigma_2	:= 27		; Offset to be added to sigma in loop 2
el_temp1	:= 30		; X radius as "int" instead of "uint8_t"
el_comp_a	:= 33		; b2 * x
el_comp_b	:= 36		; a2 * y
el_sigma_diff1	:= 39		; Offset to be added to sigma in loop 1
el_sigma_diff2	:= 42		; Offset to be added to sigma in loop 2

_Ellipse:
; Draws an ellipse, either filled or not, either clipped or not
; Arguments:
;  arg0 : X coordinate (ix+6)
;  arg1 : Y coordinate (ix+9)
;  arg2 : X radius (ix+12)
;  arg3 : Y radius (ix+15)
; Returns:
;  None
	push	ix
	ld	ix,0
	add	ix,sp
	lea	hl,ix - 42
	ld	sp,hl
	
; First, setup all the variables
	ld	a,(ix + 12)
	or	a,a
	jr	nz,.valid_x_radius
.return:
	ld	sp,ix
	pop	ix
	ret
.valid_x_radius:
	ld	l,a
	ld	h,a
	mlt	hl
	ld	(ix - el_a2),hl		; int a2 = a * a;
	add	hl,hl
	ld	(ix - el_sigma_diff2),hl; Save a2 * 2 for later
	add	hl,hl
	ld	(ix - el_fa2),hl	; int fa2 = 4 * a2;
	ld	a,(ix + 15)
	or	a,a
	jr	z,.return		; Make sure Y radius is not 0
	ld	e,a
	ld	d,1
	mlt	de
	ld	(ix - el_y),de		; int y = b;
	ld	hl,(ix - el_a2)
	ld	d,l
	ld	l,e
	mlt	de
	mlt	hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,de
	add	hl,hl
	add	hl,hl
	ex	de,hl
	ld	hl,(ix - el_fa2)
	or	a,a
	sbc	hl,de
	ld	(ix - el_sigma_1),hl	; int sigma_add_1 = fa2 * (1 - b);
	ld	l,a
	ld	h,a
	mlt	hl
	ld	(ix - el_b2),hl		; int b2 = b * b;
	add	hl,hl
	ld	(ix - el_sigma_diff1),hl	; Save b2 * 2 for later
	add	hl,hl
	ld	(ix - el_fb2),hl	; int fb2 = 4 * b2;
	ld	c,a
	ld	b,2
	mlt	bc
	or	a,a
	sbc	hl,hl
	ld	(ix - el_x),hl		; int x = 0;
	ld	(ix - el_comp_a),hl
	inc	hl
	sbc	hl,bc
	ld	bc,(ix - el_a2)
	call	_MultiplyHLBC
	ld	bc,(ix - el_b2)
	add	hl,bc
	add	hl,bc
	ld	(ix - el_sigma),hl	; int sigma = 2 * b2 + a2 * (1 - 2 * b);
	ld	e,(ix + 12)
	ld	d,1
	mlt	de
	ld	(ix - el_temp1),de	; Save int a for later
	or	a,a
	sbc	hl,hl
	inc	hl
	sbc	hl,de
	ld	bc,(ix - el_fb2)
	call	_MultiplyHLBC
	ld	(ix - el_sigma_2),hl	; int sigma_add_2 = fb2 * (1 - a);

	ld	hl,(ix - el_a2)
	ld	bc,(ix - el_y)
	call	_MultiplyHLBC
	ld	(ix - el_comp_b),hl
	
	wait_quick

.main_loop1:
	call	0
_ellipse_loop_draw_1 := $-3

; Eventually change sigma and y
	ld	hl,(ix - el_sigma)
	add	hl,hl
	jr	c,.loop1_jump		; if (sigma >= 0) {

	call	0
_ellipse_loop_draw_2 := $-3

	ld	hl,(ix - el_sigma)	; sigma += sigma_add_1;
	ld	de,(ix - el_sigma_1)
	add	hl,de
	ld	(ix - el_sigma),hl
	ld	hl,(ix - el_fa2)
	add	hl,de
	ld	(ix - el_sigma_1),hl	; sigma_add_1 += fa2;
	ld	hl, (ix - el_y)
	dec	hl
	ld	(ix - el_y),hl		; y--;
	ld	hl,(ix - el_comp_b)
	ld	de,(ix - el_a2)
	or	a,a
	sbc	hl,de
	ld	(ix - el_comp_b),hl
.loop1_jump:				; }
; Change sigma and increment x
	ld	hl,(ix - el_sigma_diff1)
	ld	de,(ix - el_fb2)
	add	hl,de
	ld	(ix - el_sigma_diff1),hl
	ld	de,(ix - el_sigma)
	add	hl,de
	ld	(ix - el_sigma),hl	; sigma += b2 * (4 * x + 6);
	ld	hl,(ix - el_x)
	inc	hl
	ld	(ix - el_x),hl		; x++;

; Update the comparison operands
	ld	hl,(ix - el_comp_a)
	ld	de,(ix - el_b2)
	add	hl,de
	ld	(ix - el_comp_a),hl
	ld	de,(ix - el_comp_b)

; And compare
	ld	bc,0x800000		; b2 * x <= a2 * y so hl <= de
	add	hl,bc
	ex	de,hl			; de <= hl
	add	hl,bc
	or	a,a
	sbc	hl,de
	jq	nc,.main_loop1

; Update few variables for the next loop
	ld	hl, (ix - el_temp1)
	ld	(ix - el_x),hl		; x = a
	ld	e,l
	or	a,a
	sbc	hl,hl
	ld	(ix - el_y),hl		; y = 0
	ld	(ix - el_comp_a),hl
	ld	d,2
	mlt	de
	inc	hl
	sbc	hl,de
	ld	bc,(ix - el_b2)
	call	_MultiplyHLBC
	ld	de,(ix - el_a2)
	add	hl,de
	add	hl,de
	ld	(ix - el_sigma), hl

	ld	hl,(ix - el_b2)
	ld	bc,(ix - el_temp1)
	call	_MultiplyHLBC
	ld	(ix - el_comp_b),hl

.main_loop2:
	call	0
_ellipse_loop_draw_3 := $-3

; Eventually update sigma and x
	ld	hl,(ix - el_sigma)
	add	hl,hl
	jr	c,.loop2_jump		; if (sigma >= 0) {
	ld	hl,(ix - el_sigma)
	ld	de,(ix - el_sigma_2)
	add	hl,de
	ld	(ix - el_sigma),hl	; sigma += sigma_add_2;
	ld	hl,(ix - el_fb2)
	add	hl,de
	ld	(ix - el_sigma_2),hl	; sigma_add_2 += fb2;
	ld	hl, (ix - el_x)
	dec	hl
	ld	(ix - el_x),hl		; x--;
	ld	hl,(ix - el_comp_b)
	ld	de,(ix - el_b2)
	or	a,a
	sbc	hl,de
	ld	(ix - el_comp_b),hl
.loop2_jump:
; Change sigma and increment y
	ld	hl,(ix - el_sigma_diff2)
	ld	de,(ix - el_fa2)
	add	hl,de
	ld	(ix - el_sigma_diff2),hl
	ld	de,(ix - el_sigma)
	add	hl,de
	ld	(ix - el_sigma),hl	; sigma += a2 * (4 * y + 6);
	ld	hl,(ix - el_y)
	inc	hl
	ld	(ix - el_y),hl		; y++;
	ld	hl,(ix - el_comp_a)
	ld	de,(ix - el_a2)
	add	hl,de
	ld	(ix - el_comp_a),hl

; Compare the boolean operators
	ld	de,(ix - el_comp_b)
	ld	bc,0x800000
	add	hl,bc
	ex	de,hl
	add	hl,bc
	or	a,a
	sbc	hl,de
	jq	nc,.main_loop2

	ld	sp,ix
	pop	ix
_ellipse_ret:
	ret

_ellipse_draw_pixels:
; bc = x coordinate
; e = y coordinate
	ld	hl,(ix + 9)
	ld	de,(ix - el_y)
	add	hl,de
	ex	de,hl
	push	de
	ld	hl,(ix + 6)
	ld	bc,(ix - el_x)
	add	hl,bc
	push	hl
	pop	bc
	call	_SetPixel_NoWait	; xc + x, yc + y
_ellipse_pixel_routine_1 := $-3
	pop	de
	ld	hl,(ix + 6)
	ld	bc,(ix - el_x)
	or	a,a
	sbc	hl,bc
	push	hl
	pop	bc
	push	bc
	call	_SetPixel_NoWait	; xc - x, yc + y
_ellipse_pixel_routine_2 := $-3
	pop	bc
	ld	hl,(ix + 9)
	ld	de,(ix - el_y)
	or	a,a
	sbc	hl,de
	ex	de,hl
	push	de
	call	_SetPixel_NoWait	; xc - x, yc - y
_ellipse_pixel_routine_3 := $-3
	pop	de
	ld	hl,(ix + 6)
	ld	bc,(ix - el_x)
	add	hl,bc
	push	hl
	pop	bc
	jp	_SetPixel_NoWait	; xc + x, yc - y
_ellipse_pixel_routine_4 := $-3

_ellipse_draw_line:
	ld	hl,(ix - el_x)
	add	hl,hl
	push	hl
	ld	hl,(ix + 9)
	ld	de,(ix - el_y)
	or	a,a
	sbc	hl,de
	push	hl
	ld	hl,(ix + 6)
	ld	de,(ix - el_x)
	or	a,a
	sbc	hl,de
	push	hl
	call	0
_ellipse_line_routine_1 := $-3
	pop	hl
	pop	hl
	pop	hl
	ld	hl,(ix - el_x)
	add	hl,hl
	push	hl
	ld	hl,(ix + 9)
	ld	de,(ix - el_y)
	add	hl,de
	push	hl
	ld	hl,(ix + 6)
	ld	de,(ix - el_x)
	or	a,a
	sbc	hl,de
	push	hl
	call	0
_ellipse_line_routine_2 := $-3
	pop	hl
	pop	hl
	pop	hl
	ret
	

;-------------------------------------------------------------------------------
gfy_Circle: ; COPIED_FROM_GRAPHX
; Draws a clipped circle outline
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
;  arg2 : Radius
; Returns:
;  None
	ld	iy,0
	add	iy,sp
	lea	hl,iy-9
	ld	sp,hl
	ld	bc,(iy+9)
	ld	(iy-6),bc
	sbc	hl,hl
	ld	(iy-3),hl
	adc	hl,bc
	jp	z,.exit
	ld	hl,1
	or	a,a
	sbc	hl,bc
	call	gfy_Wait
	jp	.next
.sectors:
	ld	bc,(iy+3)
	ld	hl,(iy-6)
	add	hl,bc
	push	hl
	push	hl
	pop	bc
	ld	de,(iy+6)
	ld	hl,(iy-3)
	add	hl,de
	ex	de,hl
	push	de
	call	_SetPixel_NoWait
	ld	bc,(iy+6)
	ld	hl,(iy-6)
	add	hl,bc
	ex	de,hl
	push	de
	ld	bc,(iy+3)
	ld	hl,(iy-3)
	add	hl,bc
	push	hl
	push	hl
	pop	bc
	call	_SetPixel_NoWait
	ld	bc,(iy-6)
	ld	hl,(iy+6)
	or	a,a
	sbc	hl,bc
	ex	de,hl
	pop	bc
	push	de
	call	_SetPixel_NoWait
	pop	de
	ld	bc,(iy-3)
	ld	hl,(iy+3)
	or	a,a
	sbc	hl,bc
	push	hl
	push	hl
	pop	bc
	call	_SetPixel_NoWait
	pop	bc
	pop	de
	call	_SetPixel_NoWait
	pop	de
	ld	bc,(iy-6)
	ld	hl,(iy+3)
	or	a,a
	sbc	hl,bc
	push	hl
	push	hl
	pop	bc
	call	_SetPixel_NoWait
	ld	bc,(iy-3)
	ld	hl,(iy+6)
	or	a,a
	sbc	hl,bc
	ex	de,hl
	pop	bc
	push	de
	call	_SetPixel_NoWait
	pop	de
	pop	bc
	call	_SetPixel_NoWait
	ld	bc,(iy-3)
	inc	bc
	ld	(iy-3),bc
	ld	bc,(iy-9)
	or	a,a
	sbc	hl,hl
	sbc	hl,bc
	jp	m,.cmp0
	jp	pe,.cmp1
	jr	.cmp2
.cmp0:
	jp	po,.cmp1
.cmp2:
	ld	hl,(iy-3)
	add	hl,hl
	inc	hl
	add	hl,bc
	jr	.next
.cmp1:
	ld	bc,(iy-6)
	dec	bc
	ld	(iy-6),bc
	ld	hl,(iy-3)
	or	a,a
	sbc	hl,bc
	add	hl,hl
	inc	hl
	ld	de,(iy-9)
	add	hl,de
.next:
	ld	(iy-9),hl
	ld	bc,(iy-3)
	ld	hl,(iy-6)
	or	a,a
	sbc	hl,bc
	jp	p,.check
	jp	pe,.sectors
	jr	.exit
.check:
	jp	po,.sectors
.exit:
	ld	sp,iy
	ret

;-------------------------------------------------------------------------------
; gfy_FillCircle:

;-------------------------------------------------------------------------------
; gfy_FillCircle_NoClip:

; ...

_ResetStack:
	ld	sp,ix
	pop	ix
	ret

;-------------------------------------------------------------------------------
; gfy_Line:

;-------------------------------------------------------------------------------
; gfy_Line_NoClip:

;-------------------------------------------------------------------------------
gfy_Blit: ; COPIED_FROM_GRAPHX
; Copies the buffer image to the screen and vice versa
; Arguments:
;  arg0 : Buffer to copy from (screen = 0, buffer = 1)
; Returns:
;  None
	pop	iy			; iy = return vector
	ex	(sp),hl
	ld	a,l			; a = buffer to blit from
	call	util.getbuffer		; determine blit buffers
	ld	bc,LcdSize
util.blit:
	call	gfy_Wait
	ldir				; just do it
	jp	(iy)

;-------------------------------------------------------------------------------
; gfy_BlitLines:

;-------------------------------------------------------------------------------
; gfy_BlitColumns:

;-------------------------------------------------------------------------------
; gfy_BlitRectangle:

;-------------------------------------------------------------------------------
; gfy_CopyRectangle:

;-------------------------------------------------------------------------------
; gfy_ShiftLeft:

;-------------------------------------------------------------------------------
; gfy_ShiftUp:

;-------------------------------------------------------------------------------
; gfy_ShiftRight:

;-------------------------------------------------------------------------------
; gfy_ShiftDown:

;-------------------------------------------------------------------------------
gfy_GetClipRegion: ; COPIED_FROM_GRAPHX
; Arguments:
;  Pointer to struct
; Returns:
;  False if offscreen
	ld	hl,3
	add	hl,sp
	ld	iy,(hl)
	dec	iy
	dec	iy
	dec	iy
	call	_ClipRegion		; get the clipping region
	sbc	a,a			; return false if offscreen (0)
	inc	a
	ret

;-------------------------------------------------------------------------------
; gfy_ScaledSprite_NoClip:

;-------------------------------------------------------------------------------
; gfy_ScaledTransparentSprite_NoClip:

; ...

	cp	a,TRASPARENT_COLOR
smcByte _TransparentColor

;-------------------------------------------------------------------------------
; gfy_TransparentSprite:

; ...

	cp	a,TRASPARENT_COLOR
smcByte _TransparentColor

;-------------------------------------------------------------------------------
; gfy_Sprite:

;-------------------------------------------------------------------------------
; gfy_Sprite_NoClip:

;-------------------------------------------------------------------------------
; gfy_GetSprite:

;-------------------------------------------------------------------------------
; gfy_TransparentSprite_NoClip:

; ...

	cp	a,TRASPARENT_COLOR
smcByte _TransparentColor

;-------------------------------------------------------------------------------
; _ClipCoordinates:

;-------------------------------------------------------------------------------
; gfy_TransparentTilemap_NoClip:

;-------------------------------------------------------------------------------
; gfy_Tilemap_NoClip:

;-------------------------------------------------------------------------------
; gfy_TransparentTilemap:

;-------------------------------------------------------------------------------
; gfy_Tilemap:

; ...

t_data        := 0
t_type_width  := 10
t_type_height := 11
t_height      := 12
t_width       := 13
t_tile_height := 6
t_tile_width  := 7
t_draw_height := 8
t_draw_width  := 9
t_x_loc       := 15
x_offset      := 9
y_offset      := 12

;-------------------------------------------------------------------------------
gfy_TilePtr: ; COPIED_FROM_GRAPHX
; Returns a pointer to a tile given the pixel offsets
; Arguments:
;  arg0 : Tilemap Struct
;  arg1 : X Pixel Offset (Unsigned)
;  arg2 : Y Pixel Offset (Unsigned)
; Returns:
;  A pointer to an indexed tile in the tilemap (so it can be looked at or changed)
; C Function:
;  uint8_t *gfy_TilePtr(gfy_tilemap_t *tilemap, unsigned x_offset, unsigned y_offset) {
;      return &tilemap->map[(x_offset/tilemap->tile_width)+((y_offset/tilemap->tile_height)*tilemap->width)];
;  }
	push	ix
	ld	ix,0
	add	ix,sp
	ld	iy,(ix+6)
	ld	hl,(ix+9)
	ld	a,(iy+t_type_width)
	or	a,a
	jr	nz,.fastdiv0
	ld	bc,0
	ld	c,(iy+t_tile_width)
	call	ti._idvrmu
	ex	de,hl
	jr	.widthnotpow2
.fastdiv0:
	ld	b,a
.div0:
	srl	h
	rr	l
	djnz	.div0
.widthnotpow2:
	ex	de,hl
	ld	hl,(ix+12)
	ld	a,(iy+t_type_height)
	or	a,a
	jr	nz,.fastdiv1
	ld	bc,0
	ld	c,(iy+t_tile_height)
	push	de
	call	ti._idvrmu
	ex	de,hl
	pop	de
	jr	.heightnotpow2
.fastdiv1:
	ld	b,a
.div1:	srl	h
	rr	l
	djnz	.div1
.heightnotpow2:
	ld	h,(iy+t_width)
	mlt	hl
	add	hl,de
	ld	de,(iy+t_data)
	add	hl,de
	pop	ix
	ret

;-------------------------------------------------------------------------------
gfy_TilePtrMapped: ; COPIED_FROM_GRAPHX
; Returns a direct pointer to the input tile
; Arguments:
;  arg0 : Tilemap Struct
;  arg1 : X Map Offset
;  arg2 : Y Map Offset
; Returns:
;  A pointer to the indexed tile in the tilemap (so it can be looked at or changed)
	pop	de			; return vector
	pop	iy			; tilemap struct
	pop	bc			; x offset
	ex	(sp),hl			; y offset
	push	de
	push	de
	push	de
	ld	h,(iy+13)		; tilemap width
	mlt	hl
	ld	b,0
	add.s	hl,bc
	ld	bc,(iy+0)		; tilemap data
	add	hl,bc
	ret

;-------------------------------------------------------------------------------
gfy_GetTextX: ; COPIED_FROM_GRAPHX
; Gets the X position of the text cursor
; Arguments:
;  None
; Returns:
;  X Text cursor posistion
	ld	hl,(_TextXPos)		; return x pos
	ret

;-------------------------------------------------------------------------------
gfy_GetTextY: ; COPIED_FROM_GRAPHX
; Gets the Y position of the text cursor
; Arguments:
;  None
; Returns:
;  Y Text cursor posistion
	ld	hl,(_TextYPos)		; return y pos
	ret

;-------------------------------------------------------------------------------
gfy_SetTextXY: ; COPIED_FROM_GRAPHX
; Sets the text X and Y positions
; Arguments:
;  arg0 : Text X Pos
;  arg1 : Text Y Pos
; Returns:
;  None
	pop	de			; de=return address, sp=&xpos
	pop	hl			; hl=xpos, sp=&ypos
	ld	(_TextXPos),hl
	ex	(sp),hl			; hl=ypos, ypos=don't care
	ld	(_TextYPos),hl
	push	hl			; xpos=don't care, sp=&xpos
	ex	de,hl			; hl=return address
;-------------------------------------------------------------------------------
_indcallHL: ; COPIED_FROM_GRAPHX
; Calls HL
; Inputs:
;  HL : Address to call
	jp	(hl)

;-------------------------------------------------------------------------------
gfy_SetTextBGColor: ; COPIED_FROM_GRAPHX
; Sets the background text color for text routines
; Arguments:
;  arg0 : Color index to set BG to
; Returns:
;  Previous text color palette index
	setSmcBytes _TextBGColor

;-------------------------------------------------------------------------------
gfy_SetTextFGColor: ; COPIED_FROM_GRAPHX
; Sets the foreground text color for text routines
; Arguments:
;  arg0 : Color index to set FG to
; Returns:
;  Previous text color palette index
	setSmcBytes _TextFGColor

;-------------------------------------------------------------------------------
gfy_SetTextTransparentColor: ; COPIED_FROM_GRAPHX
; Sets the transparency text color for text routines
; Arguments:
;  arg0 : Color index to set transparent text to
; Returns:
;  Previous text color palette index
	setSmcBytes _TextTPColor

;-------------------------------------------------------------------------------
gfy_SetFontHeight: ; COPIED_FROM_GRAPHX
; Sets the height of the font in pixels
; Arguments:
;  arg0 : New font height
; Returns:
;  Previous font height
	setSmcBytes _TextHeight

;-------------------------------------------------------------------------------
; gfy_PrintStringXY:

;-------------------------------------------------------------------------------
; gfy_PrintString:

;-------------------------------------------------------------------------------
gfy_SetTextScale: ; COPIED_FROM_GRAPHX
; Changes the amount of text scaling (note that height and width are independent)
; Arguments:
;  arg0 : Width scale (1 is default)
;  arg1 : Height scale (1 is default)
; Returns:
;  None
	pop	de
	pop	hl
	pop	bc
	push	bc
	push	hl
	push	de
	ld	a,l
	ld	de,_TextWidthScale
;	ld	hl,_TextScaleJump
	cp	a,c
	jr	z,.match
	jr	.nomatch
.match:
	dec	a
	jr	z,.bothone		; if they are both one; just use normal drawing
	inc	a
.nomatch:
	or	a,a
	ret	z			; null check
	ld	(de),a
	ld	a,c
	or	a,a
	ret	z			; null check
	ld	(_TextHeightScale),a
;	ld	(hl),_PrintLargeFont - _PrintNormalFont
	ret
.bothone:
;	ld	(hl),a			; store a 0, which means no (literal) jump
	inc	a
	ld	(de),a
	ret

;-------------------------------------------------------------------------------
gfy_SetTextConfig:
	pop	de
	ex	(sp),hl			; hl = config
	push	de
	ld	a, l
	ld	(gfy_PrintChar_Clip), a
	ret

gfy_PrintChar_Clip:
	db $00

;-------------------------------------------------------------------------------
; gfy_PrintChar:

; ...

	ld	a,0
_TextFixedWidth = $-1

; ...

	ld	bc,0
_TextXPos := $-3

; ...

	ld	hl,0
_TextYPos := $-3

; ...

	ld	ixl,8
smcByte _TextHeight

; ...

	ld	a,TEXT_BG_COLOR
smcByte _TextBGColor

; ...

	ld	a,TEXT_FG_COLOR
smcByte _TextFGColor

; ...

	cp	a,TEXT_TP_COLOR		; check if transparent
; gfy_PrintChar.transparent_color := $-1
smcByte _TextTPColor

;-------------------------------------------------------------------------------
; _PrintLargeFont:

; ...

	ld	b,1
_TextHeightScale := $-1

; ...

	ld	a,TEXT_BG_COLOR
smcByte _TextBGColor

; ...

	ld	l,1
_TextWidthScale := $-1

; ...

	ld	a,TEXT_FG_COLOR
smcByte _TextFGColor

; ...

	cp	a,TEXT_TP_COLOR		; check if transparent
smcByte _TextTPColor

;-------------------------------------------------------------------------------
; _PrintChar_Clip:

; ...

	ld	iyl,8
smcByte _TextHeight

;-------------------------------------------------------------------------------
; gfy_PrintInt:

;-------------------------------------------------------------------------------
; gfy_PrintUInt:

;-------------------------------------------------------------------------------
gfy_GetStringWidth: ; COPIED_FROM_GRAPHX
; Gets the width of a string
; Arguments:
;  arg0 : Pointer to string
; Returns:
;  Width of string in pixels
	pop	de
	pop	hl
	push	hl			; hl -> string
	push	de
	ld	de,0
.loop:
	ld	a,(hl)
	or	a,a
	jr	z,.done			; loop until null byte
	push	hl
	call	_GetCharWidth
	ex	de,hl
	pop	hl
	inc	hl
	jr	.loop
.done:
	ex	de,hl			; return width of string
	ret

;-------------------------------------------------------------------------------
gfy_GetCharWidth: ; COPIED_FROM_GRAPHX
; Gets the width of a character
; Arguments:
;  arg0 : Character
; Returns:
;  Width of character in pixels
	ld	iy,0
	lea	de,iy
	add	iy,sp
	ld	a,(iy+3)		; a = character
_GetCharWidth:
	sbc	hl,hl
	ld	l,a
	ld	a,(_TextFixedWidth)	; is fixed width
	or	a,a
	jr	nz,.fixed
	ld	bc,(_CharSpacing)	; lookup spacing
	add	hl,bc
	ld	a,(hl)
.fixed:
	ld	l,a
	ld	a,(_TextWidthScale)	; add scaling factor
	ld	h,a
	mlt	hl
	add	hl,de
	ret

;-------------------------------------------------------------------------------
gfy_GetSpriteChar: ; COPIED_FROM_GRAPHX
; Sets the data in char_sprite (must have previously allocated an 8x8 width sprite)
; the pixel map of the character c
; Arguments:
;  arg0 : Pointer to allocated sprite
;  arg1 : Character
; Returns:
;  Pointer to sprite
	pop	hl
	pop	de
	push	de
	push	hl
	ld	a,(_TextFixedWidth)
	or	a,a
	jr	nz,.fixed
	sbc	hl,hl
	ld	l,e			; hl = character
	ld	bc,(_CharSpacing)
	add	hl,bc
	ld	a,(hl)			; a = char width
.fixed:
	or	a,a
	sbc	hl,hl
	ld	l,e			; hl = character
	add	hl,hl
	add	hl,hl
	add	hl,hl
	ld	bc,(_TextData)		; get text data array
	add	hl,bc			; de = draw location
	ld	de,_TmpCharSprite
	ex	de,hl
	push	hl			; save pointer to sprite
	ld	a,8
smcByte _TextHeight
	ld	iyh,a			; ixh = char width
	ld	(hl),a			; store width of character we are drawing
	inc	hl
	ld	iyl,a			; height of char
	inc	hl
	ex	de,hl
	call	_GetChar		; read the character into the array
	pop	hl
	ret

if 0

; gfy_GetSpriteChar:

; ...

smcByte _TextHeight
	ld	iyh,a			; ixh = char width

end if

;-------------------------------------------------------------------------------
_GetChar: ; COPIED_FROM_GRAPHX
; Places a character data into a nice buffer
; Inputs:
;  HL : Points to character pixmap
;  DE : Points to output buffer
; Outputs:
;  Stored pixmap image
;  Uses IY
.loop:
	ld	c,(hl)			; c = 8 pixels (or width based)
	ld	b,iyh
.nextpixel:
	ld	a,TEXT_BG_COLOR
smcByte _TextBGColor
	rlc	c
	jr	nc,.bgcolor
	ld	a,TEXT_FG_COLOR
smcByte _TextFGColor
.bgcolor:
	cp	a,TEXT_TP_COLOR		; check if transparent
smcByte _TextTPColor
	jr	z,.transparent
	ld	(de),a
	inc	de
	djnz	.nextpixel
	inc	hl
	dec	iyl
	jr	nz,.loop
	ret
.transparent:
	ld	a,0
smcByte _TextTPColor
	ld	(de),a
	inc	de			; move to next pixel
	djnz	.nextpixel
	inc	hl
	dec	iyl
	jr	nz,.loop		; okay we stored the character sprite now draw it
	ret

if 0

; _GetChar:

; ...

	ld	a,TEXT_BG_COLOR
smcByte _TextBGColor

; ...

	ld	a,TEXT_FG_COLOR
smcByte _TextFGColor

; ...

	cp	a,TEXT_TP_COLOR		; check if transparent
smcByte _TextTPColor

end if

;-------------------------------------------------------------------------------
gfy_SetFontData: ; COPIED_FROM_GRAPHX
; Sets the font to be custom
; Arguments:
;  arg0 : Pointer to font data
;  Set Pointer to NULL to use default font
; Returns:
;  Pointer to previous font data
	pop	de
	pop	hl
	push	hl			; hl -> custom font data
	push	de
	add	hl,de
	or	a,a
	sbc	hl,de
	ld	de,(_TextData)
	jr	nz,.nonnull		; if null make default font
	ld	hl,_DefaultTextData
.nonnull:
	ld	(_TextData),hl		; save pointer to custom font
	ex	de,hl
	ret

;-------------------------------------------------------------------------------
gfy_SetCharData: ; COPIED_FROM_GRAPHX
; Sets a custom font for a specific character
; Arguments:
;  arg1 : Character index to change (0-127 or 0-255)
;  arg0 : Pointer to character data; if null returns current data
; Returns:
;  Pointer to character data if null, otherwise pointer to next character
	ld	iy,0
	add	iy,sp
	ld	hl,(iy+6)		; de -> custom character data
	add	hl,de
	or	a,a
	sbc	hl,de			; sets z flag if null
	ret	z
	ex	de,hl
	or	a,a
	sbc	hl,hl
	ld	l,(iy+3)		; hl = index
	add	hl,hl
	add	hl,hl
	add	hl,hl
	ld	bc,(_TextData)
	add	hl,bc
	ex	de,hl
	ld	bc,8
	ldir
	ret

;-------------------------------------------------------------------------------
gfy_SetFontSpacing: ; COPIED_FROM_GRAPHX
; Sets the font to be custom spacing
; Arguments:
;  arg0 : Pointer to font spacing
;  Set Pointer to NULL to use default font spacing
; Returns:
;  None
	pop	de
	pop	hl
	push	hl			; hl -> custom font width
	push	de
	add	hl,de
	or	a,a
	sbc	hl,de
	jr	nz,.notnull		; if null make default font width
	ld	hl,_DefaultCharSpacing
.notnull:
	ld	(_CharSpacing),hl	; save pointer to custom widths
	ret

;-------------------------------------------------------------------------------
gfy_SetMonospaceFont: ; COPIED_FROM_GRAPHX
; Sets the font to be monospace
; Arguments:
;  arg0 : Monospace spacing amount
; Returns:
;  None
	pop	hl
	pop	de
	push	de
	push	hl
	ld	a,e			; a = width
	ld	(_TextFixedWidth),a	; store the value of the monospace width
	ret

;-------------------------------------------------------------------------------
gfy_FillTriangle_NoClip: ; COPIED_FROM_GRAPHX
; Draws a filled triangle without clipping
; Arguments:
;  arg0-5 : x0,y0,x1,y1,x2,y2
; Returns:
;  None
	ld	hl,gfy_HorizLine_NoClip
;	jr	_FillTriangle		; emulated by dummifying next instruction:
	db	$FD			; ld hl,* -> ld iy,*
;-------------------------------------------------------------------------------
gfy_FillTriangle: ; COPIED_FROM_GRAPHX
; Draws a filled triangle with clipping
; Arguments:
;  arg0-5 : x0,y0,x1,y1,x2,y2
; Returns:
;  None
	ld	hl,gfy_HorizLine
_FillTriangle:
	ld	(.line0),hl
	ld	(.line1),hl
	ld	(.line2),hl
	push	ix
	ld	ix,0
	add	ix,sp
	lea	hl,ix-39
	ld	sp,hl
	sbc	hl,hl
	ld	(ix-15),hl
	ld	(ix-18),hl		; int sa = 0, sb = 0;
	ld	hl,(ix+9)		; sort coordinates by y order (y2 >= y1 >= y0)
	ld	de,(ix+15)		; if (y0 > y1)
	call	_SignedCompare
	jr	c,.cmp0
	ld	hl,(ix+9)
	ld	(ix+9),de
	ld	(ix+15),hl
	ld	hl,(ix+6)
	ld	de,(ix+12)
	ld	(ix+6),de
	ld	(ix+12),hl
.cmp0:
	ld	hl,(ix+15)
	ld	de,(ix+21)
	call	_SignedCompare
	jr	c,.cmp1
	ld	hl,(ix+15)
	ld	(ix+15),de
	ld	(ix+21),hl
	ld	hl,(ix+12)
	ld	de,(ix+18)
	ld	(ix+12),de
	ld	(ix+18),hl
.cmp1:
	ld	hl,(ix+9)
	ld	de,(ix+15)
	call	_SignedCompare
	jr	c,.cmp2
	ld	hl,(ix+9)
	ld	(ix+9),de
	ld	(ix+15),hl
	ld	hl,(ix+6)
	ld	de,(ix+12)
	ld	(ix+6),de
	ld	(ix+12),hl
.cmp2:
	ld	de,(ix+21)		; if(y0 == y2) - handle awkward all-on-same-line case as its own thing
	ld	hl,(ix+9)
	or	a,a
	sbc	hl,de
	jp	nz,.notflat
	ld	bc,(ix+6)		; x0
	ld	(ix-6),bc		; a = x0
	ld	(ix-3),bc		; b = x0;
	ld	hl,(ix+12)		; if (x1 < a) { a = x1; }
	or	a,a
	sbc	hl,bc
	jp	p,.cmp00
	jp	pe,.cmp01
	jr	.cmp02
.cmp00:
	jp	po,.cmp01
.cmp02:
	ld	bc,(ix+12)
	ld	(ix-3),bc
	jr	.cmp11
.cmp01:
	ld	bc,(ix+12)
	ld	hl,(ix-6)
	or	a,a
	sbc	hl,bc			; else if (x1 > b) { b = x1; }
	jp	p,.cmp10
	jp	pe,.cmp11
	jr	.cmp12
.cmp10:
	jp	po,.cmp11
.cmp12:
	ld	bc,(ix+12)
	ld	(ix-6),bc
.cmp11:
	ld	bc,(ix-3)
	ld	hl,(ix+18)
	or	a,a
	sbc	hl,bc			; if (x2 < a) { a = x2; }
	jp	p,.cmp20
	jp	pe,.cmp21
	jr	.cmp22
.cmp20:
	jp	po,.cmp21
.cmp22:
	ld	bc,(ix+18)
	ld	(ix-3),bc
	jr	.cmp31
.cmp21:
	ld	bc,(ix+18)
	ld	hl,(ix-6)
	or	a,a
	sbc	hl,bc			; else if (x2 > b) { b = x2; }
	jp	p,.cmp30
	jp	pe,.cmp31
	jr	.cmp32
.cmp30:
	jp	po,.cmp31
.cmp32:
	ld	bc,(ix+18)
	ld	(ix-6),bc
.cmp31:
	ld	de,(ix-3)
	ld	hl,(ix-6)
	or	a,a
	sbc	hl,de
	inc	hl
	push	hl
	ld	bc,(ix+9)
	push	bc
	push	de
	call	0			; horizline(a, y0, b-a+1);
.line0 := $-3
	pop	bc
	pop	bc
	pop	bc
	ld	sp,ix
	pop	ix
	ret				; return;
.notflat:
	ld	bc,(ix+6)		; x0
	ld	hl,(ix+12)
	or	a,a
	sbc	hl,bc
	ld	(ix-36),hl		; dx01 = x1 - x0;
	ld	hl,(ix+18)
	or	a,a
	sbc	hl,bc
	ld	(ix-21),hl		; dx02 = x2 - x0;
	ld	bc,(ix+9)		; y0
	ld	hl,(ix+15)
	or	a,a
	sbc	hl,bc
	ld	(ix-33),hl		; dy01 = y1 - y0;
	ld	hl,(ix+21)
	or	a,a
	sbc	hl,bc
	ld	(ix-27),hl		; dy02 = y2 - y0;
	ld	bc,(ix+12)
	ld	hl,(ix+18)
	or	a,a
	sbc	hl,bc
	ld	(ix-30),hl		; dx12 = x2 - x1;
	ld	bc,(ix+15)
	ld	hl,(ix+21)
	or	a,a
	sbc	hl,bc
	ld	(ix-39),hl		; dy12 = y2 - y1;
	jr	nz,.elselast		; if (y1 == y2) { last = y1; }
	ld	(ix-24),bc
	jr	.sublast
.elselast:
	ld	bc,(ix+15)		; else { last = y1-1; }
	dec	bc
	ld	(ix-24),bc
.sublast:
	ld	bc,(ix+9)
	ld	(ix-12),bc		; for (y = y0; y <= last; y++)
	jp	.firstloopstart
.firstloop:
	ld	hl,(ix-15)
	ld	bc,(ix-33)
	call	_DivideHLBC
	ld	bc,(ix+6)
	add	hl,bc
	ld	(ix-3),hl		; a = x0 + sa / dy01;
	ld	hl,(ix-18)
	ld	bc,(ix-27)
	call	_DivideHLBC
	ld	bc,(ix+6)
	add	hl,bc
	ld	(ix-6),hl		; b = x0 + sb / dy02;
	ld	bc,(ix-36)
	ld	hl,(ix-15)
	add	hl,bc
	ld	(ix-15),hl		; sa += dx01;
	ld	bc,(ix-21)
	ld	hl,(ix-18)
	add	hl,bc
	ld	(ix-18),hl		; sb += dx02;
	ld	de,(ix-3)
	ld	hl,(ix-6)
	or	a,a
	sbc	hl,de			; if (b < a) { swap(a,b); }
	jp	p,.cmp40
	jp	pe,.cmp41
	jr	.cmp42
.cmp40:
	jp	po,.cmp41
.cmp42:
	ld	hl,(ix-3)
	ld	de,(ix-6)
	ld	(ix-3),de
	ld	(ix-6),hl
.cmp41:
	ld	hl,(ix-6)
	or	a,a
	sbc	hl,de
	inc	hl
	push	hl
	ld	bc,(ix-12)
	push	bc
	push	de
	call	0			; horizline(a, y, b-a+1);
.line1 := $-3
	pop	bc
	pop	bc
	pop	bc
	ld	bc,(ix-12)
	inc	bc
	ld	(ix-12),bc
.firstloopstart:
	ld	hl,(ix-24)
	or	a,a
	sbc	hl,bc
	jp	p,.cmp50
	jp	pe,.firstloop
	jr	.cmp52
.cmp50:
	jp	po,.firstloop
.cmp52:
	ld	bc,(ix+15)
	ld	hl,(ix-12)
	or	a,a
	sbc	hl,bc
	ld	bc,(ix-30)
	call	_MultiplyHLBC		; sa = dx12 * (y - y1);
	ld	(ix-15),hl
	ld	bc,(ix+9)
	ld	hl,(ix-12)
	or	a,a
	sbc	hl,bc
	ld	bc,(ix-21)
	call	_MultiplyHLBC		; sb = dx02 * (y - y0);
	ld	(ix-18),hl
	jp	.secondloopstart	; for(; y <= y2; y++)
.secondloop:
	ld	hl,(ix-15)
	ld	bc,(ix-39)
	call	_DivideHLBC
	ld	bc,(ix+12)
	add	hl,bc
	ld	(ix-3),hl		; a = x1 + sa / dy12;
	ld	hl,(ix-18)
	ld	bc,(ix-27)
	call	_DivideHLBC
	ld	bc,(ix+6)
	add	hl,bc
	ld	(ix-6),hl		; b = x0 + sb / dy02;
	ld	bc,(ix-30)
	ld	hl,(ix-15)
	add	hl,bc
	ld	(ix-15),hl		; sa += dx12;
	ld	bc,(ix-21)
	ld	hl,(ix-18)
	add	hl,bc
	ld	(ix-18),hl		; sb += dx02;
	ld	de,(ix-3)
	ld	hl,(ix-6)
	or	a,a
	sbc	hl,de			; if (b < a) { swap(a,b); }
	jp	p,.cmp60
	jp	pe,.cmp61
	jr	.cmp62
.cmp60:
	jp	po,.cmp61
.cmp62:
	ld	hl,(ix-3)
	ld	de,(ix-6)
	ld	(ix-3),de
	ld	(ix-6),hl
.cmp61:
	ld	hl,(ix-6)
	or	a,a
	sbc	hl,de
	inc	hl
	push	hl
	ld	bc,(ix-12)
	push	bc
	push	de
	call	0			; horizline(a, y, b-a+1);
.line2 := $-3
	pop	bc
	pop	bc
	pop	bc
	ld	bc,(ix-12)
	inc	bc
	ld	(ix-12),bc
.secondloopstart:
	ld	bc,(ix-12)
	ld	hl,(ix+21)
	or	a,a
	sbc	hl,bc
	jp	p,.cmp70
	jp	pe,.secondloop
	ld	sp,ix
	pop	ix
	ret
.cmp70:
	jp	po,.secondloop
	ld	sp,ix
	pop	ix
	ret

;-------------------------------------------------------------------------------
; gfy_Polygon_NoClip:

;-------------------------------------------------------------------------------
; gfy_Polygon:

;-------------------------------------------------------------------------------
gfy_Reserved: ; COPIED_FROM_GRAPHX
; Deprecated unused function (available for use)
	ret

;-------------------------------------------------------------------------------
gfy_Deprecated: ; COPIED_FROM_GRAPHX
; Decompresses a sprite that is LZ77 compressed from ConvPNG (Deprecated)
	ld	hl,-23
	call	ti._frameset
	ld	hl,(ix+6)
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex.s	de,hl
	inc	hl
	inc	hl
	ld	(ix-17),hl
	ld	bc,3
	ld	(ix-3),bc
	ld	iy,(ix+6)
	ld	a,(iy+2)
	ld	(ix-8),a
	or	a,a
	sbc	hl,hl
	ld	(ix-6),hl
d_17:	ld	bc,(ix-3)
	ld	hl,(ix+6)
	add	hl,bc
	inc	bc
	ld	(ix-3),bc
	ld	a,(hl)
	ld	(ix-7),a
	cp	a,(ix-8)
	jp	nz,d_16
	ld	bc,(ix-3)
	ld	hl,(ix+6)
	add	hl,bc
	ld	(ix-14),hl
	ld	a,(hl)
	or	a,a
	jr	nz,d_13
	ld	bc,(ix-6)
	ld	hl,(ix+9)
	add	hl,bc
	inc	bc
	ld	(ix-6),bc
	ld	a,(ix-8)
	ld	(hl),a
	ld	bc,(ix-3)
	inc	bc
	ld	(ix-3),bc
	jr	d_18
d_13:	ld	bc,(ix-14)
	push	bc
	pea	ix-20
	call	_LZ_ReadVarSize
	pop	bc
	pop	bc
	ld	bc,(ix-3)
	add	hl,bc
	ld	(ix-3),hl
	ld	bc,(ix+6)
	add	hl,bc
	push	hl
	pea	ix-23
	call	_LZ_ReadVarSize
	pop	bc
	pop	bc
	ld	bc,(ix-3)
	add	hl,bc
	ld	(ix-3),hl
	or	a,a
	sbc	hl,hl
	ld	(ix-11),hl
	jr	d_11
d_9:	ld	bc,(ix-23)
	ld	hl,(ix-6)
	or	a,a
	sbc	hl,bc
	ld	bc,(ix+9)
	add	hl,bc
	push	hl
	pop	iy
	ld	bc,(ix-6)
	ld	hl,(ix+9)
	add	hl,bc
	inc	bc
	ld	(ix-6),bc
	ld	a,(iy)
	ld	(hl),a
	ld	bc,(ix-11)
	inc	bc
	ld	(ix-11),bc
d_11:	ld	bc,(ix-20)
	ld	hl,(ix-11)
	or	a,a
	sbc	hl,bc
	jr	c,d_9
	jr	d_18
d_16:	ld	bc,(ix-6)
	ld	hl,(ix+9)
	add	hl,bc
	inc	bc
	ld	(ix-6),bc
	ld	a,(ix-7)
	ld	(hl),a
d_18:	ld	bc,(ix-17)
	ld	hl,(ix-3)
	or	a,a
	sbc	hl,bc
	jp	c,d_17
	ld	sp,ix
	pop	ix
	ret

;-------------------------------------------------------------------------------
gfy_FlipSpriteX: ; COPIED_FROM_GRAPHX gfx_FlipSpriteY
; Flips an array horizontally about the center vertical axis
; Arguments:
;  arg0 : Pointer to sprite struct input
;  arg1 : Pointer to sprite struct output
; Returns:
;  arg1 : Pointer to sprite struct output
	ld	iy,0
	add	iy,sp
	push	ix
	ld	ix,(iy+3)
	ld	a,(ix+0)		; a = width of sprite
	sbc	hl,hl
	ld	l,a
	ld	c,a
	push	hl
	ld	(.width),a
	add	hl,hl
	ld	(.delta),hl		; width*2
	ld	a,(ix+1)		; a = height of sprite
	pop	hl
	lea	de,ix+2
	add	hl,de
	ld	ix,(iy+6)
	ld	(ix+1),a		; store height to width
	ld	(ix+0),c		; store width to height
	lea	de,ix+2			; de -> sprite data
	ex	(sp),ix			; restore stack frame
.loop:
	ld	b,0
.width := $-1
	ld	c,a
.pixelloop:
	dec	hl
	ld	a,(hl)
	ld	(de),a			; store the new pixel data
	inc	de
	djnz	.pixelloop
	ld	a,c
	ld	bc,0
.delta := $-3
	add	hl,bc
	dec	a
	jr	nz,.loop
	pop	hl
	ret

;-------------------------------------------------------------------------------
gfy_FlipSpriteY: ; COPIED_FROM_GRAPHX gfx_FlipSpriteX
; Flip a sprite vertically about the center horizontal axis
; Arguments:
;  arg0 : Pointer to sprite struct input
;  arg1 : Pointer to sprite struct output
; Returns:
;  arg1 : Pointer to sprite struct output
	ld	iy,0
	add	iy,sp
	push	ix
	ld	ix,(iy+3)
	xor	a,a
	sub	a,(ix+0)
	ld	(.delta),a
	neg
	ld	(.width),a
	ld	l,(ix+1)
	ld	c,l
	dec	l
	ld	h,a
	mlt	hl
	lea	de,ix+2
	add	hl,de
	ld	ix,(iy+6)
	ld	(ix+0),a
	ld	(ix+1),c
	lea	de,ix+2
	push	ix
.loop:
	ld	bc,0
.width := $-3
	ldir
	ld	bc,-1
.delta := $-3
	add	hl,bc
	add	hl,bc
	dec	a
	jr	nz,.loop
	pop	hl
	pop	ix
	ret

;-------------------------------------------------------------------------------
gfy_RotateSpriteCC: ; COPIED_FROM_GRAPHX gfy_RotateSpriteC
; Rotates an array 90 degress clockwise
; Arguments:
;  arg0 : Pointer to sprite struct input
;  arg1 : Pointer to sprite struct output
; Returns:
;  arg1 : Pointer to sprite struct output
	ld	iy,0
	add	iy,sp
	push	ix
	ld	hl,(iy+6)
	ld	iy,(iy+3)
	ld	ix,(iy+0)		; ixl = width  ,  ixh = height
	lea	bc,ix
	ld	(hl),b
	inc	hl
	ld	(hl),c
	mlt	bc
	add	hl,bc
	ex	de,hl
	ld	c,ixl
	ld	b,0
	inc	bc
.outer:
	lea	hl,iy
	dec	iy
	ld	a,ixh
.inner:
	add	hl,bc
	inc	bc
	ldd
	dec	a
	jr	nz,.inner
	dec	ixl
	jr	nz,.outer
	dec	de
	ex	de,hl
	pop	ix
	ret

;-------------------------------------------------------------------------------
gfy_RotateSpriteC: ; COPIED_FROM_GRAPHX gfy_RotateSpriteCC
; Rotates a sprite 90 degrees counter clockwise
; Arguments:
;  arg0 : Pointer to sprite struct input
;  arg1 : Pointer to sprite struct output
; Returns:
;  arg1 : Pointer to sprite struct output
	ld	iy,0
	lea	bc,iy
	add	iy,sp
	push	ix
	ld	hl,(iy+6)
	push	hl
	ld	iy,(iy+3)
	ld	ix,(iy+0)		; ixl = width  ,  ixh = height
	lea	de,ix
	ld	(hl),d
	inc	hl
	ld	(hl),e
	inc	hl
	dec	e
	ld	c,e
	ex	de,hl
.outer:
	lea	hl,iy+2
	dec	iy
	ld	a,ixh
.inner:
	add	hl,bc
	inc	c
	ldi
	dec	a
	jr	nz,.inner
	dec	ixl
	jr	nz,.outer
	pop	hl
	pop	ix
	ret

;-------------------------------------------------------------------------------
gfy_RotateSpriteHalf: ; COPIED_FROM_GRAPHX
; Rotates an array 180 degrees
; Arguments:
;  arg0 : Pointer to sprite struct input
;  arg1 : Pointer to sprite struct output
; Returns:
;  arg1 : Pointer to sprite struct output
	ld	iy,0
	add	iy,sp
	ld	hl,(iy+3)
	ld	c,(hl)			; c = width
	inc	hl
	ld	b,(hl)			; b = height
	ld	iy,(iy+6)
	ld	(iy+0),bc
	mlt	bc
	add	hl,bc
	lea	de,iy
	push	de
.loop:
	inc	de
	inc	de
	ldd
	jp	pe,.loop
	pop	hl
	ret

;-------------------------------------------------------------------------------
gfy_ScaleSprite: ; COPIED_FROM_GRAPHX
; Scale an image using an output buffer
; Arguments:
;  arg0 : Pointer to sprite struct input
;  arg1 : Pointer to sprite struct output
; Returns:
;  arg1 : Pointer to sprite struct output
	ld	iy,0
	lea	bc,iy
	add	iy,sp
	push	ix
	ld	hl,(iy+6)
	push	hl
	ld	a,(hl)
	ld	ixh,a			; target_width
	ld	(ScaleWidth),a
	inc	hl
	xor	a,a
	sub	a,(hl)
	ld	ixl,a			; -target_height
	inc	hl
	push	hl			; hl->tgt_data
	ld	hl,(iy+3)
	ld	e,(hl)			; src_width
	inc	hl
	ld	c,(hl)			; src_height
	inc	hl
	push	hl			; hl->src_data
	push	de			; e = src_width
	call	_UCDivA			; ca = dv = (source_height*256)/target_height
	pop	hl			; l = src_width
	ld	(dv_shl_16),a
	ld	h,c
	ld	c,l
	mlt	hl
	ld	(dv_shr_8_times_width),hl
	add	hl,bc
	ld	(dv_shr_8_times_width_plus_width),hl
	xor	a,a
	sub	a,ixh			; -target_width
	call	_UCDivA			; ca = du = (source_width*256)/target_width
	pop	hl			; hl->src_data
	pop	de			; de->tgt_data
	ld	iy,0
	ld	iyl,a
	ld	a,c			; du = bc:iyl
	ld	(du),a			; ixl = target_height

; b = out_loop_times
; de = target buffer adress
.outer:
	push	hl
ScaleWidth := $+2
	ld	iyh, 0
	xor	a,a
	ld	b,a
	ld	c,0
du := $-1
.loop:	ldi
	add	a,iyl
	adc	hl,bc			; xu += du
	inc	bc			; bc:iyl is du
	dec	iyh
	jr	nz,.loop
	pop	hl			; add up to hla
	ld	bc,0			; dv<<16
dv_shl_16 := $-1
	add	iy,bc
	ld	bc,0			; dv>>8*src_width
dv_shr_8_times_width := $-3
	jr	nc,.skip
	ld	bc,0			; dv>>8*src_width+src_width
dv_shr_8_times_width_plus_width := $-3
.skip:
	add	hl,bc
	inc	ixl
	jr	nz,.outer
	pop	hl
	pop	ix
	ret

;-------------------------------------------------------------------------------
gfy_RotatedScaledSprite_NoClip: ; UNIMPLEMENTED
	ret

;-------------------------------------------------------------------------------
gfy_RotatedScaledTransparentSprite_NoClip: ; UNIMPLEMENTED
	ret

; ...

	cp	a,TRASPARENT_COLOR
smcByte _TransparentColor

;-------------------------------------------------------------------------------
gfy_RotateScaleSprite: ; COPIED_FROM_GRAPHX
; Rotate and scale an image using an output buffer
; Arguments:
;  arg0 : Pointer to sprite struct input
;  arg1 : Pointer to sprite struct output
;  arg2 : Rotation angle as an integer
;  arg3 : Scale factor (64 = 100%)
; Returns:
;  arg1 : Pointer to sprite struct output
	push	ix
	ld	ix,0
	add	ix,sp
	push	hl
	ld	iy,(ix+6)		; sprite pointer
	lea	hl,iy+2
	ld	(_smc_dsrs_sprptr_0 + 1),hl ; write smc

	; sinf = _SineTable[angle] * 128 / scale;
	ld	a,(ix+12)		; angle
	call	getSinCos
	ld	l,0
	ld	h,a
	rlca
	rr	h			; hl = _SineTable[angle] * 128
	ld	c,(ix+15)
	ld	b,0
	call	_16Div8Signed		; hl = _SineTable[angle] * 128 / scale (sin)
	ex	de,hl
	ld	a,d
	rlca
	sbc	hl,hl
	ld	l,e
	ld	h,d
	ld	(_smc_dsrs_sinf_1 + 1),hl ; write smc
	push	hl
	ex	de,hl
	sbc	hl,hl
	ccf
	sbc	hl,de
	ld	(_smc_dsrs_sinf_0 + 1),hl ; write smc
	pop	hl

	; dxs = sinf * -(size * scale / 128);
	ld	c,(ix+15)
	ld	b,(iy)
	mlt	bc			; size * scale
	rl	c
	rl	b
	ld	c,b			; (size * scale / 128)
	xor	a,a
	sub	a,c
	ld	c,a
	sbc	a,a
	ld	b,a			; -(size * scale / 128)
	ld	(ix-3),bc
	call	_16Mul16SignedNeg	; sinf * -(size * scale / 128)
	ld	(_smc_dsrs_dys_0 + 1),hl ; write smc
	push	hl

	; cosf = _SineTable[angle + 64] * 128 / scale
	ld	a,64
	add	a,(ix+12)		; angle + 64
	call	getSinCos
	ld	l,0
	ld	h,a
	rlca
	rr	h			; hl = _SineTable[angle + 64] * 128
	ld	c,(ix+15)
	ld	b,0
	call	_16Div8Signed		; hl = _SineTable[angle + 64] * 128 / scale (cos)
	ex	de,hl
	ld	a,d
	rlca
	sbc	hl,hl
	ld	l,e
	ld	h,d
	ld	(_smcdsrs_cosf_0 + 1),hl ; write smc
	ld	(_smc_dsrs_cosf_1 + 1),hl ; write smc

	; dxc = cosf * -(size * scale / 128);
	ld	bc,(ix-3)		; -(size * scale / 128)
	call	_16Mul16SignedNeg	; cosf * -(size * scale / 128)
	ld	(_smcdsrs_dyc_0 + 1),hl ; write smc
	push	hl

	ld	a,(iy)			; size
	ld	(_smcdsrs_ssize_1 + 1),a ; write smc
	dec	a
	ld	(_smcdsrs_ssize_0 + 1),a ; write smc
	inc	a
	ld	b,a
	ld	c,(ix+15)
	mlt	bc			; size * scale
	srl	a			; size / 2
	or	a,a
	sbc	hl,hl
	ld	h,a
	ld	(_smc_dsrs_size128_0 + 1),hl ; write smc
	ld	(_smc_dsrs_size128_1 + 1),hl ; write smc
	ld	a,b
	rl	c
	adc	a,a
	rl	c
	adc	a,a			; size * scale / 64
	jr	nz,.skip
	inc	a			; hax for scale = 1?
.skip:
	ld	(_smc_dsrs_size_1 + 2),a ; write smc

	pop	de			; smc = dxc start
	pop	hl			; smc = dxs start

	ld	iy,(ix+9)		; sprite storing to
	push	iy
	ld	(iy+0),a
	ld	(iy+1),a
	lea	ix,iy+2

	ld	iyh,a			; size * scale / 64
_yloop:
	push	hl			; dxs
	push	de			; dxc

	; xs = (dxs + dyc) + (size * 128);
_smcdsrs_dyc_0:
	ld	bc,$000000
	add	hl,bc
_smc_dsrs_size128_0:
	ld	bc,$000000
	add	hl,bc
	ex	de,hl			; de = (dxs + dyc) + (size * 128)
	; ys = (dxc - dys) + (size * 128);
_smc_dsrs_dys_0:
	ld	bc,$000000
	or	a,a
	sbc	hl,bc
_smc_dsrs_size128_1:
	ld	bc,$000000
	add	hl,bc			; hl = (dxc - dys) + (size * 128)

_smc_dsrs_size_1:			; smc = size * scale / 64
	ld	iyl,$00
_xloop:
	push	hl			; xs

	ld	a,d
	or	a,h
	rlca
	ld	c,TRASPARENT_COLOR
smcByte _TransparentColor
	jr	c,drawSpriteRotateScale_SkipPixel
_smcdsrs_ssize_0:
	ld	a,0
	cp	a,d
	jr	c,drawSpriteRotateScale_SkipPixel
	cp	a,h
	jr	c,drawSpriteRotateScale_SkipPixel

	; get pixel and draw to buffer
_smcdsrs_ssize_1:
	ld	c,0
	ld	b,h
	mlt	bc
	sbc	hl,hl
	ld	l,d
	add	hl,bc			; y * size + x
_smc_dsrs_sprptr_0:
	ld	bc,0
	add	hl,bc
	ld	c,(hl)
drawSpriteRotateScale_SkipPixel:
	ld	(ix),c			; write pixel
	inc	ix			; x++s
_smcdsrs_cosf_0:			; smc = cosf
	ld	hl,0
	add	hl,de			; xs += cosf
	ex	de,hl
	pop	hl			; ys
_smc_dsrs_sinf_0:			; smc = -sinf
	ld	bc,0
	add	hl,bc			; ys += -sinf
	dec	iyl
	jr	nz,_xloop		; x loop

	pop	hl			; dxc
_smc_dsrs_cosf_1:			; smc = cosf
	ld	bc,0
	add	hl,bc			; dxc += cosf
	ex	de,hl
	pop	hl			; dxs
_smc_dsrs_sinf_1:			; smc = sinf
	ld	bc,0
	add	hl,bc			; dxs += sinf

	dec	iyh
	jp	nz,_yloop		; y loop
	pop	hl			; sprite out ptr
	pop	de
	pop	ix
	ret

getSinCos:
	; returns a = sin/cos(a) * 128
	ld	c,a
	bit	7,a
	jr	z,$+4
	sub	a,128
	bit	6,a
	jr	z,$+6
	ld	e,a
	ld	a,128
	sub	a,e
	ld	de,0
	ld	e,a
	ld	hl,_SineTable
	add	hl,de
	ld	a,(hl)
	bit	7,c
	ret	z
	neg
	ret

_SineTable:
	; sin(x) * 128
	db 0,3,6,9,13,16,19,22,25,28,31,34,37,40,43,46
	db 49,52,55,58,60,63,66,68,71,74,76,79,81,84,86,88
	db 91,93,95,97,99,101,103,105,106,108,110,111,113,114,116,117
	db 118,119,121,122,122,123,124,125,126,126,127,127,127,127,127,127,127

_16Div8Signed:
	ld	a,h
	xor	a,c
	push	af
	bit	7,h
	jr	z,.next0
	ex	de,hl
	sbc	hl,hl
	sbc	hl,de
.next0:
	xor	a,a
	sub	a,c
	jp	m,.next1
	ld	c,a
.next1:
	push	hl
	dec	sp
	pop	hl
	inc	sp
	ld	b,16
	xor	a,a
.div:	add	hl,hl
	rla
	cp	a,c
	jr	c,.check
	sub	a,c
	inc	l
.check:
	djnz	.div
	pop	af
	ret	p
	ex	de,hl
	sbc	hl,hl
	sbc	hl,de
	ret

_16Mul16SignedNeg:
	ld	d,b
	ld	e,l
	ld	b,h
	ld	h,c
	mlt	bc
	mlt	de
	mlt	hl
	ld	a,h
	add	a,c
	add	a,e
	ld	h,a
	ex	de,hl
	rlca
	sbc	hl,hl
	ld	l,e
	ld	h,d
	ret

;-------------------------------------------------------------------------------
gfy_FloodFill: ; UNIMPLEMENTED
	ret

;-------------------------------------------------------------------------------
; gfy_RLETSprite:

;-------------------------------------------------------------------------------
; gfy_RLETSprite_NoClip:

;-------------------------------------------------------------------------------
gfy_ConvertFromRLETSprite: ; COPIED_FROM_GRAPHX
; Converts a sprite with RLE transpareny to a sprite with normal transparency.
; Arguments:
;  arg0 : pointer to gfy_rletsprite_t input
;  arg1 : pointer to gfy_sprite_t output
; Returns:
;  arg1 : pointer to gfy_sprite_t output
	pop	bc
	pop	de			; de = gfy_rletsprite_t *input
	ex	(sp),hl			; hl = gfy_sprite_t *output
	push	de
	push	bc
	ex	de,hl			; de = output, hl = input
; Save output to return.
	push	de
; Read and copy the sprite width and height.
	ld	iy,(hl)			; iyh = height, iyl = width
	ldi				; output height = height
	ldi				; output width = width, hl = input data
; Initialize values for looping.
	inc.s	bc			; bcu = 0
; Row loop {
_ConvertFromRLETSprite_Row:
	ld	a,iyl			; a = width
;; Data loop {
_ConvertFromRLETSprite_Trans:
;;; Read the length of a transparent run.
	ld	b,(hl)			; b = trans run length
	inc	hl
	inc	b
	dec	b
;;; Skip the transparent run if the length is zero.
	jr	z,_ConvertFromRLETSprite_Opaque ; z ==> trans run length == 0
;;; Write <transparent run length> zeros to the output.
	sub	a,b			; a = width remaining after trans run
	ld	c,0			; c = trans color
smcByte _TransparentColor
	ex	de,hl			; de = input data, hl = output data
_ConvertFromRLETSprite_TransLoop:
	ld	(hl),c			; write trans color to output
	inc	hl
	djnz	_ConvertFromRLETSprite_TransLoop ; decrement trans run length remaining,
						 ; nz ==> trans run length remaining != 0
	ex	de,hl			; de = output data, hl = input data
;;; Break out of data loop if width remaining == 0.
	jr	z,_ConvertFromRLETSprite_RowEnd ; z ==> width remaining == 0
_ConvertFromRLETSprite_Opaque:
;;; Read the length of an opaque run and copy it to the output.
	ld	c,(hl)			; bc = opaque run length
	inc	hl
	sub	a,c			; a = width remaining after opqaue run
	ldir				; copy opaque run
;;; Continue data loop while width remaining != 0.
	jr	nz,_ConvertFromRLETSprite_Trans ; nz ==> width remaining != 0
;; }
_ConvertFromRLETSprite_RowEnd:
;; Decrement height remaining. Continue row loop while not zero.
	dec	iyh			; decrement height remaining
	jr	nz,_ConvertFromRLETSprite_Row ; nz ==> height remaining != 0
; }
; Return output.
	pop	hl			; hl = output
	ret

;-------------------------------------------------------------------------------
gfy_ConvertToNewRLETSprite: ; COPIED_FROM_GRAPHX
; Converts a sprite with normal transpareny to a sprite with RLE transparency,
; allocating the exact amount of necessary space for the converted sprite.
; Arguments:
;  arg0 : pointer to gfy_sprite_t input
;  arg1 : pointer to malloc
; Returns:
;  arg1 : pointer to gfy_rletsprite_t output
	pop	bc
	pop	de			; de = gfy_sprite_t *input
	ex	(sp),hl			; hl = malloc
	push	de
	push	bc
	ld	(_ConvertToNewRLETSprite_Malloc_SMC),hl
	ex	de,hl			; hl = input
; Save input to copy after allocating output.
	push	hl
; Read the sprite width and height.
	ld	iy,(hl)			; iyh = height, iyl = width
	inc	hl			; hl = <input data>-1
; Initialize values for looping.
	ld	de,2			; de = 2 = output size
	ld	a,0			; a = trans color
smcByte _TransparentColor
; Row loop {
_ConvertToNewRLETSprite_Row:
	ld	b,iyl			; b = width
	inc	de			; increment output size for first trans run
;; Transparent loop {
_ConvertToNewRLETSprite_TransLoop:
	inc	hl
	cp	a,(hl)			; compare an input pixel to trans color
	inc	de			; increment output size for potential opaque run
	jr	nz,_ConvertToNewRLETSprite_OpaquePixel ; nz ==> not transparent
	dec	de			; revert output size, not opaque run
_ConvertToNewRLETSprite_TransPixel:
;;; Continue while width remaining != 0.
	djnz	_ConvertToNewRLETSprite_TransLoop ; decrement width remaining,
						  ; nz ==> width remaining != 0
;; }
;; Finish row.
	jr	_ConvertToNewRLETSprite_RowEnd
;; Opaque loop {
_ConvertToNewRLETSprite_OpaqueLoop:
	inc	hl
	cp	a,(hl)			; compare an input pixel to trans color
_ConvertToNewRLETSprite_OpaquePixel:
	inc	de			; increment output length
	jr	z,_ConvertToNewRLETSprite_TransPixel ; z ==> transparent
;;; Continue while width remaining != 0.
	djnz	_ConvertToNewRLETSprite_OpaqueLoop ; decrement width remaining,
						   ; nz ==> width remaining != 0
;; }
_ConvertToNewRLETSprite_RowEnd:
;; Decrement height remaining. Continue row loop while not zero.
	dec	iyh			; decrement height remaining
	jr	nz,_ConvertToNewRLETSprite_Row ; nz ==> height remaining != 0
; }
; Allocate output.
	push	de
	call	0			; malloc(output size), hl = output
_ConvertToNewRLETSprite_Malloc_SMC := $-3
	pop	de
; Convert sprite.
	pop	de			; de = input
	jr	_ConvertToRLETSprite_ASM

;-------------------------------------------------------------------------------
gfy_ConvertToRLETSprite: ; COPIED_FROM_GRAPHX
; Converts a sprite with normal transpareny to a sprite with RLE transparency.
; Arguments:
;  arg0 : pointer to gfy_sprite_t input
;  arg1 : pointer to gfy_rletsprite_t output
; Returns:
;  arg1 : pointer to gfy_rletsprite_t output
	pop	bc
	pop	de			; de = gfy_sprite_t *input
	ex	(sp),hl			; hl = gfy_rletsprite_t *output
	push	de
	push	bc
_ConvertToRLETSprite_ASM:
	ex	de,hl			; de = output, hl = input
; Save output to return.
	push	de
; Read and copy the sprite width and height.
	ld	iy,(hl)			; iyh = height, iyl = width
	ldi				; output height = height
	ldi				; output width = width, hl = input data
; Initialize values for looping.
	inc.s	bc			; bcu = 0
	ld	a,0			; a = trans color
smcByte _TransparentColor
; Row loop {
_ConvertToRLETSprite_Row:
	ld	b,iyl			; b = width
;; Data loop {
_ConvertToRLETSprite_Trans:
;;; Calculate the length of a transparent run.
	ld	c,0			; c = 0 = trans run length
;;; Transparent loop {
_ConvertToRLETSprite_TransLoop:
	cp	a,(hl)			; compare an input pixel to trans color
	jr	nz,_ConvertToRLETSprite_TransEnd ; nz ==> not transparent
	inc	hl
	inc	bc			; increment trans run length
;;;; Continue transparent loop while width remaining != 0.
	djnz	_ConvertToRLETSprite_TransLoop ; decrement width remaining,
					       ; nz ==> width remaining != 0
;;; }
;;; Write the length of the transparent run to the output.
_ConvertToRLETSprite_TransEnd:
	ex	de,hl			; de = input data, hl = output data
	ld	(hl),c			; write trans run length
	inc	hl
	ex	de,hl			; de = output data, hl = input data
;;; Break out of data loop if width remaining == 0.
	jr	z,_ConvertToRLETSprite_RowEnd ; z ==> last pixel was transparent
					      ;   ==> width remaining == 0
;;; Copy an opaque run to the output.
_ConvertToRLETSprite_Opaque:
	ld	c,0			; c = 0 = opaque run length
	push	de			; (sp) = location to write opaque run length
	inc	de
;;; Opaque loop {
_ConvertToRLETSprite_OpaqueLoop:
	cp	a,(hl)			; compare an input pixel to trans color
	jr	z,_ConvertToRLETSprite_OpaqueEnd ; z ==> transparent
	inc	bc			; cancel dec bc from upcoming ldi
	ldi				; copy opaque pixel
	inc	bc			; increment opaque run length
;;;; Continue opaque/data loop while width remaining != 0.
	djnz	_ConvertToRLETSprite_OpaqueLoop ; decrement width remaining,
						; nz ==> width remaining != 0
_ConvertToRLETSprite_OpaqueEnd:
	ex	(sp),hl			; (sp) = input data, hl = location to write opaque run length
	ld	(hl),c			; write opaque run length
	pop	hl			; hl = input data
;;; Continue data loop if width remaining != 0.
	jr	z,_ConvertToRLETSprite_Trans ; z ==> last pixel was transparent
					     ;   ==> width remaining != 0
;;; }
;; }
_ConvertToRLETSprite_RowEnd:
;; Decrement height remaining. Continue row loop while not zero.
	dec	iyh			; decrement height remaining
	jr	nz,_ConvertToRLETSprite_Row ; nz ==> height remaining != 0
; }
; Return output.
	pop	hl			; hl = output
	ret

;-------------------------------------------------------------------------------
; Inner library routines
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
_LZ_ReadVarSize: ; COPIED_FROM_GRAPHX
; LZ Decompression Subroutine (DEPRECATED)
	push	ix
	ld	ix,0
	lea	de,ix
	add	ix,sp
	lea	hl,ix-12
	ld	sp,hl
	ld	(ix-3),de
	ld	(ix-6),de
.loop:
	or	a,a
	sbc	hl,hl
	ex	de,hl
	ld	hl,(ix+9)
	ld	a,(hl)
	or	a,a
	sbc	hl,hl
	ld	l,a
	ld	(ix-9),hl
	ld	bc,(ix+9)
	inc	bc
	ld	(ix+9),bc
	ld	a,(ix-9)
	and	a,127
	sbc	hl,hl
	ld	l,a
	ld	(ix-12),hl
	ld	hl,(ix-3)
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	push	hl
	pop	bc
	ld	hl,(ix-12)
	call	ti._ior
	ld	(ix-3),hl
	ld	bc,(ix-6)
	inc	bc
	ld	(ix-6),bc
	ld	a,(ix-9)
	and	a,128
	sbc	hl,hl
	ld	l,a
	sbc	hl,de
	jr	nz,.loop
	ld	hl,(ix+6)
	ld	bc,(ix-3)
	ld	(hl),bc
	ld	hl,(ix-6)
	ld	sp,ix
	pop	ix
	ret

;-------------------------------------------------------------------------------
_Maximum: ; COPIED_FROM_GRAPHX
; Calculate the resut of a signed comparison
; Inputs:
;  DE,HL=numbers
; Oututs:
;  HL=max number
	or	a,a
	sbc	hl,de
	add	hl,de
	jp	p,.skip
	ret	pe
	ex	de,hl
.skip:
	ret	po
	ex	de,hl
	ret

;-------------------------------------------------------------------------------
_Minimum: ; COPIED_FROM_GRAPHX
; Calculate the resut of a signed comparison
; Inputs:
;  DE,HL=numbers
; Oututs:
;  HL=min number
	or	a,a
	sbc	hl,de
	ex	de,hl
	jp	p,.skip
	ret	pe
	add	hl,de
.skip:
	ret	po
	add	hl,de
	ret

;-------------------------------------------------------------------------------
_ClipRegion:
; Calculates the new coordinates given the clip  and inputs
; Inputs:
;  None
; Outputs:
;  Modifies data registers
;  Sets C flag if offscreen
	ld	hl,0
smcWord _XMin
.XMin := $-3
	ld	de,(iy+3)
	call	_Maximum
	ld	(iy+3),hl
	ld	hl,ti.lcdWidth
smcWord _XMax
.XMax := $-3
	ld	de,(iy+9)
	call	_Minimum
	ld	(iy+9),hl
	ld	de,(iy+3)
	call	.compare
	ret	c
	ld	hl,0
smcWord _YMin
.YMin := $-3
	ld	de,(iy+6)
	call	_Maximum
	ld	(iy+6),hl
	ld	hl,ti.lcdHeight
smcWord _YMax
.YMax := $-3
	ld	de,(iy+12)
	call	_Minimum
	ld	(iy+12),hl
	ld	de,(iy+6)
.compare:
	dec	hl
_SignedCompare:
	or	a,a
	sbc	hl,de
	add	hl,hl
	ret	po
	ccf
	ret

;-------------------------------------------------------------------------------
_UCDivA: ; COPIED_FROM_GRAPHX
	sbc	hl,hl
	ld	h,a
	xor	a,a
	ld	l,a
	ex	de,hl
	sbc	hl,hl
	ld	l,c
	call	.load
	ld	c,a
.load:
	ld	b,8
.loop:	add	hl,hl
	add	hl,de
	jr	c,.skip
	sbc	hl,de
.skip:	rla
	djnz	.loop
	ret				; ca = c*256/a, h = c*256%a

;-------------------------------------------------------------------------------
_DivideHLBC: ; COPIED_FROM_GRAPHX
; Performs signed integer division
; Inputs:
;  HL : Operand 1
;  BC : Operand 2
; Outputs:
;  HL = HL/BC
	ex	de,hl
	xor	a,a
	sbc	hl,hl
	sbc	hl,bc
	jp	p,.next0
	push	hl
	pop	bc
	inc	a
.next0:
	or	a,a
	sbc	hl,hl
	sbc	hl,de
	jp	m,.next1
	ex	de,hl
	inc	a
.next1:
	add	hl,de
	rra
	ld	a,24
.loop:
	ex	de,hl
	adc	hl,hl
	ex	de,hl
	adc	hl,hl
	add	hl,bc
	jr	c,.spill
	sbc	hl,bc
.spill:
	dec	a
	jr	nz,.loop

	ex	de,hl
	adc	hl,hl
	ret	c
	ex	de,hl
	sbc	hl,hl
	sbc	hl,de
	ret

;-------------------------------------------------------------------------------
_MultiplyHLDE: ; COPIED_FROM_GRAPHX
; Performs (un)signed integer multiplication
; Inputs:
;  HL : Operand 1
;  DE : Operand 2
; Outputs:
;  HL = HL*DE
	push	de
	pop	bc

;-------------------------------------------------------------------------------
_MultiplyHLBC: ; COPIED_FROM_GRAPHX
; Performs (un)signed integer multiplication
; Inputs:
;  HL : Operand 1
;  BC : Operand 2
; Outputs:
;  HL = HL*BC
	push	iy
	push	hl
	push	bc
	push	hl
	ld	iy,0
	ld	d,l
	ld	e,b
	mlt	de
	add	iy,de
	ld	d,c
	ld	e,h
	mlt	de
	add	iy,de
	ld	d,c
	ld	e,l
	mlt	de
	ld	c,h
	mlt	bc
	ld	a,c
	inc	sp
	inc	sp
	pop	hl
	mlt	hl
	add	a,l
	pop	hl
	inc	sp
	mlt	hl
	add	a,l
	ld	b,a
	ld	c,0
	lea	hl,iy+0
	add	hl,bc
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,de
	pop	iy
	ret

;-------------------------------------------------------------------------------
_ComputeOutcode:
; Compute the bitcode for a point (x, y) using the clip rectangle
; bounded diagonally by (xmin, ymin), and (xmax, ymax)
; Inputs:
;  HL : X Argument
;  DE : Y Argument
; Outputs:
;   A : Bitcode
	ld	bc,0
smcWord _XMin
	push	hl
	xor	a,a
	sbc	hl,bc
	pop	bc
	add	hl,hl
	jp	po,.skip1
	ccf
.skip1:
	rla
	ld	hl,ti.lcdWidth-1
smcWord _XMaxMinus1
	sbc	hl,bc
	add	hl,hl
	jp	po,.skip2
	ccf
.skip2:
	rla
	ld	hl,0
smcWord _YMin
	scf
	sbc	hl,de
	add	hl,hl
	jp	pe,.skip3
	ccf
.skip3:
	rla
	ld	hl,ti.lcdHeight-1
smcWord _YMaxMinus1
	sbc	hl,de
	add	hl,hl
	rla
	ret	po
	xor	a,1
	ret

;-------------------------------------------------------------------------------
util.getbuffer: ; COPIED_FROM_GRAPHX
	ld	hl,ti.vRam + LcdSize
	ld	de,(ti.mpLcdBase)
	or	a,a
	sbc	hl,de
	add	hl,de
	jr	nz,.check
	ld	hl,ti.vRam
.check:
	or	a,a			; if 0, copy buffer to screen
	ret	nz
	ex	de,hl
	ret

;-------------------------------------------------------------------------------
; _ShiftCalculate:

;-------------------------------------------------------------------------------
_SetSmcBytes: ; COPIED_FROM_GRAPHX
	pop	hl
	pop	de
	pop	bc
	push	bc
	push	de
	ld	b,(hl)
.loop:
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	inc.s	hl
	add	hl,de
	ld	a,(hl)
	ld	(hl),c
	ex	de,hl
	djnz	.loop
	ret



;-------------------------------------------------------------------------------
; Internal library data
;-------------------------------------------------------------------------------

_CharSpacing:
	dl	_DefaultCharSpacing
_TextData:
	dl	_DefaultTextData

_DefaultCharSpacing: ; COPIED_FROM_GRAPHX
	;	0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F
	db	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
	db	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
	db	3,4,6,8,8,8,8,5,5,5,8,7,4,7,3,8
	db	8,7,8,8,8,8,8,8,8,8,3,4,6,7,6,7
	db	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
	db	8,8,8,8,8,8,8,8,8,8,8,5,8,5,8,8
	db	4,8,8,8,8,8,8,8,8,5,8,8,5,8,8,8
	db	8,8,8,8,7,8,8,8,8,8,8,7,3,7,8,8
	db	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
	db	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8

_DefaultTextData: ; COPIED_FROM_GRAPHX
	db	$00,$00,$00,$00,$00,$00,$00,$00 ;  
	db	$7E,$81,$A5,$81,$BD,$BD,$81,$7E ; ☺
	db	$7E,$FF,$DB,$FF,$C3,$C3,$FF,$7E ; ☻
	db	$6C,$FE,$FE,$FE,$7C,$38,$10,$00 ; ♥
	db	$10,$38,$7C,$FE,$7C,$38,$10,$00 ; ♦
	db	$38,$7C,$38,$FE,$FE,$10,$10,$7C ; ♣
	db	$00,$18,$3C,$7E,$FF,$7E,$18,$7E ; ♠
	db	$00,$00,$18,$3C,$3C,$18,$00,$00 ; •
	db	$FF,$FF,$E7,$C3,$C3,$E7,$FF,$FF ; ◘
	db	$00,$3C,$66,$42,$42,$66,$3C,$00 ; ○
	db	$FF,$C3,$99,$BD,$BD,$99,$C3,$FF ; ◙
	db	$0F,$07,$0F,$7D,$CC,$CC,$CC,$78 ; ♂
	db	$3C,$66,$66,$66,$3C,$18,$7E,$18 ; ♀
	db	$3F,$33,$3F,$30,$30,$70,$F0,$E0 ; ♪
	db	$7F,$63,$7F,$63,$63,$67,$E6,$C0 ; ♫
	db	$99,$5A,$3C,$E7,$E7,$3C,$5A,$99 ; *
	db	$80,$E0,$F8,$FE,$F8,$E0,$80,$00 ; ►
	db	$02,$0E,$3E,$FE,$3E,$0E,$02,$00 ; ◄
	db	$18,$3C,$7E,$18,$18,$7E,$3C,$18 ; ↕
	db	$66,$66,$66,$66,$66,$00,$66,$00 ; ‼
	db	$7F,$DB,$DB,$7B,$1B,$1B,$1B,$00 ; ¶
	db	$3F,$60,$7C,$66,$66,$3E,$06,$FC ; §
	db	$00,$00,$00,$00,$7E,$7E,$7E,$00 ; ▬
	db	$18,$3C,$7E,$18,$7E,$3C,$18,$FF ; ↨
	db	$18,$3C,$7E,$18,$18,$18,$18,$00 ; ↑
	db	$18,$18,$18,$18,$7E,$3C,$18,$00 ; ↓
	db	$00,$18,$0C,$FE,$0C,$18,$00,$00 ; →
	db	$00,$30,$60,$FE,$60,$30,$00,$00 ; ←
	db	$00,$00,$C0,$C0,$C0,$FE,$00,$00 ; └
	db	$00,$24,$66,$FF,$66,$24,$00,$00 ; ↔
	db	$00,$18,$3C,$7E,$FF,$FF,$00,$00 ; ▲
	db	$00,$FF,$FF,$7E,$3C,$18,$00,$00 ; ▼
	db	$00,$00,$00,$00,$00,$00,$00,$00 ;
	db	$C0,$C0,$C0,$C0,$C0,$00,$C0,$00 ; !
	db	$D8,$D8,$D8,$00,$00,$00,$00,$00 ; "
	db	$6C,$6C,$FE,$6C,$FE,$6C,$6C,$00 ; #
	db	$18,$7E,$C0,$7C,$06,$FC,$18,$00 ; $
	db	$00,$C6,$CC,$18,$30,$66,$C6,$00 ; %
	db	$38,$6C,$38,$76,$DC,$CC,$76,$00 ; &
	db	$30,$30,$60,$00,$00,$00,$00,$00 ; '
	db	$30,$60,$C0,$C0,$C0,$60,$30,$00 ; (
	db	$C0,$60,$30,$30,$30,$60,$C0,$00 ; )
	db	$00,$66,$3C,$FF,$3C,$66,$00,$00 ; *
	db	$00,$30,$30,$FC,$FC,$30,$30,$00 ; +
	db	$00,$00,$00,$00,$00,$60,$60,$C0 ; ,
	db	$00,$00,$00,$FC,$00,$00,$00,$00 ; -
	db	$00,$00,$00,$00,$00,$C0,$C0,$00 ; .
	db	$06,$0C,$18,$30,$60,$C0,$80,$00 ; /
	db	$7C,$CE,$DE,$F6,$E6,$C6,$7C,$00 ; 0
	db	$30,$70,$30,$30,$30,$30,$FC,$00 ; 1
	db	$7C,$C6,$06,$7C,$C0,$C0,$FE,$00 ; 2
	db	$FC,$06,$06,$3C,$06,$06,$FC,$00 ; 3
	db	$0C,$CC,$CC,$CC,$FE,$0C,$0C,$00 ; 4
	db	$FE,$C0,$FC,$06,$06,$C6,$7C,$00 ; 5
	db	$7C,$C0,$C0,$FC,$C6,$C6,$7C,$00 ; 6
	db	$FE,$06,$06,$0C,$18,$30,$30,$00 ; 7
	db	$7C,$C6,$C6,$7C,$C6,$C6,$7C,$00 ; 8
	db	$7C,$C6,$C6,$7E,$06,$06,$7C,$00 ; 9
	db	$00,$C0,$C0,$00,$00,$C0,$C0,$00 ; :
	db	$00,$60,$60,$00,$00,$60,$60,$C0 ; ;
	db	$18,$30,$60,$C0,$60,$30,$18,$00 ; <
	db	$00,$00,$FC,$00,$FC,$00,$00,$00 ; =
	db	$C0,$60,$30,$18,$30,$60,$C0,$00 ; >
	db	$78,$CC,$18,$30,$30,$00,$30,$00 ; ?
	db	$7C,$C6,$DE,$DE,$DE,$C0,$7E,$00 ; @
	db	$38,$6C,$C6,$C6,$FE,$C6,$C6,$00 ; A
	db	$FC,$C6,$C6,$FC,$C6,$C6,$FC,$00 ; B
	db	$7C,$C6,$C0,$C0,$C0,$C6,$7C,$00 ; C
	db	$F8,$CC,$C6,$C6,$C6,$CC,$F8,$00 ; D
	db	$FE,$C0,$C0,$F8,$C0,$C0,$FE,$00 ; E
	db	$FE,$C0,$C0,$F8,$C0,$C0,$C0,$00 ; F
	db	$7C,$C6,$C0,$C0,$CE,$C6,$7C,$00 ; G
	db	$C6,$C6,$C6,$FE,$C6,$C6,$C6,$00 ; H
	db	$7E,$18,$18,$18,$18,$18,$7E,$00 ; I
	db	$06,$06,$06,$06,$06,$C6,$7C,$00 ; J
	db	$C6,$CC,$D8,$F0,$D8,$CC,$C6,$00 ; K
	db	$C0,$C0,$C0,$C0,$C0,$C0,$FE,$00 ; L
	db	$C6,$EE,$FE,$FE,$D6,$C6,$C6,$00 ; M
	db	$C6,$E6,$F6,$DE,$CE,$C6,$C6,$00 ; N
	db	$7C,$C6,$C6,$C6,$C6,$C6,$7C,$00 ; O
	db	$FC,$C6,$C6,$FC,$C0,$C0,$C0,$00 ; P
	db	$7C,$C6,$C6,$C6,$D6,$DE,$7C,$06 ; Q
	db	$FC,$C6,$C6,$FC,$D8,$CC,$C6,$00 ; R
	db	$7C,$C6,$C0,$7C,$06,$C6,$7C,$00 ; S
	db	$FF,$18,$18,$18,$18,$18,$18,$00 ; T
	db	$C6,$C6,$C6,$C6,$C6,$C6,$FE,$00 ; U
	db	$C6,$C6,$C6,$C6,$C6,$7C,$38,$00 ; V
	db	$C6,$C6,$C6,$C6,$D6,$FE,$6C,$00 ; W
	db	$C6,$C6,$6C,$38,$6C,$C6,$C6,$00 ; X
	db	$C6,$C6,$C6,$7C,$18,$30,$E0,$00 ; Y
	db	$FE,$06,$0C,$18,$30,$60,$FE,$00 ; Z
	db	$F0,$C0,$C0,$C0,$C0,$C0,$F0,$00 ; [
	db	$C0,$60,$30,$18,$0C,$06,$02,$00 ; \
	db	$F0,$30,$30,$30,$30,$30,$F0,$00 ; ]
	db	$10,$38,$6C,$C6,$00,$00,$00,$00 ; ^
	db	$00,$00,$00,$00,$00,$00,$00,$FF ; _
	db	$C0,$C0,$60,$00,$00,$00,$00,$00 ; `
	db	$00,$00,$7C,$06,$7E,$C6,$7E,$00 ; a
	db	$C0,$C0,$C0,$FC,$C6,$C6,$FC,$00 ; b
	db	$00,$00,$7C,$C6,$C0,$C6,$7C,$00 ; c
	db	$06,$06,$06,$7E,$C6,$C6,$7E,$00 ; d
	db	$00,$00,$7C,$C6,$FE,$C0,$7C,$00 ; e
	db	$1C,$36,$30,$78,$30,$30,$78,$00 ; f
	db	$00,$00,$7E,$C6,$C6,$7E,$06,$FC ; g
	db	$C0,$C0,$FC,$C6,$C6,$C6,$C6,$00 ; h
	db	$60,$00,$E0,$60,$60,$60,$F0,$00 ; i
	db	$06,$00,$06,$06,$06,$06,$C6,$7C ; j
	db	$C0,$C0,$CC,$D8,$F8,$CC,$C6,$00 ; k
	db	$E0,$60,$60,$60,$60,$60,$F0,$00 ; l
	db	$00,$00,$CC,$FE,$FE,$D6,$D6,$00 ; m
	db	$00,$00,$FC,$C6,$C6,$C6,$C6,$00 ; n
	db	$00,$00,$7C,$C6,$C6,$C6,$7C,$00 ; o
	db	$00,$00,$FC,$C6,$C6,$FC,$C0,$C0 ; p
	db	$00,$00,$7E,$C6,$C6,$7E,$06,$06 ; q
	db	$00,$00,$FC,$C6,$C0,$C0,$C0,$00 ; r
	db	$00,$00,$7E,$C0,$7C,$06,$FC,$00 ; s
	db	$30,$30,$FC,$30,$30,$30,$1C,$00 ; t
	db	$00,$00,$C6,$C6,$C6,$C6,$7E,$00 ; u
	db	$00,$00,$C6,$C6,$C6,$7C,$38,$00 ; v
	db	$00,$00,$C6,$C6,$D6,$FE,$6C,$00 ; w
	db	$00,$00,$C6,$6C,$38,$6C,$C6,$00 ; x
	db	$00,$00,$C6,$C6,$C6,$7E,$06,$FC ; y
	db	$00,$00,$FE,$0C,$38,$60,$FE,$00 ; z
	db	$1C,$30,$30,$E0,$30,$30,$1C,$00 ; {
	db	$C0,$C0,$C0,$00,$C0,$C0,$C0,$00 ; |
	db	$E0,$30,$30,$1C,$30,$30,$E0,$00 ; }
	db	$76,$DC,$00,$00,$00,$00,$00,$00 ; ~
	db	$00,$10,$38,$6C,$C6,$C6,$FE,$00 ; △

_LcdTiming:
;	db	14 shl 2		; PPL shl 2
	db	7			; HSW
	db	87			; HFP
	db	63			; HBP
	dw	(0 shl 10)+319		; (VSW shl 10)+LPP
	db	179			; VFP
	db	0			; VBP
	db	(0 shl 6)+(0 shl 5)+0	; (ACB shl 6)+(CLKSEL shl 5)+PCD_LO
;  H = ((PPL+1)*16)+(HSW+1)+(HFP+1)+(HBP+1) = 240+8+88+64 = 400
;  V = (LPP+1)+(VSW+1)+VFP+VBP = 320+1+179+0 = 500
; CC = H*V*PCD*2 = 400*500*2*2 = 800000
; Hz = 48000000/CC = 60

_TmpCharSprite:
	db	8,8
_TmpCharData:
	db	0,0,0,0,0,0,0,0
	db	0,0,0,0,0,0,0,0
	db	0,0,0,0,0,0,0,0
	db	0,0,0,0,0,0,0,0
	db	0,0,0,0,0,0,0,0
	db	0,0,0,0,0,0,0,0
	db	0,0,0,0,0,0,0,0
	db	0,0,0,0,0,0,0,0

;-------------------------------------------------------------------------------

	ld a, 0
gfy_Color := $-1
smcByte _Color

	ld	a, TRASPARENT_COLOR
gfy_Transparent_Color := $-1
smcByte _TransparentColor

	ld	a, TEXT_FG_COLOR
gfy_Text_FG_Color := $-1
smcByte _TextFGColor

	ld	a, TEXT_BG_COLOR
gfy_Text_BG_Color := $-1
smcByte _TextBGColor

	ld	a, TEXT_TP_COLOR
gfy_Text_TP_Color := $-1
smcByte _TextTPColor

	ld	a, 8
gfy_FontHeight := $-1
smcByte _TextHeight

;-------------------------------------------------------------------------------

	ld	hl, 0
gfy_ClipXMin := $-3
smcWord _XMin

	ld	hl, 0
gfy_ClipYMin := $-3
smcByte _YMin

	ld	hl, ti.lcdWidth
gfy_ClipXMax := $-3
smcWord _XMax

	ld	hl, ti.lcdHeight
gfy_ClipYMax := $-3
smcByte _YMax

	ld	hl, ti.lcdWidth
gfy_ClipXSpan := $-3
smcWord _XSpan

	ld	hl, ti.lcdHeight
gfy_ClipYSpan := $-3
smcWord _YSpan

	ld	hl, ti.lcdWidth-1
gfy_ClipXMaxMinus1 := $-3
smcWord _XMaxMinus1

	ld	hl, ti.lcdHeight-1
gfy_ClipYMaxMinus1 := $-3
smcWord _YMaxMinus1

;-------------------------------------------------------------------------------

; export	gfy_DefaultCharSpacing
; export	gfy_DefaultTextData
; export	gfy_CharSpacing
; export	gfy_TextData

; export	gfy_TextXPos
; export	gfy_TextYPos
; export	gfy_TextWidthScale
; export	gfy_TextHeightScale
; export	gfy_FontHeight
; export	gfy_MonospaceFont
; export	gfy_TmpCharSprite

; export	gfy_Color
; export	gfy_Transparent_Color
; export	gfy_Text_FG_Color
; export	gfy_Text_BG_Color
; export	gfy_Text_TP_Color

; export	gfy_ClipXMin
; export	gfy_ClipYMin
; export	gfy_ClipXMax
; export	gfy_ClipYMax
; export	gfy_ClipXSpan
; export	gfy_ClipYSpan
; export	gfy_ClipXMaxMinus1
; export	gfy_ClipYMaxMinus1

gfy_DefaultCharSpacing := _DefaultCharSpacing
gfy_DefaultTextData    := _DefaultTextData
gfy_CharSpacing        := _CharSpacing
gfy_TextData           := _TextData

gfy_TextXPos        := _TextXPos
gfy_TextYPos        := _TextYPos
gfy_TextWidthScale  := _TextWidthScale
gfy_TextHeightScale := _TextHeightScale
; gfy_FontHeight      := _TextHeight
gfy_MonospaceFont   := _TextFixedWidth
gfy_TmpCharSprite   := _TmpCharSprite

; gfy_Color             := _Color
; gfy_Transparent_Color := _TransparentColor
; gfy_Text_FG_Color     := _TextFGColor
; gfy_Text_BG_Color     := _TextBGColor
; gfy_Text_TP_Color     := _TextTPColor

; gfy_ClipXMin := _XMin
; gfy_ClipYMin := _YMin
; gfy_ClipXMax := _XMax
; gfy_ClipYMax := _YMax
; gfy_ClipXSpan := _XSpan
; gfy_ClipYSpan := _YSpan
; gfy_ClipXMaxMinus1 := _XMaxMinus1
; gfy_ClipYMaxMinus1 := _YMaxMinus1

gfy_SineTable := _SineTable

;-------------------------------------------------------------------------------
; libc
;-------------------------------------------------------------------------------

; __bremu     := 
_memcpy     := $0000A4
__idivs     := $00013C
__idivu     := $000140
__ixor      := $000198
__sremu     := $00023C
__ineg      := $000160
; __indcallhl := 
_memmove    := $0000A8
__imulu     := $000154
__ishl      := $000174
__setflag   := $000218
__iand      := $000134
_memset     := $0000AC
__frameset  := $00012C
__iremu     := $000170
__bshl      := $000100

__bremu:
; I: A=dividend, C=divisor
; O: a=A%C
	push	bc
	push	hl
	ld	b, a
	call	__bdvrmu
	ld	a, h
	pop	hl
	pop	bc
	ret

__bdvrmu:
; I: B=dividend, C=divisor
; O: a=?, b=0, h=B%C, l=B/C
	ld	l, b
.hijack_l_dividend:
	ld	h, 0
	ld	b, 8
.loop:
	add	hl, hl
	ld	a, h
	sub	a, c
	jr	c, .bit_skip
	ld	h, a
	inc	l
.bit_skip:
	djnz	.loop
	ret

__indcallhl:
	jp	(hl)

;-------------------------------------------------------------------------------
; graphy_lcddrvce.asm
;-------------------------------------------------------------------------------

include 'graphy_lcddrvce.asm'

;-------------------------------------------------------------------------------
; graphy.c.src
;-------------------------------------------------------------------------------

include 'graphy.c.src'
