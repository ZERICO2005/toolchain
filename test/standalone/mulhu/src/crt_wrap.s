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
	push	de
	; backup af
	push	af
	push	ix
	ld	ix, 0
	add	ix, sp

	; On stack to get upper byte when needed
	push	de		; de will also be used to perform the actual multiplication
	push	hl
	push	iy
	push	bc

	; bc = a[0], a[1]
	ld	a, l		; a = b[0]
	ld	iy, (ix - 5)	; iy = b[1], b[2]

	; or	a, a		; carry is already cleared
	sbc	hl, hl
	push	hl		; upper bytes of sum at -15
	; Stack Use:
	; ix-1  : deu b[5]
	; ix-2  : d   b[4]
	; ix-3  : e   b[3]
	; ix-4  : hlu b[2]
	; ix-5  : h   b[1]
	; ix-6  : l   b[0]
	; ix-7  : iyu a[5]
	; ix-8  : iyh a[4]
	; ix-9  : iyl a[3]
	; ix-10 : bcu a[2]
	; ix-11 : b   a[1]
	; ix-12 : c   a[0]
	; ix-13 :   sum[5]
	; ix-14 :   sum[4]
	; ix-15 :   sum[3]
	; ix-16 :   sum[2]
	; ix-17 :   sum[1]
	; ix-18 :   sum[0]

	; ======================================================================
	; sum[0-1]

	; a[0]*b[0]
	ld	d, c		; d = a[0]
	ld	e, a		; e = b[0]
	mlt	de
	push	de		; lower bytes of sum at -18

	; ======================================================================
	; sum[1-2]
	ld	l, d		; hl will store current partial sum

	; a[1]*b[0]
	ld	d, b		; d = a[1]
	ld	e, a		; e = b[0]
	mlt	de
	add	hl, de

	; a[0]*b[1]
	ld	d, c		; d = a[0]
	ld	e, iyl		; e = b[1]
	mlt	de
	add	hl, de

	ld	(ix - 17), hl

	; ======================================================================
	; sum[2-3]
	ld	hl, (ix - 16)	; hl will store current partial sum

	; a[0]*b[2]
	ld	d, c		; d = a[0]
	ld	e, iyh		; e = b[2]
	mlt	de
	add	hl, de

	; a[1]*b[1]
	ld	d, b		; d = a[1]
	ld	e, iyl		; e = b[1]
	mlt	de
	add	hl, de

	; a[2]*b[0]
	ld	d, (ix - 10)	; d = a[2]
	ld	e, a		; e = b[0]
	mlt	de
	add	hl, de

	ld	(ix - 16), hl

	; ======================================================================
	; sum[3-4]
	ld	hl, (ix - 15)	; hl will store current partial sum

	; a[1]*b[2]
	ld	d, b		; d = a[1]
	ld	e, iyh		; e = b[2]
	mlt	de
	add	hl, de

	; a[2]*b[1]
	ld	d, (ix - 10)	; d = a[2]
	ld	e, iyl		; e = b[1]
	mlt	de
	add	hl, de

	ld	(ix - 15), hl

	; ======================================================================
	; sum[4-5]
	ld	hl, (ix - 14)	; hl will store current partial sum

	; a[2]*b[2]
	ld	d, (ix - 10)	; d = a[2]
	ld	e, iyh		; e = b[2]
	mlt	de
	add	hl, de

	ld	(ix - 14), l
	ld	(ix - 13), h

	; clean up stack and restore registers
	pop	de
	pop	hl
	pop	bc
	pop	iy

	ld	sp, ix
	pop	ix
	pop	af
	pop	de
	ret
