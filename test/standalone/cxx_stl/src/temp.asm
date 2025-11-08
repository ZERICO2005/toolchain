	assume	adl=1

	section	.text

	public	_ungetc
_ungetc:
	xor	a, a
	ld	bc, 0
	ld	de, 0
	ld	hl, 0
	ret
