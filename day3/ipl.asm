
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
	MOV		ES, AX
	

; 结束运行
fin:
	HLT
	JMP		fin
