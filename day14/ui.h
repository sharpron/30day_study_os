#include "layer.h"
#include "mouse.h"
#include "mem.h"

void enter_ui(struct Graphic *graphic, struct MemMan *memman);

void drawint(int value, int x, int y);

void handle_key(int keycode);

void handle_mouse(struct MouseDec *mousedec);
