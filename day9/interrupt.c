#include "interrupt.h"
#include "scancode.h"
#include <stddef.h>
#include "fifo8.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1
#define KEY_BUF_SIZE 32
#define MOUSE_BUF_SIZE 128

#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47
#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

extern int io_in8(int port);
extern void io_out8(int port, int data);
extern void reg_int_handler(int irn, void (*handle)(void));

void irq1_handle(void);
void irq12_handle(void);

static void wait_KBC_sendready(void);

static unsigned char keybuf[KEY_BUF_SIZE];
static struct Fifo8 keyinfo = {
	.buf = keybuf,
	.size = KEY_BUF_SIZE,
	.free = KEY_BUF_SIZE,
	.next_r = 0,
	.next_w = 0
};

static unsigned char mousebuf[MOUSE_BUF_SIZE];
static struct Fifo8 mouseinfo = {
	.buf = mousebuf,
	.size = MOUSE_BUF_SIZE,
	.free = MOUSE_BUF_SIZE,
	.next_r = 0,
	.next_w = 0
};
  
void init_pic(void) {
	/*     Ports
    *    PIC1   PIC2
    *Command 0x20   0xA0
    *Data    0x21   0xA1
    */

    /* ICW1 - begin initialization */
    io_out8(PIC1_COMMAND, 0x11);
    io_out8(PIC2_COMMAND, 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    io_out8(PIC1_DATA, 0x20);
    io_out8(PIC2_DATA, 0x28);

    /* ICW3 - setup cascading */
    io_out8(PIC1_DATA, 4);
    io_out8(PIC2_DATA, 2);

    /* ICW4 - environment info */
    io_out8(PIC1_DATA, 0x01);
    io_out8(PIC2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    io_out8(PIC1_DATA, 0xf9); /* 11111011 PIC1以外全部禁止 */
    io_out8(PIC2_DATA, 0xef); /* 11111111 禁止所有中断 */

	reg_int_handler(1, irq1_handle);
	reg_int_handler(12, irq12_handle);
}

void init_keyboard(void) {
	/* 初始化键盘控制电路 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
}


static void wait_KBC_sendready(void) {
	/* 等待键盘控制电路准备完毕 */
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
}

void enable_mouse(void) {
	/* 激活鼠标 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	/* 顺利的话，键盘控制器会返回ACK(0xfa) */
}

inline int get_keycode(void) {
	return fifo8_read(&keyinfo);
}

inline int get_mousecode(void) {
	return fifo8_read(&mouseinfo);
}	

inline bool has_keycode(void) {
	return fifo8_hasval(&keyinfo);
}

inline bool has_mousecode(void) {
	return fifo8_hasval(&mouseinfo);
}

/* 设置键盘处理器程序 */
void irq1_handle(void) {
    io_out8(PIC1_COMMAND, 0x61);
    int scancode = io_in8(0x0060);
	fifo8_write(&keyinfo, scancode);	
}

/* 设置键盘处理器程序 */
void irq12_handle(void) {
	io_out8(PIC2_COMMAND, 0x64);	/* 通知PIC IRQ-12 已经受理完毕 */
	io_out8(PIC1_COMMAND, 0x62);
	int mousecode = io_in8(0x0060);	
	fifo8_write(&mouseinfo, mousecode);	
}
