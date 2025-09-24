	assume	adl=1

	section	.text

	public	_my_strtol

_my_strtol:
	ld	hl, -3
	call	__frameset
	ld	hl, (ix + 12)	; base
	ld	b, l		; so we don't have to load base again
	ld	de, -37
	add	hl, de
	ld	c, e		; ld c, -1 (indicates that no prefixes have been found)
	jp	c, .invalid_base
	ld	hl, (ix + 6)	; nptr
;-------------------------------------------------------------------------------
; consume whitespace (inlined isspace)
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
	xor	a, '-' - 14
	push	af		; Z = negative, NZ = positive, carry cleared
	jr	z, .minus_sign
	xor	a, ('+' - 14) xor ('-' - 14)
	jr	z, .plus_sign
	dec	hl
	xor	a, a
.plus_sign:
.minus_sign:
	; A = 0, (HL) = start of number
;-------------------------------------------------------------------------------
; update the base if needed
	or	a, b		; base
	jr	z, .auto_base
	cp	a, 16
	jr	z, .hex_base
	cp	a, 2
	jr	nz, .other_base
.auto_base:	; test for 0* 0x* 0X* 0b* 0B*
.bin_base:	; test for 0x* 0X*
.hex_base:	; test for 0b* 0B*
	inc	b		; djnz hax
	ld	a, (hl)
	cp	a, '0'
	jr	nz, .maybe_decimal
	inc	hl
	ld	a, (hl)
	res	5, a		; upper case
	cp	a, 'X'
	jr	z, .maybe_hex
	cp	a, 'B'
	jr	z, .maybe_bin
	dec	hl
	djnz	.other_base
	ld	b, 8		; octal
	jr	.save_new_base

.maybe_bin:
	bit	4, b
	jr	nz, .undo_inc	; hexadecimal
	; base is 0 or 2
	inc	hl
	ld	b, 2
	ld	c, b
	jr	.save_new_base

.maybe_hex:
	bit	1, b
	jr	nz, .undo_inc	; binary
	; base is 0 or 16
	inc	hl
	ld	b, 16
	ld	c, b
	jr	.save_new_base

.undo_inc:
	dec	hl
	; dec	b
	; jr	.other_base
.maybe_decimal:
	; set to decimal if base is not zero
	djnz	.other_base
	ld	b, 10		; decimal
.save_new_base:
;-------------------------------------------------------------------------------
.other_base:
	ld	a, (hl)		; first digit of the number
	push	hl
	pop	iy
	or	a, a
	sbc	hl, hl
	ld	e, l
	ld	d, b
	inc.s	bc
	ld	b, l
	; A = first digit of the number
	; E:UHL = 0
	; D = base
	; UBC = 0
	; B = 0
	; C is -1 if a "0x", "0X", "0b", or "0B" prefix was not detected

	; The strto* functions return nptr (not nptr + whitespace) if there are
	; no digits in the string. Having a digit check here allows us to
	; directly handle the case where the string has no digits.

	sub	a, 48
	cp	a, 10
	jr	c, .check_digit
	; Convert an alphabetic digit, case-insensitive
	sub	a, 65 - 48
	res	5, a
	add	a, 10
.check_digit:
	; End the loop when the digit is out of range for the base
	cp	a, d
	jr	c, .loop
	; no digit found
	inc	c
	jr	z, .no_number
	; no digits found after a prefix
	dec	iy
	jr	.end_loop
;-------------------------------------------------------------------------------
.check_decimal:
	cp	a, d
	jr	nc, .end_loop
.loop:
	ld	c, a		; UBC = digit if no carry, don't care otherwise
	ld	a, d		; A = base
	ld	d, e		; D = upper accumulator byte
	ld	e, b		; E = 0 if no carry, don't care otherwise
	; E:UHL = UHL * base
	call	__lmulu_b
	; Add digit to lower product bytes
	add	hl, bc
	ld	c, a		; C = base
	ld	a, e		; A = upper product byte
	ld	e, c		; E = base
	mlt	de		; DE = upper accumulator byte * base
	; Carry into upper product byte
	adc	a, e
	ld	e, a
	; Set B != 0 if any carry from the upper byte or previous iterations
	sbc	a, a
	or	a, d
	or	a, b
	ld	b, a
	ld	d, c		; D = base
	; IY = str, D = base, E:UHL = accumulator, BCU = 0, B = 0 if no carry
; next digit
	inc	iy
.start:
	; Convert a numerical digit
	ld	a, (iy)
	sub	a, 48
	cp	a, 10
	jr	c, .check_decimal
	; Convert an alphabetic digit, case-insensitive
	sub	a, 65 - 48
	res	5, a
	add	a, 10
	; End the loop when the digit is out of range for the base
	cp	a, d
	jr	c, .loop
.end_loop:
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
	; overflow occured if B is non-zero
	inc	b
	djnz	.out_of_range
	bit	7, e
	jr	nz, .maybe_out_of_range
;-------------------------------------------------------------------------------
.finish_pop_af:
	pop	af		; Z = negative, NZ = positive, carry cleared
.exact_int_min:
.finish:
	ld	sp, ix
	pop	ix
	ret	nz
	jp	__lneg

.no_number:
.invalid_base:
	xor	a, a
	sbc	hl, hl
	ex	de, hl
	ld	hl, (ix + 9)	; endptr
	adc	hl, de
	jr	z, .invalid_base_endptr_null
	ld	iy, (ix + 6)	; nptr
	ld	(hl), iy
.invalid_base_endptr_null:
	ex	de, hl
	ld	e, a
	; E:UHL = 0
	jr	.finish

.maybe_out_of_range:
	pop	af		; Z = negative, NZ = positive, carry cleared
	; greater than INT_MAX
	jr	nz, .overflow
	; negative
	; check that the result is not an exact INT_MIN
	adc	hl, hl
	jr	nz, .is_actually_underflow
	ld	a, e
	adc	a, a
	jr	z, .exact_int_min
.is_actually_underflow:
	xor	a, a		; set Z
	jr	.underflow

.out_of_range:
	pop	af		; Z = negative, NZ = positive, carry cleared
.overflow:
.underflow:
	ld	e, $80
	ld	hl, 5		; ERANGE
	ld	(_errno), hl
	ld	l, h		; ld hl, 0
	jr	z, .finish	; underflow or invalid base
	dec	hl
	dec	e		; sets NZ
	; overflow
	jr	.finish

	extern	__frameset
	extern	_errno
	extern	__lneg
	extern	__lmulu_b
