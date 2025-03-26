	assume	adl=1

	section	.text

	public	_my_floorf, _my_floor, _my_ceilf, _my_ceil

if 0

; float floorf(float)
_my_floorf:
_my_floor:
	ld	iy, 0
	add	iy, sp
	ld	e, (iy + 6)
	bit	7, e
	jp	z, _truncf
	
	ld	bc, (iy + 3)
	sbc	hl, hl
	add	hl, bc	; copy BC to HL
	add	hl, hl
	ld	a, e
	rla

	sub	a, 127	; exponent bias
	jr	c, .ret_zero
	sub	a, 23	; bits in the mantissa
	jr	nc, .ret_self
	ld	hl, 1
	neg
	ld	d, c	; store C
	ld	c, a
	call	__ishl
	ld	c, d	; restore C

	push	hl
	dec	hl
	call	__inot
	call	__iand
	
	push	hl
	or	a, a
	sbc	hl, bc	; tests if x was already an integer
	pop	hl

	pop	bc	
	jr	z, .skip
	add	hl, bc
.skip:
	ld	a, 0
	adc	a, e
	ld	e, a
	ret

.ret_zero:
	sbc	hl, hl
	add	hl, bc
	ld	a, e
	jr	c, .ret_one
	rla
	or	a, a
	ld	a, e
	jr	nz, .ret_one
	; return signed zero
	inc	hl
	and	a, $80
	ld	e, a
	ret
.ret_one:
	or	a, $3F
	ld	e, a
	ld	hl, $800000
	ret

.ret_self:
	; already (large) integer, inf, or NaN
	sbc	hl, hl
	add	hl, bc
	ret

;	extern	__ishl
;	extern	__iand
;	extern	__inot
;	extern	_truncf

end if


_my_floorf:
_my_floor:
	ld	iy, 0
	add	iy, sp
	ld	bc, (iy + 6)
	ld	a, (iy + 9)

; IEEE single precision floor
; aubc = floor(aubc)
__fpfloor: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), floorf(bitcast(float, pair8_24_t, { in.BC, in.A }))) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	cp	a, 080h
	jq	c, __fptrunc
.roundup:
	push	bc
	ex	(sp), hl
	adc	hl, hl
	ld	h, a
	rla
	jq	z, .mantissa.zero
	sub	a, 07Fh
	jq	c, .return.one
	sub	a, 24
	jq	nc, .return
	push	bc
	cpl
	ld	c, a
	ld	a, h
	sbc	hl, hl
	call	__ishl
	ex	(sp), hl
	pop	bc
	dec	hl
	call	__iand
	or	a, a
	sbc	hl, bc
	sbc	a, -1
	push	hl
	pop	bc
	ld	h, a
.return:
	ld	a, h
	pop	hl
	ret

.mantissa.zero:
	dec	a
	cp	a, 07Fh - 1
	jq	nc, .return
.return.one:
	ld	bc, 0800000h
	ld	a, h
	or	a, 07Fh shr 1
	pop	hl
	ret

_my_ceilf:
_my_ceil:
	ld	iy, 0
	add	iy, sp
	ld	bc, (iy + 6)
	ld	a, (iy + 9)

; IEEE single precision ceiling
; aubc = ceil(aubc)
__fpceil: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), ceilf(bitcast(float, pair8_24_t, { in.BC, in.A }))) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	rlca
	rrca
	jq	nc, __fpfloor.roundup

; IEEE single precision truncation
; aubc = trunc(aubc)
__fptrunc: ; CHECK: same(bitcast(float, pair8_24_t, { out.BC, out.A }), truncf(bitcast(float, pair8_24_t, { in.BC, in.A }))) && out.DE == in.DE && out.HL == in.HL && out.IX == in.IX && out.IY == in.IY
	push	bc
	ex	(sp), hl
	add	hl, hl
	ld	h, a
	rla
	sub	a, 07Fh
	jq	c, .return.zero
	sub	a, 24
	jq	nc, .return
	ld	l, c
	cpl
	ld	c, a
	ld	a, l
	push	hl
	sbc	hl, hl
	call	__ishl
	ld	c, a
	call	__iand
	ex	(sp), hl
	pop	bc
.return:
	ld	a, h
	pop	hl
	ret

.return.zero:
	ld	bc, 0
	ld	a, h
	and	a, 080h
	pop	hl
	ret

	extern	__ishl
	extern	__iand
	extern	__inot
