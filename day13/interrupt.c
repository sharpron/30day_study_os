#include "interrupt.h"
#include "fifo8.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

#define PORT_KEYDAT             0x0060
#define PORT_KEYSTA             0x0064
#define PORT_KEYCMD             0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE       0x60
#define KBC_MODE                0x47
#define KEYCMD_SENDTO_MOUSE     0xd4
#define MOUSECMD_ENABLE         0xf4

#define KEY_BUF_SIZE 			32
#define MOUSE_BUF_SIZE          128

#define MOUSE_IRQ               12

extern int io_inb(int port);
extern void io_outb(int port, int data);
extern void reg_int_handler(int irn, void (*handle)(void));

static void irq1_handle(void);
static void irq12_handle(void);
static void wait_KBC_sendready(void);

void init_pic(void) {
	/*     Ports
    *    PIC1   PIC2
    *Command 0x20   0xA0
    *Data    0x21   0xA1
    */

    /* ICW1 - begin initialization */
    io_outb(PIC1_COMMAND, 0x11);
    io_outb(PIC2_COMMAND, 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    io_outb(PIC1_DATA, 0x20);
    io_outb(PIC2_DATA, 0x28);

    /* ICW3 - setup cascading */
    io_outb(PIC1_DATA, 4);
    io_outb(PIC2_DATA, 2);

    /* ICW4 - environment info */
    io_outb(PIC1_DATA, 0x01);
    io_outb(PIC2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    io_outb(PIC1_DATA, 0xf9); /* 11111011 PIC1以外全部禁止 */
    io_outb(PIC2_DATA, 0xef); /* 11111111 禁止所有中断 */

}

void init_keyboard(void) {
    reg_int_handler(1, irq1_handle);
    /* 初始化键盘控制电路 */
    wait_KBC_sendready();
    io_outb(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_outb(PORT_KEYDAT, KBC_MODE);
}

void init_mouse(void) {
	reg_int_handler(MOUSE_IRQ, irq12_handle);
	wait_KBC_sendready();
    io_outb(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_outb(PORT_KEYDAT, MOUSECMD_ENABLE);
}

static void wait_KBC_sendready(void) {
    /* 等待键盘控制电路准备完毕 */
    for (;;) {
        if ((io_inb(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
}

static unsigned char keybuf[KEY_BUF_SIZE];

static struct Fifo8 keyinfo = {
    .buf = keybuf,
    .size = KEY_BUF_SIZE,
    .free = KEY_BUF_SIZE,
    .next_r = 0,
    .next_w = 0
};

/* 键盘中断处理 */
static void irq1_handle(void) {
	io_outb(PIC1_COMMAND, 0x61);
	int keycode = io_inb(0x60);
	fifo8_write(&keyinfo, keycode);
}

bool has_keycode(void) {
	return fifo8_hasval(&keyinfo);
}

int get_keycode(void) {
	return fifo8_read(&keyinfo);
}

static unsigned char mousebuf[MOUSE_BUF_SIZE];
static struct Fifo8 mouseinfo = {
    .buf = mousebuf,
    .size = MOUSE_BUF_SIZE,
    .free = MOUSE_BUF_SIZE,
    .next_r = 0,
    .next_w = 0
};
/* 鼠标中断处理 */
static void irq12_handle(void) {
	io_outb(PIC2_COMMAND, 0x64);    /* 通知PIC IRQ-12 已经受理完>毕 */
    io_outb(PIC1_COMMAND, 0x62);
    int mousecode = io_inb(0x60);
	fifo8_write(&mouseinfo, mousecode);
}

bool has_mousecode(void) {
    return fifo8_hasval(&mouseinfo);
}

int get_mousecode(void) {
    return fifo8_read(&mouseinfo);
}
