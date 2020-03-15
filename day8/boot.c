/*
author ron
2020.02.25
*/
#include "graphic.h"
#include "scancodes.h"


extern void load_idt();

extern void reg_int_handler(int irn, void *func);

extern void io_out8(int port, int data);

extern int io_in8(int port);

void irq1_handle();

// 休息
extern void io_hlt();


// 启动信息，和head.asm中对应
struct BOOTINFO {
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	unsigned char *vram;
};


// 程序入口
void main (void) 
{
	struct BOOTINFO *bootinfo;
	load_idt();

	reg_int_handler(1, irq1_handle);

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	io_out8(0x20 , 0x11);
	io_out8(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	io_out8(0x21 , 0x20);
	io_out8(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	io_out8(0x21 , 4);  
	io_out8(0xA1 , 2);  

	/* ICW4 - environment info */
	io_out8(0x21 , 0x01);
	io_out8(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	io_out8(0x21,  0xf9  ); /* 11111011 PIC1以外全部禁止 */
	io_out8(0xa1,  0xef  ); /* 11111111 禁止所有中断 */	

	bootinfo = (struct BOOTINFO *) 0x0ff0;

	init_palette();
	draw_ui(bootinfo->vram, bootinfo->scrnx, bootinfo->scrny);	
	
    for(;;) {
		io_hlt();
    } //make sure our kernel never stops, with an infinite loop
}

void irq0_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq0");	
}
int a=10;
void irq1_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	//io_out8(0x0020, 0x61);
	int scancode = io_in8(0x0060);

	unsigned char sc2ascii[] = {SCANCODE2ASCII_TABLE};

	unsigned char ascii = sc2ascii[scancode];

	unsigned char result[2] = {
		ascii, 0x00
	};
	a += 10;
	showchars(vram, 320, COL_RED, a, 140, result);	
}
void irq2_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq2");	
}
void irq3_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq3");	
}
void irq4_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq4");	
}
void irq5_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq5");	
}
void irq6_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq6");	
}
void irq7_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq7");	
}
void irq8_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq8");	
}
void irq9_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq9");	
}
void irq10_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq10");	
}
void irq11_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq11");	
}
void irq12_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq12");	
}
void irq13_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq13");	
}
void irq14_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq14");	
}
void irq15_handle() {
	unsigned char *vram = (unsigned char*) 0xa0000;
	showchars(vram, 320, COL_RED, 0, 140, "irq15");	
}
