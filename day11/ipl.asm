
; os 引导程序
; c语言内核开始地址
KERNEL  EQU 0x7e00
CYLS    EQU 0x0ff0
LEDS    EQU 0x0ff1
VMODE   EQU 0x0ff2
SCRNX   EQU 0x0ff4
SCRNY   EQU 0x0ff6
VRAM    EQU 0x0ff8

MEM_DEC_COUNT       EQU     0x1000
MEM_DEC             EQU     0x1002

; initial program loader
    ORG     0x7c00
    JMP     stage1
    DB      0x90
    DB      "LIGHTIPL"
    DW      512
    DB      1
    DW      1
    DB      2
    DW      224
    DW      2880
    DB      0xf0
    DW      9
    DW      18
    DW      2
    DD      0
    DD      2880
    DB      0, 0, 0x29
    DD      0xffffffff
    DB      "LIGHT-OS   "
    DB      "FAT12   "
    TIMES   18 DB 0x00

; 第一阶段加载
stage1:
; 读取软盘数据
.read_floppy:
    MOV     BX, KERNEL          ; ES:BX 缓冲区地址

    MOV     CH, 0               ; 柱面0
    MOV     DH, 0               ; 磁头0
    MOV     CL, 2               ; 扇区2

    MOV     AH, 0x02            ; 模式为读盘
    MOV     AL, 63              ; 连续读63个扇区
    MOV     DL, 0               ; A驱动器
    INT     0x13                ; 调用磁盘bios

; 第二阶段加载
stage2:
.init
; 初始化vga显卡
    MOV     AL, 0x13
    MOV     AH, 0x00
    INT     0x10
; 把vga显卡元数据存起来
;    MOV     BYTE [VMODE], 8
    MOV     WORD [SCRNX], 320
    MOV     WORD [SCRNY], 200
    MOV     DWORD [VRAM], 0x000a0000

; 获取键盘LED指示灯的状态
    MOV     AH, 0x02
    INT     0x16
    MOV     [LEDS], AL	

; 0xe820 detect memory
.detect_mem:
    MOV     DI, MEM_DEC
    XOR     EBX, EBX
    XOR     SI, SI
.loop
    MOV     EDX, 0x534D4150
    MOV     EAX, 0xE820
    MOV     ECX, 24
    INT     0x15

    CMP     EBX, 0
    JE      .fin

    INC     SI
    ADD     DI, 24
    JMP     .loop
.fin:
    MOV     byte [0x500], 0

; 进入保护模式
.enter_pm:
; 设置GLOBAL DESCRIPTOR TABLE
    CLI
    LGDT    [gdt_descriptor]
; 进入保护模式
    MOV     EAX, CR0
    OR      EAX, 0x1
    MOV     CR0, EAX
	
    JMP     0x8:.init_pm

[BITS 32]
.init_pm:
	MOV		AX, 0x10
	MOV		DS, AX
	MOV		SS, AX
	MOV		ES, AX
	MOV		FS, AX
	MOV		GS, AX

	CALL	KERNEL

[BITS 16]
; gdt 全局描述符表
gdt:
;null :
	DD		0
	DD		0
;code :
    DW		0xffff       ;Limit
    DW		0x0          ;Base
    DB 		0x0          ;Base
    DB	 	0b10011010   ;1st flag, Type flag
    DB	 	0b11001111   ;2nd flag, Limit
    DB	 	0x0          ;Base
;data :
    DW	 	0xffff
    DW 		0x0
    DB		0x0
    DB 		0b10010010
    DB	 	0b11001111
    DB 		0x0
gdt_descriptor:
    DW      $-gdt-1
    DD      gdt
	
    TIMES   510-($-$$) DB 0
    DW      0xaa55				; boot signature
