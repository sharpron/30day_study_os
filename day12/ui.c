#include "ui.h"
#include "format.h"

static void draw_bg(void);

static struct VideoInfo *_info = 0;

static int mousex = 100;
static int mousey = 100;

void enter_ui(struct VideoInfo *info) {
	_info = info;
	init_palette();
	draw_bg();
	drawtext(_info, 1, 0, 0, "welcome to los!");
	
	
	
}

void drawint(int value, int x, int y) {
	char buf[30];
    itoa(buf, value);
    drawtext(_info, COL_RED, x, y, buf);
}
	

static void draw_bg(void) {
	int dock_height = _info->scrny / 5;
    int dock_width = _info->scrnx * 2 / 3;
	
	fillrect(_info, COL_L_D_BLUE, 0, 0, _info->scrnx - 1, _info->scrny - 1);

	
    int startx = (_info->scrnx - dock_width) / 2;
    int starty = 150;
    fillrect(_info, COL_GREY, startx, starty,
        startx + dock_width, starty + dock_height);	

	
	drawmouse(COL_D_RED, mousex, mousey);

}


void drawmouse(unsigned char color, int x, int y) {
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
				setpixel(_info, color, x + j, y + i);
            }
        }
    }

}

void handle_key(int keycode) {
	char a[30];
    itoa(a, keycode);
    drawtext(_info, COL_RED, 30, 140, a);
}

void handle_mouse(struct MouseDec *mousedec) {
	fillrect(_info, COL_L_D_BLUE, mousex, mousey, mousex + 8, mousey + 4);
    mousex += mousedec->x;
    mousey += mousedec->y;
    if (mousex < 0) {
        mousex = 0;
    }
    if (mousex > _info->scrnx - 8) {
        mousex = _info->scrnx - 8;
    }
    if (mousey < 0) {
        mousey = 0;
    }
    if (mousey > _info->scrny - 4) {
        mousey = _info->scrny - 4;
    }

    drawmouse(COL_RED, mousex, mousey);
}
