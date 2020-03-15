
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA  0x3C9

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
#define COL_D_YELLOW        11
#define COL_D_CYAN      12
#define COL_D_PURPLE        13
#define COL_L_D_BLUE        14
#define COL_D_GREY      15

// 初始化调色板
void init_palette(void);
// 设置调色板
void set_palette(unsigned char *rgb);

void draw_ui(unsigned char *vram, int scrnx, int scrny);

void drawchar(unsigned char *vram, int scrnx, unsigned char color, int x, int y, char *font);
// 显示多个字符
void showchars(unsigned char *vram, int xsize,
    unsigned char color, int x, int y, char *s);
//
void to_asc_chars(char *result, int number);

void drawmouse(unsigned char *vram, int xsize,
        unsigned char color, int x, int y);

void fillrect(unsigned char *vram, int xsize, unsigned char color,
    int x0, int y0, int x1, int y1);
