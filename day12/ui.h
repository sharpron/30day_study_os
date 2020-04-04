#include "graphic.h"
#include "mouse.h"

void enter_ui(struct VideoInfo *info);

void drawmouse(unsigned char color, int x, int y);

void drawint(int value, int x, int y);

void handle_key(int keycode);

void handle_mouse(struct MouseDec *mousedec);
