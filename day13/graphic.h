
// 调色板存放的颜色
#define COL_BLACK       0
#define COL_RED         1
#define COL_GREEN       2
#define COL_YELLOW      3
#define COL_BLUE        4
#define COL_PURPLE      5
#define COL_L_BLUE      6
#define COL_WHITE       7
#define COL_GREY        8
#define COL_D_RED       9
#define COL_D_GREEN     10
#define COL_D_YELLOW    11
#define COL_D_CYAN      12
#define COL_D_PURPLE    13
#define COL_L_D_BLUE    14
#define COL_D_GREY      15
// 透明的颜色
#define COL_TRANS		99


struct Graphic {
	int width;
	int height;
	unsigned char *buf;
};

/* 初始化调色板 */
void init_palette(void);

/* 写文字 */
void drawtext(struct Graphic *graphic, unsigned char color, int x, int y, char *text);

/* 填充矩形 */
void fillrect(struct Graphic *graphic, unsigned char color, 
	int x0, int y0, int x1, int y1);

/* 指定某一像素点上的颜色 */
void setpixel(struct Graphic *graphic, unsigned char color, int x, int y);
