	assume	adl=1

	section	.text

	public	__llmulhu

; BC:UDE:UHL = ((uint128_t)BC:UDE:UHL * (uint128_t)UIY:UBC) >> 64
__llmulhu:
	push	ix
	push	iy
	ld	ix, -36
	add	ix, sp
	ld	sp, ix
	lea	ix, ix + 36

	ld	(ix - 3), bc
	ld	(ix - 6), de
	ld	(ix - 9), hl

	ld	(ix - 1), 0

	ld	bc, 0
	ld	(ix - 30), bc
	ld	c, (ix + 12)
	ld	(ix - 33), bc
	ld	iy, (ix + 9)
	ld	(ix - 36), iy
	
	; x_lo * y_lo
	ld	c, b
	ld	d, b
	inc	de
	dec.s	de
	call	__llmulu
	; -10 0
	; -11 0
	; -12 0
	;
	; -13 0
	; -14 0
	; -15 B
	;
	; -16 C
	; -17 UDE
	; -18 D
	; -19 0
	; -20 0
	; -21
	; -22
	; -23
	; -24
	ld	(ix - 16), bc
	ld	(ix - 19), de
	ld	(ix - 19), 0
	ld	bc, 0
	ld	(ix - 14), bc
	ld	(ix - 12), bc

	; x_hi * y_lo
	inc.s	de
	ld	d, b
	ld	e, (ix - 2)
	ld	hl, (ix - 5)
	call	__llmulu
	ld	(ix - 21), bc
	ld	(ix - 24), de
	ld	(ix - 27), hl

	ld	c, (ix + 16)
	ld	(ix - 33), c
	ld	iy, (ix + 13)
	ld	(ix - 36), iy

	; x_lo * y_hi
	ld	bc, 0
	inc.s	de
	ld	d, b
	ld	e, (ix - 6)
	ld	hl, (ix - 9)
	call	__llmulu
	lea	iy, ix - 27
	call	__llmulhu_add
	lea	iy, ix - 18
	call	__llmulhu_add
	ld	(ix - 16), bc
	ld	(ix - 19), de
	ld	(ix - 19), 0
	ld	bc, 0
	ld	(ix - 14), bc
	ld	(ix - 12), bc
	
	; x_hi * y_hi
	inc.s	de
	ld	d, b
	ld	e, (ix - 2)
	ld	hl, (ix - 5)
	call	__llmulu
	lea	iy, ix - 18
	call	__llmulhu_add
	ld	sp, ix
	pop	iy
	pop	ix
	ret

__llmulhu_add:
	; similar to __lladd, except iy points to the stack and is destroyed
	push	bc
	ld	bc, (iy + 0)
	add	hl, bc
	ex	de, hl
	ld	bc, (iy + 3)
	adc	hl, bc
	ex	de, hl
	pop	bc
	jr	nc, .no_carry48
	inc	bc
.no_carry48:
	ld	iy, (iy + 6)
	add	iy, bc
	lea	bc, iy
	ret

__set_de_to_ude_zext:
	push	hl
	push	de
	ld	de, 0
	ld	hl, 2
	add	hl, sp
	ld	e, (hl)
	pop	hl
	pop	hl
	ret

	extern	__llmulu
