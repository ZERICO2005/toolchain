	assume	adl=1

	section	.data

	public	_reg
_reg:
	;  A  L H U  E D U   C B U
	db 0, 0,0,0, 0,0,0,  0,0,0
	;  A  L H U  E D U   C B U
	db 0, 0,0,0, 0,0,0,  0,0,0
	;  A  L H U  E D U   C B U
	db 0, 0,0,0, 0,0,0,  0,0,0

	section	.text

_output_reg_0:
	ld	(_reg + 0 + (10 * 0)), a
	ld	(_reg + 1 + (10 * 0)), hl
	ld	(_reg + 4 + (10 * 0)), de
	ld	(_reg + 7 + (10 * 0)), bc
	ret
_output_reg_1:
	ld	(_reg + 0 + (10 * 1)), a
	ld	(_reg + 1 + (10 * 1)), hl
	ld	(_reg + 4 + (10 * 1)), de
	ld	(_reg + 7 + (10 * 1)), bc
	ret
_output_reg_2:
	ld	(_reg + 0 + (10 * 2)), a
	ld	(_reg + 1 + (10 * 2)), hl
	ld	(_reg + 4 + (10 * 2)), de
	ld	(_reg + 7 + (10 * 2)), bc
	ret

	public	_my_ldexpf

	private	__ldexpf_helper
__ldexpf_helper:
.ret_self:
	ld	hl, (iy + 3)
.ret_zero:
	ld	e, (iy + 6)
	ret

.maybe_subnormal:
	dec	bc	; BC is now -1
	add	hl, bc
	ld	hl, (iy + 3)	; mant
	jr	nc, .ret_zero	; return zero
; .subnormal_input:
	; BC is -1 here
	bit	7, (iy + 11)	; scale sign
	jr	nz, .move_subnormal_down
	; jr	z, .move_subnormal_up
.move_subnormal_up:
	ld	de, (iy + 9)	; scale
.norm_loop:
	add	hl, hl
	jr	c, .normalized
	ex	de, hl
	add	hl, bc	; --scale
	ex	de, hl
	jr	c, .norm_loop
; .still_subnormal:
	; DE is -1 here
	inc	e	; ld e, 0
	jr	_my_ldexpf.finish_subnormal
.normalized:
	inc	de
	ex	de, hl
	jr	_my_ldexpf.scale_up

.move_subnormal_down:
	; BC is -1 here
	; first we need to test that the result won't be zero
	; ld	hl, (iy + 3)	; mant
	call	__ictlz
	; A is [1, 23]
	; return zero if (scale < clz_result - 24) or (clz_result - 25 >= scale)
	; return zero if (scale < clz_result - 24)
	sub	a, 24	; A is [-23, -1]
	ld	c, a	; sign extend A
	ld	hl, (iy + 9)	; scale
	ld	a, l
	or	a, a
	sbc	hl, bc
	cpl
	jr	nc, _my_ldexpf.shru_common	; tests jr c, .underflow_to_zero
.underflow_to_zero:
	xor	a, a
	ld	b, a	; ld b, 0
.overflow_to_inf:	; <-- Carry is set when inf/NaN
	ld	hl, 5	; ERANGE
	ld	(_errno), hl
	ld	l, h	; ld l, 0
	ex	de, hl
	jr	nc, _my_ldexpf.zero_hijack
	ld	de, $800000
	ld	b, $7F
	jr	_my_ldexpf.inf_nan_hijack

;-------------------------------------------------------------------------------
_my_ldexpf:
	ld	iy, 0
	lea	bc, iy + 0
	add	iy, sp
	ld	hl, (iy + 3)	; mant
	add	hl, hl
	ld	a, (iy + 6)	; expon
	adc	a, a
	jr	z, __ldexpf_helper.maybe_subnormal
	ld	c, a
	inc	a
	jr	z, __ldexpf_helper.ret_self	; inf NaN
	ex	de, hl
	ld	hl, (iy + 9)	; scale
	add	hl, bc	; add expon
	bit	7, (iy + 11)	; scale sign
	jr	nz, .scale_down
.scale_up:
	ld	bc, -255	; $FFFF01
	add	hl, bc
	jr	c, __ldexpf_helper.overflow_to_inf
	; sbc	hl, bc	; restore hl
	dec	l	; we only care about the low 8 bits
	ex	de, hl
.finish_subnormal:
	push	hl
.finish:
	ld	a, (iy + 6)	; expon
	rla
	rr	e
	rr	(iy - 1)
	pop	hl
	rr	h
	rr	l
	ret

;-------------------------------------------------------------------------------
.scale_down:
	push	de	; mant <<= 1
	ld	e, l	; shift amount
	dec	hl
	add	hl, hl
	jr	nc, .finish	; expon > 0
;-------------------------------------------------------------------------------
.shru_to_subnormal:
	xor	a, a
	sub	a, e
	pop	de
	ld	c, 48	; 24 << 1
	add	hl, bc

	set	7, (iy + 5)	; set implicit mantissa bit
	jr	nc, __ldexpf_helper.underflow_to_zero
.shru_common:
	; A should be [0, 23]
	ld	b, a
	ld	hl, (iy + 3)	; mantissa
	push	hl	; ld (iy - 3), hl
	xor	a, a
	inc	b
	ld	c, a	; ld c, 0
	ld	d, (iy - 1)
.shru_loop:
	adc	a, c	; collect sticky bits
	srl	d
	rr	h
	rr	l
	djnz	.shru_loop
	ld	(iy - 1), d
	pop	de
	ld	d, h
	ld	e, l
	
	; round upwards to even if (round && (guard || sticky))
	jr	nc, .no_round
	inc	a
	dec	a
	jr	nz, .round_up
	bit	0, e	; test guard bit
	jr	z, .no_round
.round_up:
	inc	de	; round upwards to even (wont overflow)
.no_round:
	adc	a, a
	jr	z, .result_is_exact
.zero_hijack:
.inf_nan_hijack:
	ld	hl, ___fe_cur_env
	set	5, (hl)	; FE_INEXACT
.result_is_exact:
	ld	a, (iy + 6)	; get signbit
	ex	de, hl
	and	a, $80	; copysign
	or	a, b	; used for the ret infinite path
	ld	e, a
	ret

;-------------------------------------------------------------------------------

	extern	___fe_cur_env
	extern	_errno
	extern	__ictlz
