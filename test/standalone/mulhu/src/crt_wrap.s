	.assume	adl=1

	.section	.data._prev_reg

	.global	_prev_reg
	.type	_prev_reg, @function
_prev_reg:
	;	L H U  E D U  C B U  A  Y I U  X I U
	db	0,0,0, 0,0,0, 0,0,0, 0, 0,0,0, 0,0,0

	.section	.data._next_reg

	.global	_next_reg
	.type	_next_reg, @function
_next_reg:
	;	L H U  E D U  C B U  A  Y I U  X I U
	db	0,0,0, 0,0,0, 0,0,0, 0, 0,0,0, 0,0,0

	.section	.text

	.local	_set_prev_reg
_set_prev_reg:
	ld	(_prev_reg +  0), hl
	ld	(_prev_reg +  3), de
	ld	(_prev_reg +  6), bc
	ld	(_prev_reg +  9), a
	ld	(_prev_reg + 10), iy
	ld	(_prev_reg + 13), ix
	ret

	.local	_set_next_reg
_set_next_reg:
	ld	(_next_reg +  0), hl
	ld	(_next_reg +  3), de
	ld	(_next_reg +  6), bc
	ld	(_next_reg +  9), a
	ld	(_next_reg + 10), iy
	ld	(_next_reg + 13), ix
	ret

	.global	_CRT_smulhu
	.type	_CRT_smulhu, @function
_CRT_smulhu:
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	b, (iy + 10)
	ld	c, (iy + 9)
	ld	h, (iy + 7)
	ld	l, (iy + 6)
	pop	iy
	call	_set_prev_reg
	call	__smulhu
	jp	_set_next_reg

	.global	_CRT_smulhs
	.type	_CRT_smulhs, @function
_CRT_smulhs:
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	b, (iy + 10)
	ld	c, (iy + 9)
	ld	h, (iy + 7)
	ld	l, (iy + 6)
	pop	iy
	call	_set_prev_reg
	call	__smulhs
	jp	_set_next_reg

	.global	_CRT_imulhu
	.type	_CRT_imulhu, @function
_CRT_imulhu:
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 6)
	ld	bc, (iy + 9)
	pop	iy
	call	_set_prev_reg
	call	__my_imulhu
	jp	_set_next_reg

	.global	_CRT_imulhs
	.type	_CRT_imulhs, @function
_CRT_imulhs:
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 6)
	ld	bc, (iy + 9)
	pop	iy
	call	_set_prev_reg
	call	__my_imulhs
	jp	_set_next_reg

	.global	_CRT_lmulhu
	.type	_CRT_lmulhu, @function
_CRT_lmulhu:
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 6)
	ld	e, (iy + 9)
	ld	bc, (iy + 12)
	ld	a, (iy + 15)
	pop	iy
	call	_set_prev_reg
	call	__lmulhu
	jp	_set_next_reg

	.global	_CRT_lmulhs
	.type	_CRT_lmulhs, @function
_CRT_lmulhs:
	push	iy
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 6)
	ld	e, (iy + 9)
	ld	bc, (iy + 12)
	ld	a, (iy + 15)
	pop	iy
	call	_set_prev_reg
	call	__lmulhs
	jp	_set_next_reg

	.global	_CRT_i48mulhu
	.type	_CRT_i48mulhu, @function
_CRT_i48mulhu:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 3)
	ld	de, (iy + 6)
	ld	bc, (iy + 9)
	ld	iy, (iy + 12)
	call	_set_prev_reg
	call	__i48mulhu
	jp	_set_next_reg

	.global	_CRT_i48mulhs
	.type	_CRT_i48mulhs, @function
_CRT_i48mulhs:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 3)
	ld	de, (iy + 6)
	ld	bc, (iy + 9)
	ld	iy, (iy + 12)
	call	_set_prev_reg
	call	__i48mulhs
	jp	_set_next_reg

	.global	_CRT_llmulhu
	.type	_CRT_llmulhu, @function
_CRT_llmulhu:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 18)
	push	hl
	ld	hl, (iy + 15)
	push	hl
	ld	hl, (iy + 12)
	push	hl
	ld	bc, (iy + 9)
	ld	de, (iy + 6)
	ld	hl, (iy + 3)
	call	_set_prev_reg
	call	__llmulhu
	ld	sp, iy
	jp	_set_next_reg

	.global	_CRT_llmulhs
	.type	_CRT_llmulhs, @function
_CRT_llmulhs:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 18)
	push	hl
	ld	hl, (iy + 15)
	push	hl
	ld	hl, (iy + 12)
	push	hl
	ld	bc, (iy + 9)
	ld	de, (iy + 6)
	ld	hl, (iy + 3)
	call	_set_prev_reg
	call	__llmulhs
	ld	sp, iy
	jp	_set_next_reg

	.extern	__smulhu
	.extern	__imulhu
	.extern	__lmulhu
	.extern	__i48mulhu
	.extern	__llmulhu

	.extern	__smulhs
	.extern	__imulhs
	.extern	__lmulhs
	.extern	__i48mulhs
	.extern	__llmulhs

