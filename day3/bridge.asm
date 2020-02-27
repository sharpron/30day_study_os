[BITS	32]					; 使用32位模式


EXTERN		main
GLOBAL		_start
GLOBAL		io_hlt

[SECTION	.text]

; 入口
_start:
	mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	CALL    main			; 调用c语言入口函数
	RET

; 让cpu休息的函数
io_hlt:	; void io_hlt(void);	
	HLT
	RET
