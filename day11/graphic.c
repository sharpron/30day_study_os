
#include "graphic.h"


#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA  0x3C9

extern void io_cli(void);
extern void io_sti(void);

extern void io_outb(int port, int data);

extern int io_poll_eflags(void);
extern void io_push_eflags(int eflags);

extern char font[4096];

static void set_palette(unsigned char *rgb);
static void drawchar(struct VideoInfo *info, char color,
     int x, int y, char *curfont);

void init_palette(void) {
	unsigned char rgb[] = {
        0x00, 0x00, 0x00, // 黑色
        0xff, 0x00, 0x00, // 亮红
        0x00, 0xff, 0x00, // 亮绿
        0xff, 0xff, 0x00, // 亮黄
        0x00, 0x00, 0xff, // 亮蓝
        0xff, 0x00, 0xff, // 亮紫
        0x00, 0xff, 0xff, // 浅亮蓝
        0xff, 0xff, 0xff, // 白色
        0xc6, 0xc6, 0xc6, // 亮灰
        0x84, 0x00, 0x00, // 暗红
        0x00, 0x84, 0x00, // 暗绿
        0x84, 0x84, 0x00, // 暗黄
        0x00, 0x00, 0x84, // 暗青
        0x84, 0x00, 0x84, // 暗紫
        0x00, 0x84, 0x84, // 浅暗蓝
        0x84, 0x84, 0x84  // 暗灰
    };	
	set_palette(rgb);
}

static void set_palette(unsigned char *rgb) {
    int i, eflags;
    eflags = io_poll_eflags();
    io_cli();

    io_outb(PALETTE_WRITE, 0);

    for (i = 0; i < 48; i++) {
        io_outb(PALETTE_DATA, rgb[i]/4);
    }

    io_sti();
    io_push_eflags(eflags);
}

void drawtext(struct VideoInfo *info, unsigned char color, int x, int y, char *text) {
	while (*text) {
        drawchar(info, color, x, y, font + *text * 16);
        text ++;
        x += 8;
    }
}

static void drawchar(struct VideoInfo *info, char color,
	 int x, int y, char *curfont) {
    char data;
    for (int i = 0; i < 16; i++) {
        data = curfont[i];
        for (int j = 0; j < 8; j++) {
            if (data & (1 << j)) {
				setpixel(info, color, x + j, y + i);
            }
        }
    }
}

void fillrect(struct VideoInfo *info, unsigned char color,
    int x0, int y0, int x1, int y1) {

    for (int x = x0; x <= x1; x++) {
        for (int y = y0; y <= y1; y++) {
			setpixel(info, color, x, y);
        }
    }
}

void setpixel(struct VideoInfo *info, unsigned char color, int x, int y) {
	info->vram[y * info->scrnx + x] = color;
}
