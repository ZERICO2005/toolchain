	assume	adl=1

	section	.text

	public	__i48mulhu

; UDE:UHL = ((uint96_t)UDE:UHL * (uint96_t)UIY:UBC) >> 48
__i48mulhu:
	push	ix
	push	iy
	push	bc
	ld	ix, 0
	add	ix, sp
	push	de
	push	hl
	push	af
	ld	iy, 0

	; x_lo * y_lo
	lea	de, iy
	call	__i48mulu
	ld	(ix - 9), de	; (uint24_t)(UDE:UHL >> 24)

	; x_hi * y_lo
	lea	de, iy
	ld	hl, (ix - 3)
	call	__i48mulu
	; ld	(ix - 12), de
	; ld	(ix - 15), hl
	push	de		; hi24
	push	hl		; lo24

	; x_lo * y_hi
	lea	de, iy
	ld	bc, (ix + 3)
	ld	hl, (ix - 6)
	call	__i48mulu
	; ld	(ix - 18), de
	; ld	(ix - 21), hl

	ld	bc, (ix - 9)
	add	hl, bc
	jr	nc, .no_carry_1
	inc	de
.no_carry_1:
	; ld	bc, (ix - 15)
	pop	bc		; lo24
	add	hl, bc
	ex	de, hl
	pop	bc		; hi24
	; ld	bc, (ix - 12)
	adc	hl, bc
	push	hl		; low order terms

	; x_hi * y_hi
	lea	de, iy
	ld	bc, (ix + 3)
	ld	hl, (ix - 3)
	call	__i48mulu
	pop	bc		; low order terms
	add	hl, bc
	jr	nc, .no_carry_2
	inc	de
.no_carry_2:
	ld	sp, ix
	pop	bc
	pop	iy
	pop	ix
	ret

	extern	__i48mulu
