
; Bios 引导程序
START     EQU 0x7e00

; initial program loader
	ORG		0x7c00
	JMP		read_floppy
	DB		0x90
	DB		"LIGHTIPL"
	DW		512
	DB		1
	DW		1
	DB		2
    DW		224	
	DW		2880
	DB		0xf0
	DW		9	
	DW		18
	DW		2
	DD		0
	DD		2880
	DB		0, 0, 0x29
	DD		0xffffffff
	DB		"LIGHT-OS   "
	DB		"FAT12   "
	TIMES	18 DB 0x00

; 读取软盘数据
read_floppy:
	MOV		BX, START          ; ES:BX 缓冲区地址
	
	MOV		CH, 0				; 柱面0
	MOV		DH, 0				; 磁头0
	MOV		CL, 2				; 扇区2
	
	MOV		AH, 0x02			; 模式为读盘
	MOV		AL,	63				; 连续读63个扇区
	MOV		DL, 0				; A驱动器
	INT		0x13				; 调用磁盘bios
    
    JMP     START
    JMP     read_floppy
	
    TIMES   510-($-$$) DB 0
    DW      0xaa55
