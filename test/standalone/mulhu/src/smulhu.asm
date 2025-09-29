	assume	adl=1

	section	.text

	public	__smulhu

; HL = ((uint32_t)HL * (uint32_t)BC) >> 16
__smulhu:
	push	af, de
	xor	a, a
	ld	e, a
	inc	hl
	dec.s	hl
	inc	bc
	dec.s	bc
	call	__lmulu

	dec	sp
	push	hl
	inc	sp
	pop	hl
	ld	l, h
	ld	h, e
	inc	hl
	dec.s	hl
	pop	de, af
	ret

	extern	__lmulu
