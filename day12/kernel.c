
#include "ui.h"
#include "interrupt.h"
#include "mem.h"

extern void load_idt(void);
extern void io_hlt(void);
extern void io_cli(void);
extern void io_sti(void);
extern void io_sti_hlt(void);


struct MemoryDec {
    unsigned int base_low;
    unsigned int base_high;

    unsigned int len_low;
    unsigned int len_high;

    unsigned int type;
    unsigned int other;
};

void init_mm(struct MemMan *memman, short memcount, struct MemoryDec *memdec);

// 内核入口函数
void _start(void) {

	init_pic();
	init_keyboard();
	init_mouse();
	load_idt();

	struct VideoInfo *info = (struct VideoInfo*) 0x0ff4;
	enter_ui(info);
	

	struct MemMan *memman = (struct MemMan*) 0x2000;
	short *memcount = 0x1000;
	struct MemoryDec *memdec = 0x1002;
	init_mm(memman, *memcount, memdec);	

	drawint(memman->frees[0].start, 20, 20);
	drawint(memman->frees[0].size, 100, 20);
	drawint(memman_total(memman), 180, 20);

	unsigned int r = memman_alloc(memman, 10);
	drawint(r, 100, 100);
	memman_free(memman, r, 5);
	
	drawint(memman->frees[1].start, 20, 50);
	drawint(memman->frees[1].size, 100, 50);
	drawint(memman_total(memman), 180, 50);

	for(;;) {
        io_cli();
        if (has_keycode()) {
            int keycode = get_keycode();
            io_sti();
			handle_key(keycode);
        } else if (has_mousecode()) {
            int mousecode = get_mousecode();
            io_sti();
            struct MouseDec *mousedec = mouse_decode(mousecode);
            if (mousedec) {
                handle_mouse(mousedec);
            }
        } else {
            io_sti_hlt();
        }
    } //make sure our kernel never stops, with an infinite loop
}

void init_mm(struct MemMan *memman, short memcount, struct MemoryDec *memdec) {
	unsigned int start = 0;
	unsigned int size = 0;
	for (int i = 0; i < memcount; i++) {
		if (memdec->type == 1 && memdec->len_low > size) {
			start = memdec->base_low;	
			size = memdec->len_low;
		}
        memdec++;
    }
	memman_init(memman, start, size);
}
