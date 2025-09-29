	assume	adl=1

	section	.text

	public	__imulhu

; UHL = ((uint48_t)UHL * (uint48_t)UBC) >> 24
__imulhu:
; CC: 20 bytes | 223F + 60R + 44W + 86
	push	iy, de
	ld	iy, 0
	lea	de, iy
	call	__i48mulu
	ex	de, hl
	pop	de, iy
	ret

	extern	__i48mulu
