	assume	adl=1

	section	.text

	public	_my_strtol

_my_strtol:
	ld	hl, -3
	call	__frameset
	ld	hl, (ix + 12)	; base
	ld	de, -37
	add	hl, de
	jp	c, .invalid_base
	ld	hl, (ix + 6)	; nptr
;-------------------------------------------------------------------------------
; consume whitespace
.whitespace_loop:
	ld	a, (hl)
	inc	hl
	cp	a, 32
	jr	z, .whitespace_loop
	sub	a, 9
	add	a, -5
	jr	nc, .whitespace_loop
; test for plus/minus signs
	; A = (HL - 1) - 9 + -5
	; A = (HL - 1) - 14
	sub	a, '-' - 14
	push	af
	jr	z, .minus_sign
	; A = (HL - 1) - 14 - ('-' - 14)
	; A = (HL - 1) - '-'
	xor	a, '+' - '-'
	jr	z, .plus_sign
	dec	hl
.plus_sign:
.minus_sign:
;-------------------------------------------------------------------------------
	ld	a, (ix + 12)	; base
	or	a, a
	jr	z, .auto_base
	cp	a, 16
	jr	z, .hex_base
	cp	a, 2
	jr	z, .bin_base
	jr	.other_base
.auto_base:
.bin_base:
.hex_base:
	ld	b, a
	ld	a, (hl)
	cp	a, '0'
	jr	nz, .maybe_decimal
	inc	hl
	ld	a, (hl)
	cp	a, 'x'
	jr	z, .maybe_hex
	cp	a, 'X'
	jr	z, .maybe_hex
	cp	a, 'b'
	jr	z, .maybe_bin
	cp	a, 'B'
	jr	z, .maybe_bin
	dec	hl
	inc	b
	djnz	.other_base
	ld	b, 8	; octal
	jr	.save_new_base

.maybe_bin:
	bit	4, b
	jr	nz, .undo_inc	; hexadecimal
	inc	hl
	ld	b, 2
	jr	.save_new_base

.maybe_hex:
	bit	1, b
	jr	nz, .undo_inc	; binary
	inc	hl
	ld	b, 16
	jr	.save_new_base

.undo_inc:
	dec	hl
	jr	.other_base

.maybe_decimal:
	inc	b
	djnz	.other_base
	ld	b, 10	; decimal
.decimal_base:
.save_new_base:
	ld	(ix + 12), b	; save new base
;-------------------------------------------------------------------------------
.other_base:
	push	hl
	pop	iy
	ld	(ix - 1), 10
	ld	(ix - 2), 0
	ld	a, (ix + 12)	; base
	cp	a, 11
	jr	nc, .has_letters
	ld	(ix - 1), a
	jr	.no_letters
.has_letters:
	sub	a, 10
	ld	(ix - 2), a
.no_letters:
	xor	a, a
	ld	hl, 0
	ld	de, 0
	ld	bc, 0
	ld	d, (ix + 12)	; base
	; jr	.start
	ld	a, (iy)
	sub	a, $30
	cp	a, (ix - 1)
	jr	c, .loop
	sub	a, $11
	cp	a, (ix - 2)
	jr	c, .loop_add_10
	sub	a, $20
	cp	a, (ix - 2)
	jr	c, .loop_add_10
	jr	.no_number

.loop_add_10:
	add	a, 10
.loop:
	ld	c, a	; UBC = digit if no carry, don't care otherwise
	ld	a, d	; A = base
	ld	d, e	; D = upper accumulator byte
	ld	e, b	; E = 0 if no carry, don't care otherwise
	; E:UHL = UHL * base
	call	__lmulu_b
	; Add digit to lower product bytes
	add	hl, bc
	ld	c, a	; C = base
	ld	a, e	; A = upper product byte
	ld	e, c	; E = base
	mlt	de	; DE = upper accumulator byte * base
	; Carry into upper product byte
	adc	a, e
	ld	e, a
	; Set B != 0 if any carry from the upper byte or previous iterations
	sbc	a, a
	or	a, d
	or	a, b
	ld	b, a
	ld	d, c	; D = base
	; IY = str, D = base, E:UHL = accumulator, BCU = 0, B = 0 if no carry
; next digit
	inc	iy
.start:
	ld	a, (iy)
	sub	a, $30
	cp	a, (ix - 1)
	jr	c, .loop
	sub	a, $11
	cp	a, (ix - 2)
	jr	c, .loop_add_10
	sub	a, $20
	cp	a, (ix - 2)
	jr	c, .loop_add_10
;-------------------------------------------------------------------------------
.write_endptr:
	push	hl
	ld	hl, (ix + 9)	; endptr
	add	hl, de
	or	a, a
	sbc	hl, de
	jr	z, .endptr_null
	ld	(hl), iy
.endptr_null:
	pop	hl
;-------------------------------------------------------------------------------
.overflow_testing:
	inc	b
	dec	b
	jr	nz, .out_of_range
	bit	7, e
	jr	nz, .maybe_overflow
;-------------------------------------------------------------------------------
.finish:
	pop	af
.was_int_min:
.finish_out_of_range:
	ld	sp, ix
	pop	ix
	ret	nz
	jp	__lneg

.no_number:
.invalid_base:
	ld	hl, (ix + 9)	; endptr
	add	hl, de
	or	a, a
	sbc	hl, de
	jr	z, .invalid_base_endptr_null
	ld	de, (ix + 6)	; nptr
	ld	(hl), de
.invalid_base_endptr_null:
	ld	hl, 5	; ERANGE
	ld	(_errno), hl
	ld	l, h
	ld	e, h
	jr	.finish

.maybe_overflow:
	pop	af
	; greater than INT_MAX
	jr	nz, .overflow
	; negative
	; check that the result is not an exact INT_MIN
	add	hl, de
	or	a, a
	sbc	hl, de
	jr	nz, .is_actually_underflow
	ld	a, e
	cp	a, $80
	jr	nz, .is_actually_underflow
	jr	.was_int_min

.is_actually_underflow:
	ld	hl, 5	; ERANGE
	ld	(_errno), hl
	ld	e, $80
	ld	hl, $000000
	jr	.finish_out_of_range

.out_of_range:
	pop	af
.overflow:
	ld	hl, 5	; ERANGE
	ld	(_errno), hl
	ld	e, $7F
	ld	hl, $FFFFFF
	jr	nz, .finish_out_of_range
	inc	hl
	inc	e
	jr	.finish_out_of_range

	extern	__frameset
	extern	_errno
	extern	__lneg
	extern	__lmulu_b
