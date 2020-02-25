; 头信息

CYLS	EQU		0x0ff0
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2
SCRNX	EQU		0x0ff4
SCRNY	EQU		0x0ff6
VRAM	EQU		0x0ff8

		ORG		0x8200

; 初始化vga显卡

		MOV		AL, 0x13
		MOV		AH, 0x00
		INT		0x10	

; 把vga显卡元数据存起来

;		MOV		BYTE [VMODE], 8
;		MOV		WORD [SCRNX], 320
;		MOV		WORD [SCRNY], 200
;		MOV		DWORD [VRAM], 0x000a0000

; 获取键盘LED指示灯的状态

		MOV		AH, 0x02
		INT		0x16							; 调用键盘bios
;		MOV		[LEDS], AL

; boot info

	
; 设置GLOBAL DESCRIPTOR TABLE

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
		JMP		entry

[BITS	16]
GDT:
; null	:
		DD		0x0
		DD		0x0
; code	:
		DW		0xFFFF
		DW		0x0
		DB		0x0
		DB		0b10011010
		DB		0b11001111
		DB		0x0	
; data	:
		DW		0xFFFF
		DW		0x0
		DB		0x0
		DB		0b10010010
		DB		0b11001111
		DB		0x0	
gdt_descriptor:
		DW		$-GDT-1
		DD		GDT

TIMES	512-($-$$) DB 0x0
; 入口
entry:
