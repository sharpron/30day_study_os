#include "ui.h"
#include "format.h"

#define MOUSE_WIDTH 	7
#define MOUSE_HEIGHT 	11

static void draw_bg(struct Graphic *graphic);

void drawmouse(struct Graphic *graphic, unsigned char color);

static struct LayerCtl *ctl;
static struct Layer *bglayer;
static struct Layer *mouselayer;

void enter_ui(struct Graphic *graphic, struct MemMan *memman) {
	init_palette();

	ctl = layerctl_init(memman, graphic);

	bglayer = layer_alloc(ctl);
	//drawint(memman->frees[0].start, 20, 100);
    //drawint(memman->frees[0].size, 100, 100);
    //drawint(memman_total(memman), 180, 100);

	unsigned char *bgbuf = (unsigned char*) memman_alloc(memman, 
		graphic->width * graphic->height);
	layer_setbuf(bglayer, bgbuf, graphic->width, graphic->height);
	draw_bg(&bglayer->graphic);
	layer_setzindex(bglayer, 1);
	
	mouselayer = layer_alloc(ctl);
	unsigned char *mousebuf = (unsigned char*) memman_alloc(memman, 
		MOUSE_WIDTH * MOUSE_HEIGHT);
	layer_setbuf(mouselayer, mousebuf, MOUSE_WIDTH, MOUSE_HEIGHT);
	drawmouse(&mouselayer->graphic, COL_BLACK);

	mouselayer->y = 100;
	mouselayer->x = 100;

	layer_setzindex(mouselayer, 2);

	
}

void drawint(int value, int x, int y) {
	char buf[30];
    itoa(buf, value);
    drawtext(&bglayer->graphic, COL_RED, x, y, buf);
}
	

static void draw_bg(struct Graphic *graphic) {
	int dock_height = graphic->height / 5;
    int dock_width = graphic->width * 2 / 3;
	
	fillrect(graphic, COL_L_D_BLUE, 0, 0, graphic->width - 1, graphic->height - 1);

	
    int startx = (graphic->width - dock_width) / 2;
    int starty = 150;
    fillrect(graphic, COL_GREY, startx, starty,
        startx + dock_width, starty + dock_height);	
}


void drawmouse(struct Graphic *graphic, unsigned char color) {
    unsigned char mouse[MOUSE_HEIGHT][MOUSE_WIDTH] = {
		"*......",
		"**.....",
		"***....",
		"****...",
		"*****..",
		"******.",
		"*******",
		"****...",
		"**.*...",
		"*...*..",
		".....*.",
    };

    for (int y = 0; y < MOUSE_HEIGHT; y++) {
        unsigned char *line = mouse[y];
        for (int x = 0; x < MOUSE_WIDTH; x++) {
			unsigned char finalcolor = line[x] == '*' ? color : COL_TRANS;
			setpixel(graphic, finalcolor, x, y);
        }
    }

}

void handle_key(int keycode) {
	char a[30];
    itoa(a, keycode);
    drawtext(&bglayer->graphic, COL_RED, 30, 140, a);
}

void handle_mouse(struct MouseDec *mousedec) {
	int mousex = mouselayer->x;
	int mousey = mouselayer->y;

    mousex += mousedec->x;
    mousey += mousedec->y;
	
	int scrnx = ctl->graphic->width;
	int scrny = ctl->graphic->height;
    if (mousex < 0) {
        mousex = 0;
    }
    if (mousex > scrnx - 1) {
        mousex = scrnx - 1;
    }
    if (mousey < 0) {
        mousey = 0;
    }
    if (mousey > scrny - 1) {
        mousey = scrny - 1;
    }
	
	layer_move(mouselayer, mousex, mousey);
}
