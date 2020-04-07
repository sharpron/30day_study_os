
#include "format.h"

static char *map = "0123456789ABCDEF";

void itoa(char *buf, long number) {
	itoa_radix(buf, number, 10);	
}

void uitoa(char *buf, unsigned long number) {
	unsigned long radix = 10;
	int i;
    for (i = 0; number >= radix; i++) {
       	buf[i] = map[number % radix];
        number /= radix;
    }
    buf[i] = map[number];

    // swap order
    for (int start = 0, end = i; start < end; start++, end--) {
        char endchar = buf[end];
        buf[end] = buf[start];
        buf[start] = endchar;
    }

    buf[i+1] = '\0';
}

void itoa_radix(char *buf, long number, int radix) {
    if (number < 0) {
        number = -number;
        *buf = '-';
        buf++;
    }

    int i;
    for (i = 0; number >= radix; i++) {
        buf[i] = map[number % radix];
        number /= radix;
    }
    buf[i] = map[number];

    // swap order
    for (int start = 0, end = i; start < end; start++, end--) {
        char endchar = buf[end];
        buf[end] = buf[start];
        buf[start] = endchar;
    }

    buf[i+1] = '\0';
}

