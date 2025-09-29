	assume	adl=1

	section	.text

	public	__i48mulhu

; UDE:UHL = ((uint96_t)UDE:UHL * (uint96_t)UIY:UBC) >> 48
__i48mulhu:
	push	ix
	push	iy
	push	bc
	ld	ix, 0
	lea	iy, ix
	add	ix, sp
	push	de
	push	hl

	; x_lo * y_lo
	lea	de, iy
	call	__i48mulu
	push	de		; UHL * UBC (low carry)

	; x_hi * y_lo
	lea	de, iy
	ld	hl, (ix - 3)
	call	__i48mulu
	push	de		; hi24
	push	hl		; lo24

	; x_lo * y_hi
	lea	de, iy
	ld	bc, (ix + 3)
	ld	hl, (ix - 6)
	call	__i48mulu
	pop	bc		; lo24
	add	hl, bc
	ex	de, hl
	pop	bc		; hi24
	adc	hl, bc

	pop	bc		; UHL * UBC (low carry)
	ex	de, hl
	add	hl, bc
	jr	nc, .no_low_carry
	inc	de
.no_low_carry:
	push	de		; high carry

	; x_hi * y_hi
	lea	de, iy
	ld	bc, (ix + 3)
	ld	hl, (ix - 3)
	call	__i48mulu
	pop	bc		; high carry
	add	hl, bc
	jr	nc, .no_high_carry
	inc	de
.no_high_carry:
	ld	sp, ix
	pop	bc
	pop	iy
	pop	ix
	ret

	extern	__i48mulu
