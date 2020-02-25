/*
author ron
2020.02.25
*/

// 定义调色板模式

#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA  0x3C9


// 休息
void io_hlt();
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

// 初始化调色板
void init_palette();

void set_palette(unsigned char *rgb);

void boxfill(unsigned char *vga, int xsize, unsigned char color, 
	int x0, int y0, int x1, int y1);

// 程序入口
void main (void) 
{
	char *vga = (char*) 0xa0000;
	int offset;
	int i;
	
	init_palette();
 
/*	for (offset=0; offset <= 0xffff; offset++) {
		vga[offset] = offset & 0x0f;
	}
*/
	
	
	boxfill(vga, 320, 1, 10, 10, 190, 190);
    for(;;) {
		io_hlt();
    } //make sure our kernel never stops, with an infinite loop
}

void boxfill(unsigned char *vga, int xsize, unsigned char color, int x0, int y0, int x1, int y1)
{

	int x, y;
	for (x = x0; x <= x1; x++) {
		for (y = y0; y <= y1; y++) {
			vga[y * xsize + x] = color; 
		}
	}
}

void init_palette() 
{

	static unsigned char rgb[] = {
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
	
//	io_sti();
	io_push_eflags(eflags);
}


