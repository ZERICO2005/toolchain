	assume	adl=1

	section	.text

	public	__i48mulhu

; UDE:UHL = ((uint96_t)UDE:UHL * (uint96_t)UIY:UBC) >> 48
__i48mulhu:
	push	ix
	push	iy
	push	bc
	push	de
	push	hl
	ld	ix, 0
	add	ix, sp
	push	af, af, af, af
	ld	iy, 0

	; x_lo * y_lo
	lea	de, iy
	ld	bc, (ix + 6)
	ld	hl, (ix + 0)
	call	__i48mulu
	ld	(ix - 3), de	; (uint24_t)(UDE:UHL >> 24)

	; x_hi * y_lo
	lea	de, iy
	ld	bc, (ix + 6)
	ld	hl, (ix + 3)
	call	__i48mulu
	ld	(ix - 9), hl
	ld	(ix - 6), de

	; x_lo * y_hi
	lea	de, iy
	ld	bc, (ix + 9)
	ld	hl, (ix + 0)
	call	__i48mulu
	; ld	(ix - 18), de
	; ld	(ix - 21), hl

	ld	bc, (ix - 3)
	add	hl, bc
	jr	nc, .no_carry_1
	inc	de
.no_carry_1:
	ld	bc, (ix - 9)
	add	hl, bc
	ex	de, hl
	ld	bc, (ix - 6)
	adc	hl, bc
	ld	(ix - 12), hl

	; x_hi * y_hi
	lea	de, iy
	ld	bc, (ix + 9)
	ld	hl, (ix + 3)
	call	__i48mulu
	ld	bc, (ix - 12)
	add	hl, bc
	jr	nc, .no_carry_2
	inc	de
.no_carry_2:
	ld	sp, ix
	pop	bc		; reset SP
	pop	bc		; reset SP
	pop	bc
	pop	iy
	pop	ix
	ret

	extern	__i48mulu
