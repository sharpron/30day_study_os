[BITS	32]

EXTERN		main
GLOBAL		_start
GLOBAL		io_hlt

[SECTION	.text]

_start:
	CALL		main

io_hlt:
	HLT
	RET

