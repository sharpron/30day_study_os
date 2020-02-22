
; initial program loader

	ORG		0x7c00

	JMP		init
	DB		0x90

; 标准FAT12格式软盘专用代码
	
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
	RESB    18

; 程序入口
	
init:
	MOV		AX, 0
    MOV		SS, AX
	MOV		SP, 0x7c00	
	MOV		DS, AX
	MOV		ES, AX
	
	MOV		SI, msg

show_msg:
	MOV		AL, [SI]
	INC		SI
	CMP		AL, 0
	JE	    fin	
	MOV		AH, 0x0e
	MOV		BX, 15
	INT		0x10
	JMP		show_msg

; 结束运行
fin:
	HLT
	JMP		fin

msg:
	DB		0x0a, 0x0a
	DB		"welcome play light os!"
	DB		0x0a
	DB		0
	
	TIMES	0x1fe-($-$$) DB	0x00
	DW 		0xAA55
