void io_hlt();

void main (void) 
{
	char *vga;
 
	for (vga=0xa0000; vga <= 0xaffff; vga++) {
		*vga = (int) vga & 0x0f;
	}

    for(;;) {
		io_hlt();
    } //make sure our kernel never stops, with an infinite loop
}
