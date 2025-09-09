	assume	adl=1

;-------------------------------------------------------------------------------

	section	.data

	public	_prev_reg
_prev_reg:
	;       L H U  E D U  C B U  A  X I U  Y I U
	db	0,0,0, 0,0,0, 0,0,0, 0, 0,0,0, 0,0,0

	public	_next_reg
_next_reg:
	;       L H U  E D U  C B U  A  X I U  Y I U
	db	0,0,0, 0,0,0, 0,0,0, 0, 0,0,0, 0,0,0

;-------------------------------------------------------------------------------

	section	.text

;-------------------------------------------------------------------------------

	public	_CRT_babs_crt
_CRT_babs_crt:
	ld	hl, 3
	add	hl, sp
	ld	a, (hl)
	call	_set_prev_reg
	call	__babs_crt
	jq	_set_next_reg

	public	_CRT_sabs_crt
_CRT_sabs_crt:
	ld	hl, 3
	add	hl, sp
	ld	hl, (hl)
	call	_set_prev_reg
	call	__sabs_crt
	jq	_set_next_reg

	public	_CRT_iabs_crt
_CRT_iabs_crt:
	ld	hl, 3
	add	hl, sp
	ld	hl, (hl)
	call	_set_prev_reg
	call	__iabs_crt
	jq	_set_next_reg

	public	_CRT_labs_crt
_CRT_labs_crt:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 3)
	ld	e, (iy + 6)
	call	_set_prev_reg
	call	__labs_crt
	jq	_set_next_reg

	public	_CRT_i48abs_crt
_CRT_i48abs_crt:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 3)
	ld	de, (iy + 6)
	call	_set_prev_reg
	call	__i48abs_crt
	jq	_set_next_reg

	public	_CRT_llabs_crt
_CRT_llabs_crt:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 3)
	ld	de, (iy + 6)
	ld	bc, (iy + 9)
	call	_set_prev_reg
	call	__llabs_crt
	jq	_set_next_reg

;-------------------------------------------------------------------------------

	private	_set_prev_reg
_set_prev_reg:
	ld	(_prev_reg +  0), hl
	ld	(_prev_reg +  3), de
	ld	(_prev_reg +  6), bc
	ld	(_prev_reg +  9), a
	; ld	(_prev_reg + 10), iy
	; ld	(_prev_reg + 13), ix
	ret

	private	_set_next_reg
_set_next_reg:
	ld	(_next_reg +  0), hl
	ld	(_next_reg +  3), de
	ld	(_next_reg +  6), bc
	ld	(_next_reg +  9), a
	; ld	(_next_reg + 10), iy
	; ld	(_next_reg + 13), ix
	ret

;-------------------------------------------------------------------------------

	public	_CRT_sabs_crt_fast
_CRT_sabs_crt_fast:
	ld	hl, 3
	add	hl, sp
	ld	hl, (hl)
	jp	__sabs_crt_fast

	public	_CRT_iabs_crt_fast
_CRT_iabs_crt_fast:
	ld	hl, 3
	add	hl, sp
	ld	hl, (hl)
	jp	__iabs_crt_fast

	public	_CRT_labs_crt_fast
_CRT_labs_crt_fast:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 3)
	ld	e, (iy + 6)
	jp	__labs_crt_fast

; 	public	_CRT_i48abs_crt_fast
; _CRT_i48abs_crt_fast:
; 	ld	iy, 0
; 	add	iy, sp
; 	ld	hl, (iy + 3)
; 	ld	de, (iy + 6)
; 	jp	__i48abs_crt_fast

	public	_CRT_llabs_crt_fast
_CRT_llabs_crt_fast:
	ld	iy, 0
	add	iy, sp
	ld	hl, (iy + 3)
	ld	de, (iy + 6)
	ld	bc, (iy + 9)
	jp	__llabs_crt_fast

;-------------------------------------------------------------------------------

	extern	__babs_crt
	extern	__sabs_crt
	extern	__iabs_crt
	extern	__labs_crt
	extern	__i48abs_crt
	extern	__llabs_crt
	extern	__sabs_crt_fast
	extern	__iabs_crt_fast
	extern	__labs_crt_fast
	; extern	__i48abs_crt_fast
	extern	__llabs_crt_fast

