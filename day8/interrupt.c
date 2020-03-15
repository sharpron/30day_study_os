#include "interrupt.h"

void init_pic() {
	/*     Ports
    *    PIC1   PIC2
    *Command 0x20   0xA0
    *Data    0x21   0xA1
    */

    /* ICW1 - begin initialization */
    io_out8(PIC1_COMMAND, 0x11);
    io_out8(PIC2_COMMAND, 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    io_out8(PIC1_DATA, 0x20);
    io_out8(PIC2_DATA, 0x28);

    /* ICW3 - setup cascading */
    io_out8(PIC1_DATA, 4);
    io_out8(PIC2_DATA, 2);

    /* ICW4 - environment info */
    io_out8(PIC1_DATA, 0x01);
    io_out8(PIC2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    io_out8(PIC1_DATA, 0xf9); /* 11111011 PIC1以外全部禁止 */
    io_out8(PIC2_DATA, 0xef); /* 11111111 禁止所有中断 */
}
