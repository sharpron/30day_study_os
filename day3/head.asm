ORG		0x8200

MOV     AL, 0x13            ; VGA显卡，320*200*8位彩色
MOV     AH, 0x00 
INT     0x10
CALL		0x8530
TIMES 	0x200-($-$$) DB 0

