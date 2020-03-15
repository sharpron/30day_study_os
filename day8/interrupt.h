#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

extern int io_in8(int port);

struct KEY_BUF {
	unsigned char data, flag
};

struct KEY_BUF keybuf;


void init_pic();
