#include "mouse.h"
#include <stddef.h>


#define MOUSE_CODE_SIZE 		3
#define MOUSE_READYED   		0xfa
#define MOUSE_PHASE_UNREADY 	-1


static struct MouseDec mousedec;

static unsigned char buf[MOUSE_CODE_SIZE];
static int phase = MOUSE_PHASE_UNREADY;


struct MouseDec* mouse_decode(int mousecode) {
    if (phase == MOUSE_PHASE_UNREADY) {
        // 鼠标状态准备好了
        if (mousecode == MOUSE_READYED) {
            phase = 0;
        }
        return NULL;
    }

    // 判断鼠标第一个字节的范围，确保buf中内容不会错位
    if (phase == 0 && (mousecode & 0xc8) != 0x08) {
        return NULL;
    }

    buf[phase++] = mousecode;
    if (phase == MOUSE_CODE_SIZE) {
        unsigned char char1 = buf[0];

        mousedec.btn = char1 & 0x07;
        mousedec.x = buf[1];
        mousedec.y = buf[2];
        if (char1 & 0x10) {
            mousedec.x |= 0xffffff00;
        }
        if (char1 & 0x20) {
            mousedec.y |= 0xffffff00;
        }
        /* 鼠标的y方向与画面符号相反 */
        mousedec.y = - mousedec.y;
        phase = 0;
        return &mousedec;
    }
    return NULL;
}
