
#include "graphic.h"
// 清除中断标志
void io_cli();
// 设置中断标志
void io_sti();
// 向指定端口发送8字节的数据
void io_out8(int port, int data);
// 取得eflags
int io_poll_eflags();
// 放回eflags
void io_push_eflags(int eflags);

void init_palette()
{

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


void set_palette(unsigned char *rgb)
{
    int i, eflags;
    eflags = io_poll_eflags();
    io_cli();

    io_out8(PALETTE_WRITE, 0);

    for (i = 0; i < 48; i++) {
        io_out8(PALETTE_DATA, rgb[i]/4);
    }

    //io_sti();
    io_push_eflags(eflags);
}

void draw_ui(unsigned char *vram, int scrnx, int scrny) {
    int dock_height = scrny / 5;
    int dock_width = scrnx * 2 / 3;

    fillrect(vram, scrnx, COL_L_D_BLUE, 0, 0, scrnx - 1,
        scrny - 1);

    int startx = (scrnx - dock_width) / 2;
    int starty = 150;
    fillrect(vram, scrnx, COL_GREY, startx, starty,
        startx + dock_width, starty + dock_height);

    showchars(vram, scrnx, COL_RED, 0, 0, "helloworld");
    char result[40];

    to_asc_chars(result, 19562);


    showchars(vram, scrnx, COL_RED, 16, 64, result);
}

void drawmouse(unsigned char *vram, int xsize,
    unsigned char color, int x, int y) {

    unsigned char mouse[4][8] = {
        "..****..",
        "********",
        "********",
        "..****..",
    };

    for (int i = 0; i < 4; i ++) {
        unsigned char *line = mouse[i];
        for (int j = 0; j < 8; j++) {
            if (line[j] == '*') {
                vram[(y+i) * xsize + x + j] = color;
            }
        }
    }

}

void showchars(unsigned char *vram, int xsize,
    unsigned char color, int x, int y, char *s) {
    extern char font[4096];
    while (*s != 0x00) {
        drawchar(vram, xsize, color, x, y, font + *s * 16);
        s ++;
        x += 8;
    }
}

void drawchar(unsigned char *vram, int xsize,
    unsigned char color, int x, int y, char *font)
{
    char d;
    int i;
    for (i = 0; i < 16; i++) {
        d = font[i];
        for (int j = 0; j < 8; j++) {
            if (d & (1 << j)) {
                vram[(y+i) * xsize + x + j] = color;
            }
        }
    }


}

void fillrect(unsigned char *vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1)
{

    int x, y;
    for (x = x0; x <= x1; x++) {
        for (y = y0; y <= y1; y++) {
            vram[y * xsize + x] = color;
        }
    }
}

void to_asc_chars(char *result, int number)
{

    char s[10];
    int i = 0;

    int offset = 0;

    if (number < 0) {
        result[offset] = '-';
        offset++;
        number = -number;
    }

    for (; number > 0; i++) {
        int mod = number % 10;
        s[i] = (char) ('0' + mod);
        number = number / 10;
    }

    for (; i > 0; i--, offset++) {
        result[offset] = s[i-1];
    }
	result[offset] = 0x00;
}
