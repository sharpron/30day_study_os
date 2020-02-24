; C语言转换入口

	ORG		0x8200

	MOV		BP, 0x9000
	MOV		SP, BP

	CLI
	LGDT	[gdt_descriptor]
; 进入保护模式
	
	MOV		EAX, CR0
	OR		EAX, 0x1
	MOV		CR0, EAX
	JMP		0x8:init_pm

[BITS	32]
init_pm:
	MOV		AX, 0x10
	MOV		DS, AX
	MOV		SS, AX
	MOV		ES, AX
	MOV		FS, AX
	MOV		GS, AX
	CALL	entry

[BITS	16]
GDT:
; null	:
	DD		0x00
	DD		0x00
; code	:
	DW		0xFFFF
	DW		0
	DB		0
	DB		0b10011010
	DB		0b11001111
	DB		0	
; data	:
	DW		0xFFFF
	DW		0
	DB		0
	DB		0b10010010
	DB		0b11001111
	DB		0	
gdt_descriptor:
	DW		$-GDT-1
	DD		GDT
entry:
