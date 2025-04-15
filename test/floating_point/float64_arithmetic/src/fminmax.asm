	assume	adl=1

	section	.text

	public	_myfmaxl, _myfminl

_myfmaxl:
	ld	iy, 0
	add	iy, sp
	; load y into registers
	ld	hl, (iy + 12)
	ld	de, (iy + 15)
	ld	bc, (iy + 18)
	pop	iy
	call	__llcmpu	; x is on the stack
	push	iy
	jr	nz, _myfminl.not_equal
	ret	; equal

_myfminl:
	ld	iy, 0
	add	iy, sp
	; load y into registers
	ld	hl, (iy + 12)
	ld	de, (iy + 15)
	ld	bc, (iy + 18)
	pop	iy
	call	__llcmpu	; x is on the stack
	push	iy
	ret	z	; equal
	ccf

_myfminl.not_equal:
	rra
	xor	a, b	; y ^ Carry

	push	af
	push	bc, de, hl
	call	__isnanl
	pop	af, af, af
	ld	l, a
	pop	af

	ld	iy, 0
	add	iy, sp
	bit	0, l
	jr	nz, .ret_x
	
	rla
	jr	c, .ret_x
	lea	iy, iy + 9
.ret_x:
	ld	hl, (iy + 3)
	ld	de, (iy + 6)
	ld	bc, (iy + 9)
	ret

	extern	__llcmpu
	extern	__isnanl
