
#include "ui.h"
#include "interrupt.h"

extern void load_idt(void);
extern void io_hlt(void);
extern void io_cli(void);
extern void io_sti(void);
extern void io_sti_hlt(void);


struct MemoryInfo {
    unsigned int base_low;
    unsigned int base_high;

    unsigned int len_low;
    unsigned int len_high;

    unsigned int type;
    unsigned int other;
};

// 内核入口函数
void _start(void) {

	init_pic();
	init_keyboard();
	init_mouse();
	load_idt();

	struct VideoInfo *info = (struct VideoInfo*) 0x0ff4;
	enter_ui(info);
	
	struct MemoryInfo *head = 0x1002;
	for (int i = 0; i < 10; i++) {
		drawint(head->type, 10, (i + 1) * 20);
        head ++;
    }


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

