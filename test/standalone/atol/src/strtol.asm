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
	; or	a, a
	; jr	z, .auto_base
	; cp	a, 16
	; jr	z, .hex_base
	; cp	a, 2
	; jr	z, .bin_base
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
	jr	nz, .other_base	; hexadecimal
	ld	b, 2
	jr	.save_new_base

.maybe_hex:
	bit	1, b
	jr	nz, .other_base	; binary
	ld	b, 16
	jr	.save_new_base

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
; 	ld	a, (ix + 9)	; base
	ld	(ix - 1), 10
; 	ld	(ix - 2), 0
; 	cp	a, 11
; 	jr	nc, .has_letters
; 	ld	(ix - 1), a
; 	jr	.no_letters
; .has_letters:
; 	sub	a, 10
; 	ld	(ix - 2), a
; .no_letters:
	xor	a, a
	ld	hl, 0
	ld	de, 0
	ld	bc, 0
	jr	.start
.loop_add_10:
	add	a, 10
.loop:
	ld	(ix - 3), a	; digit
	ld	a, (ix + 12)	; base
; __lmulu_b_fast
	dec	sp
	push	hl
	inc	sp
	pop	bc	; B = UHL, C = H
	ld	c, a
	mlt	bc	; BC = A * U
	ld	d, a
	push	de	; A * E
	ld	d, l
	ld	e, a
	ld	l, a
	mlt	hl	; HL = A * H
	ld	a, h
	add	a, c
	ld	h, a	; A = H
	adc	a, b	; A = H + AU.hi + carry
	sub	a, h	; A = AU.hi + carry
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	mlt	de	; DE = A * L
	; add digit
	ld	a, (ix - 3)
	add	a, e
	ld	e, a
	adc	a, d
	sub	a, e
	ld	d, a
	; continue
	add	hl, de	; UHL = AH.hi + AU.lo, AH.lo + AL.hi, AL.lo
	pop	de
	mlt	de	; DE = A * E
	adc	a, e	; AU.hi + AE.lo + Carry
	ld	e, a
; overflow test
	adc	a, d
	sub	a, e
	jr	nz, .overflow
; next digit
	inc	iy
.start:
	ld	a, (iy)
	; sub	a, 48
	; cp	a, 10
	; jr	c, .loop
	sub	a, $30
	cp	a, (ix - 1)
	jr	c, .loop
	; sub	a, $11
	; cp	a, (ix - 2)
	; jr	c, .loop_add_10
	; sub	a, $20
	; cp	a, (ix - 2)
	; jr	c, .loop_add_10
.finish:
	pop	af
	ld	sp, ix
	pop	ix
	ret	nz
	jp	__lneg

.overflow:
	ld	e, $7F
	ld	hl, $113355
	jr	.finish

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
	ld	hl, $123456
	jr	.finish

_str_toupper:
	ld	a, c
	sub	a, 'a'
	cp	a, 1+'z'-'a'
	ret	nc
	res	5, c
	ret

	extern	__frameset
	extern	_errno
	extern	__lneg
	extern	__lmulu_b
