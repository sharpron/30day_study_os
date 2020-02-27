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

		MOV		BYTE [VMODE], 8
		MOV		WORD [SCRNX], 320
		MOV		WORD [SCRNY], 200
		MOV		DWORD [VRAM], 0x000a0000

; 获取键盘LED指示灯的状态

		MOV		AH, 0x02
		INT		0x16							; 调用键盘bios
;		MOV		[LEDS], AL

; boot info

	
; 设置GLOBAL DESCRIPTOR TABLE

		CLI
		LGDT	[gdt_descriptor]

; 设置INTERUPT DESCRIPTOR TABLE
		LIDT	[IDT]
	
; 进入保护模式
	
		MOV		EAX, CR0
		OR		EAX, 0x1
		MOV		CR0, EAX
		MOV		AX, 0x8
		MOV		ES, AX
		JMP		0x8:init_pm

[BITS	32]
init_pm:
		MOV		AX, 0x00
		MOV		ES, AX
		JMP		entry

[BITS	16]
GDT:

; null	: 							; 处理器规定GDT的开始必须是空描述符
		DD		0x0
		DD		0x0
; code	:
		DW		0xffff				; 段界限
		DW		0x0000				; 段基地址低16位

		DB		0x0					; 段基地址高8位
		DB		0b10011010			; 第8位P(存在位，内存紧张时把很少用的段拷贝到磁
									; 第7-6位DPL(descriptor privilege level最高是0，
									; 最低是3，不同特权级的程序相互隔离; 第5位S,0表
									; 示系统段，1标识数据段或代码段;低四位是TYPE(描
									; 述符子类型

		
		DB		0b11001111			; 高4位介绍: G(粒度单位) D/B(默认操作数大小)
									;  L(64bit使用) AVL(软件用), 低四位标识段界限高4位

		DB		0x0					; 段基地址最高8位
; data	:
		DW		0xffff				; 段界限
		DW		0x0000				; 段基地址低16位

		DB		0x0					; 段基地址高8位
		DB		0b10010010			; 0x92 固定, 系统专用，可读写不可执行
		DB		0b11001111
		DB		0x0	
gdt_descriptor:
		DW		$-GDT-1				; 段上限 16bit
		DD		GDT					; gdt 地址 32bit

IDT:
		DW		0x0
		DW		0x0
		DB		0x0
		DB		0x0
		DW		0x0

idt_descriptor:
		DW		$-IDT-1
		DD		IDT

TIMES	512-($-$$) DB 0x0
; 入口
entry:
