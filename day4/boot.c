void io_hlt();

void main (void) 
{
    unsigned char* vga = (unsigned char*) 0xb8000;
    vga[0] = 'c'; //need to make sure that this is a character
    vga[1] = 0x09; //append the attribute byte
    for(;;) {
		io_hlt();
    } //make sure our kernel never stops, with an infinite loop
}
