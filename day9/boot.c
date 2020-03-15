/*
author ron
2020.02.25
*/
#include "graphic.h"
#include "interrupt.h"

#define BOOT_INFO_ADDR  0x0ff0
#define MOUSE_BUF_SIZE  3
#define MOUSE_READYED   0xfa
#define MOUSE_PHASE_UNREADY -1


extern void load_idt(void);


extern void io_cli(void);

extern void io_sti(void);

extern void io_sti_hlt(void);

// 休息
extern void io_hlt(void);

void irq1_handle(void);

struct BOOTINFO {
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	unsigned char *vram;
};

struct MouseDec {
	unsigned char buf[MOUSE_BUF_SIZE];
	int phase;
	int x, y, btn;
};

_Bool mouse_decode(struct MouseDec *mousedec, int mousecode);

static struct BOOTINFO *bootinfo = (struct BOOTINFO*) BOOT_INFO_ADDR;


// 程序入口
void main (void) 
{
	load_idt();
	
	init_pic();
	init_keyboard();

	init_palette();
	draw_ui(bootinfo->vram, bootinfo->scrnx, bootinfo->scrny);	

	enable_mouse();
	int x = 0;
	
	struct MouseDec mousedec;	
	mousedec.phase = MOUSE_PHASE_UNREADY;
	
	int mousex = 16, mousey = 100;
	drawmouse(bootinfo->vram, bootinfo->scrnx, COL_RED, mousex, mousey);
	
    for(;;) {
		io_cli();
		if (has_keycode()) {
			int keycode = get_keycode();
			io_sti();
			char a[30];
			to_asc_chars(a, keycode);
			showchars(bootinfo->vram, 320, COL_RED, x+=30, 140, a);	
		} else if (has_mousecode()) {
			int mousecode = get_mousecode();
			io_sti();
			if (mouse_decode(&mousedec, mousecode) == 1) {
				fillrect(bootinfo->vram, bootinfo->scrnx, COL_L_D_BLUE, mousex, mousey, 
					mousex + 8, mousey + 4);
				mousex += mousedec.x;
				mousey += mousedec.y;
				
				if (mousex < 0) {
					mousex = 0;
				}
				if (mousex > bootinfo->scrnx - 8) {
					mousex = bootinfo->scrnx - 8;
				}
				if (mousey < 0) {
					mousey = 0;
				}
				if (mousey > bootinfo->scrny - 4) {
					mousey = bootinfo->scrny - 4;
				}
				
				drawmouse(bootinfo->vram, bootinfo->scrnx, COL_RED, mousex, mousey);
				
			}	
		} else {
			io_sti_hlt();
		}
		
    } //make sure our kernel never stops, with an infinite loop
}

_Bool mouse_decode(struct MouseDec *mousedec, int mousecode) {
	if (mousedec->phase == MOUSE_PHASE_UNREADY) {
		// 鼠标状态准备好了
		if (mousecode == MOUSE_READYED) {
			mousedec->phase = 0;
		}
		return 0;
	} 
	
	// 判断鼠标第一个字节的范围，确保buf中内容不会错位	
	if (mousedec->phase == 0 && (mousecode & 0xc8) != 0x08) {
		return 0;		
	}

	mousedec->buf[mousedec->phase++] = mousecode;
	if (mousedec->phase == MOUSE_BUF_SIZE) {
		unsigned char char1 = mousedec->buf[0];

		mousedec->btn = char1 & 0x07;
		mousedec->x = mousedec->buf[1];
		mousedec->y = mousedec->buf[2];
		if (char1 & 0x10) {
			mousedec->x |= 0xffffff00;
		}
		if (char1 & 0x20) {
			mousedec->y |= 0xffffff00;
		}     
		/* 鼠标的y方向与画面符号相反 */   
		mousedec->y = - mousedec->y; 
		mousedec->phase = 0;
		return 1;
	}
	return 0;
}	
