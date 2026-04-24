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
	push	af
	push	de
	push	iy
	push	bc
	push	hl
	ld	d,l
	ld	e,c
	mlt	de
	ld	a,d
	ld	d,h
	ld	e,b
	ld	b,h
	ld	h,e
	mlt	hl
	mlt	bc
	mlt	de
	add	a,c
	ld	c,a
	ld	a,b
	adc	a,0h
	ld	b,a
	add.s	hl,bc
	jr	nc,_L0
	inc	d
_L0:
	ld	iy,0h
	add	iy,sp
	ld	b,(iy+2)
	ld	c,(iy+5)
	mlt	bc
	ld	a,h
	add	a,e
	ld	e,a
	jr	nc,_L1
	inc	d
	jr	nz,_L1
	inc	bc
_L1:
	ld	h,(iy+2)
	ld	l,(iy+3)
	mlt	hl

	add.s	hl,de
	jr	nc,_L2
	inc	bc
_L2:
	ld	d,(iy+0)
	ld	e,(iy+5)
	mlt	de
	add.s	hl,de
	jr	nc,_L3
	inc	bc
_L3:
	ld	a,h
	ld	d,(iy+2)
	ld	e,(iy+4)
	ld	h,(iy+1)
	ld	l,(iy+5)
	mlt	de
	mlt	hl
	add.s	hl,de
	jr	nc,_L4
	inc	b
_L4:
	add	a,l
	ld	l,a
	ld	a,h
	adc	a,c
	ld	h,a
	jr	nc,_L5
	inc	b
_L5:
	ex	de,hl
	push	bc
	dec	sp
	pop	hl
	inc	sp
	ld	h,d
	ld	l,e
	pop	bc
	pop	bc
	pop	iy
	pop	de
	pop	af
	ret
