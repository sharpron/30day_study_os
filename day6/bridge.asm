[BITS	32]

EXTERN		main
GLOBAL		_start
GLOBAL		io_hlt
GLOBAL		io_cli
GLOBAL		io_sti
GLOBAL		io_in8
GLOBAL		io_out8
GLOBAL		io_poll_eflags
GLOBAL		io_push_eflags

[SECTION	.text]

_start:
	CALL		main
	RET

io_hlt:
	HLT
	RET

io_cli:
	CLI
	RET

io_sti:
	STI
	RET

io_in8:						; void io_in8(int port)
	MOV		EDX, [ESP+4]
	MOV		EAX, 0
	IN		AL, DX
	RET

io_out8:                    ; void io_out8(int port, int data)
	MOV		EDX, [ESP+4] 	; port
	MOV		AL, [ESP+8]     ; data
	OUT		DX, AL
	RET
	

io_poll_eflags:
	PUSHFD
	POP		EAX
	RET

io_push_eflags:			; void io_push_eflags(int eflags)
	MOV		EAX, [ESP+4]
	PUSH	EAX
	POPFD
	RET
