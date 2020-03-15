[BITS	32]

GLOBAL	io_hlt, io_cli, io_sti, io_in8, io_out8
GLOBAL	io_poll_eflags, io_push_eflags
GLOBAL	load_idt
GLOBAL	IDT
GLOBAL  reg_int_handler


[SECTION .data]
; 中断表数据
IDT:	
	TIMES	256 DQ 0x00
idtr:
	DW	$-IDT-1
	DD	IDT

int_map:
	DD	irq0, 0
	DD	irq1, 0
	DD	irq2, 0
	DD	irq3, 0
	DD	irq4, 0
	DD	irq5, 0
	DD	irq6, 0
	DD	irq7, 0
	DD	irq8, 0
	DD	irq9, 0
	DD	irq10, 0
	DD	irq11, 0
	DD	irq12, 0
	DD	irq13, 0
	DD	irq14, 0
	DD	irq15, 0

[SECTION .text]
io_hlt:
	HLT
	RET

io_cli:
	CLI
	RET

io_sti:
	STI
	RET

io_in8:	; int io_in8(int port)
	MOV	EDX, [ESP+4]
	MOV	EAX, 0
	IN	AL, DX
	RET

io_out8: ; void io_out8(int port, int data)
	MOV	EDX, [ESP+4] 	; port
	MOV	AL, [ESP+8]     ; data
	OUT	DX, AL
	RET
outb: ; void io_out8(int port, int data)
	MOV	EDX, [ESP+4] 	; port
	MOV	AL, [ESP+8]     ; data
	OUT	DX, AL
	

io_poll_eflags: ; int io_poll_eflags()
	PUSHFD
	POP	EAX
	RET

io_push_eflags:	; void io_push_eflags(int eflags)
	MOV	EAX, [ESP+4]
	PUSH	EAX
	POPFD
	RET

load_idt: ; void load_idt()
	lidt [idtr]
	sti
	ret

reg_int_handler: ; void reg_int_handle(int irn, void *func)
	MOV	EAX, [ESP + 4]
	SHL	EAX, 3

	MOV	ECX, EAX
	ADD	ECX, int_map; ecx int_map 中的offset
	MOV	EBX, [ESP + 8]
	MOV	[ECX+4], EBX

	MOV	EBX, [ECX] ;irn的地址

	ADD	EAX, IDT+256
	MOV	ECX, EBX
	AND	ECX, 0xffff
	MOV	[EAX], CX
	
	MOV	WORD [EAX+2], 0x8
	
	MOV	WORD [EAX+4], 0

	MOV	WORD [EAX+5], 0x8e
	
	MOV	ECX, EBX
	AND	ECX, 0xffff0000
	SHR	ECX, 16
	MOV	[EAX+6], CX
	RET

	
	
	

%macro irqn 1
	pusha
	call [int_map+(%1 * 8) + 4]
	popa
	iret
%endmacro
; 中断0
irq0: irqn 0
irq1: irqn 1
irq2: irqn 2
irq3: irqn 3
irq4: irqn 4
irq5: irqn 5
irq6: irqn 6
irq7: irqn 7
irq8: irqn 8
irq9: irqn 9
irq10: irqn 10
irq11: irqn 11
irq12: irqn 12
irq13: irqn 13
irq14: irqn 14
irq15: irqn 15

