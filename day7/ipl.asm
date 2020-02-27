; 读取柱面数量

C_	EQU		10

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
	TIMES	18 DB 0x00

; 程序入口
	
init:
	MOV		AX, 0
    MOV		SS, AX
	MOV		SP, 0x7c00	
	MOV		DS, AX
	MOV		ES, AX

; 读取软盘数据
read_floppy:
	MOV		AX, 0x0820
	MOV		ES, AX
	MOV		BX, 0
	
	MOV		CH, 0				; 柱面0
	MOV		DH, 0				; 磁头0
	MOV		CL, 2				; 扇区2

read_loop:
	MOV		SI, 0
	
read_retry:
	
	MOV		AH, 0x02			; 模式为读盘
	MOV		AL,	1				; 连续读1个扇区
	MOV		DL, 0				; A驱动器
	INT		0x13				; 调用磁盘bios
	JNC		read_next
	
	INC		SI
	CMP		SI, 5
	JA		error
	MOV		AH, 0
	MOV		DL, 0
	INT		0x13
	
	JMP		read_retry

read_next:
	MOV  	AX, ES
	ADD		AX, 32
	MOV		ES, AX				; 将缓存区位置移动512个字节
	
	INC		CL					; 读下一个扇区
	CMP		CL,	18				; 比较扇区号是否大于18
	JBE		read_loop			
	
	MOV		CL, 1				; 重置扇区，磁头号进位
	INC		DH					; 读下一个磁头
	CMP		DH, 2				; 共有两个磁头
	JB		read_loop
	
	MOV		DH, 0				; 重置磁头号，柱面进位
	INC		CH					; 读下一个柱面
	CMP		CH, C_				
	JB		read_loop
			
    JMP 	0x8200				; 跳转到系统执行	
	
	
; 显示错误消息
error:
	MOV		SI, msg_error

putloop:
	MOV		AL, [SI]
	ADD		SI, 1
	CMP		AL, 0
	JE	    fin	
	MOV		AH, 0x0e
	MOV		BX, 15
	INT		0x10
	JMP		putloop

; 结束运行
fin:
	HLT
	JMP		fin

msg_error:
	DB		"read disk error"
	DB		0x00
	DB		0x00
	DB 		0x00
	
	TIMES	0x1fe-($-$$) DB	0x00
	DW 		0xAA55