__my_imulhs:
	push	af
	push	bc
	push	hl

	push	hl
	add	hl, hl
	rla
	ld	hl, $800000
	add	hl, bc
	rla
	pop	hl

	call	__my_imulhu

	; if (UBC < 0) { result -= UHL; }
	pop	bc
	cpl
	rra
	jr	c, .L.positive_bc
	sbc	hl, bc
.L.positive_bc:

	; if (UHL < 0) { result -= UBC; }
	pop	bc
	rra
	jr	c, .L.positive_hl
	sbc	hl, bc
.L.positive_hl:

	pop	af
	ret

; UHL = ((uint48_t)UHL * (uint48_t)UBC) >> 24
__my_imulhu:
; TODO: Optimize this routine as this is mostly just a copy paste of __i48mulu with some stuff removed.
;
; CC: 118*r(PC)+39*r(SPL)+38*w(SPL)+37
; CC: 117 bytes | 118F + 39R + 38W + 37

	; backup af
	push	af
	push	ix

	; On stack to get upper byte when needed
	push	de		; de will also be used to perform the actual multiplication
	push	hl
	push	iy
	push	bc

	ld	ix, 0
	push	ix		; upper bytes of sum at (ix + 0)
	add	ix, sp

	; Stack Use:
	; ix + 14 : deu X[5]
	; ix + 13 : d   X[4]
	; ix + 12 : e   X[3]
	; ix + 11 : hlu X[2]
	; ix + 10 : h   X[1]
	; ix + 9  : l   X[0]
	; ix + 8  : iyu Y[5]
	; ix + 7  : iyh Y[4]
	; ix + 6  : iyl Y[3]
	; ix + 5  : bcu Y[2]
	; ix + 4  : b   Y[1]
	; ix + 3  : c   Y[0]
	; ix + 2  :   sum[5]
	; ix + 1  :   sum[4]
	; ix + 0  :   sum[3]
	; ix - 1  :   sum[2]
	; ix - 2  :   sum[1]
	; ix - 3  :   sum[0]

	ld	iy, (ix + 10)	; iy = X[1], X[2]
	; bc = Y[0], Y[1]
	ld	a, l		; a = X[0]

	; ======================================================================
	; sum[0-1]

	; X[0]*Y[0]
	; l = X[0]
	ld	h, c		; h = Y[0]
	mlt	hl
	push	hl		; lower bytes of sum at (ix - 3)

	; ======================================================================
	; sum[1-2]
	ld	l, h		; hl will store current partial sum
	ld	h, 0

	; X[0]*Y[1]
	ld	e, a		; e = X[0]
	ld	d, b		; d = Y[1]
	mlt	de
	add	hl, de

	; X[1]*Y[0]
	ld	e, iyl		; e = X[1]
	ld	d, c		; d = Y[0]
	mlt	de
	add	hl, de

	ld	(ix - 2), hl

	; ======================================================================
	; sum[2-3]
	ld	hl, (ix - 1)	; hl will store current partial sum

	; X[2]*Y[0]
	ld	e, iyh		; e = X[2]
	ld	d, c		; d = Y[0]
	mlt	de
	add	hl, de

	; X[1]*Y[1]
	ld	e, iyl		; e = X[1]
	ld	d, b		; d = Y[1]
	mlt	de
	add	hl, de

	; X[0]*Y[2]
	ld	e, a		; e = X[0]
	ld	d, (ix + 5)	; d = Y[2]
	ld	c, d		; c = Y[2]
	mlt	de
	add	hl, de
	ld	d, c		; d = Y[2]

	ld	(ix - 1), hl

	; ======================================================================
	; sum[3-4]
	ld	hl, (ix + 0)	; hl will store current partial sum

	; X[2]*Y[1]
	ld	c, iyh		; c = X[2]
	; b = Y[1]
	mlt	bc
	add	hl, bc

	; X[1]*Y[2]
	ld	e, iyl		; e = X[1]
	; d = Y[2]
	mlt	de
	add	hl, de

	ld	(ix + 0), hl

	; ======================================================================
	; sum[4-5]
	ld	hl, (ix + 1)	; hl will store current partial sum

	; X[2]*Y[2]
	ld	e, iyh		; e = X[2]
	ld	d, (ix + 5)	; d = Y[2]
	mlt	de
	add	hl, de

	ld	a, l		; ld (ix + 1), l
	ld	(ix + 2), h

	; clean up stack and restore registers
	pop	hl		; reset SP
	pop	hl
	ld	h, a
	pop	bc
	pop	iy

	pop	de		; reset SP
	pop	de		; restore DE

	pop	ix
	pop	af
	ret
