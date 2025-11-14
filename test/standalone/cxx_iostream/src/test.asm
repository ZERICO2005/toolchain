	assume	adl=1

	section	.text

	public	_init_foo

	public	___dtor_foo
___dtor_foo:
	ret

_init_foo:
	push	hl
	call	__ZNSt3__18ios_base4InitC2Ev
	pop	hl
	ld	hl, ___dtor_foo
	push	hl
	call	_atexit
	pop	hl
	ret

	extern	_atexit
	extern	__ZNSt3__18ios_base4InitC2Ev
