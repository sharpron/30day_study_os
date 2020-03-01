CYLS    EQU 0x0ff0
LEDS    EQU 0x0ff1
VMODE   EQU 0x0ff2
SCRNX   EQU 0x0ff4
SCRNY   EQU 0x0ff6
VRAM    EQU 0x0ff8

    ORG     0x7e00
; 初始化vga显卡
    MOV     AL, 0x13
    MOV     AH, 0x00
    INT     0x10
; 把vga显卡元数据存起来
    MOV     BYTE [VMODE], 8
    MOV     WORD [SCRNX], 320
    MOV     WORD [SCRNY], 200
    MOV     DWORD [VRAM], 0x000a0000
; 获取键盘LED指示灯的状态
    MOV     AH, 0x02
    INT     0x16
    MOV     [LEDS], AL
; 设置GLOBAL DESCRIPTOR TABLE
    CLI
    LGDT    [gdt_descriptor]
; 进入保护模式
    MOV     EAX, CR0
    OR      EAX, 0x1
    MOV     CR0, EAX
    JMP     0x8: boot
; gdt 全局描述符表
GDT:
;null : 
    dd 0x0 
    dd 0x0

;code : 
    dw 0xffff       ;Limit
    dw 0x0          ;Base
    db 0x0          ;Base
    db 0b10011010   ;1st flag, Type flag
    db 0b11001111   ;2nd flag, Limit
    db 0x0          ;Base

;data : 
    dw 0xffff       
    dw 0x0          
    db 0x0
    db 0b10010010 
    db 0b11001111 
    db 0x0
gdt_descriptor:
    DW      $-GDT-1
    DD      GDT
    TIMES   512-($-$$) DB   0x00

boot:
