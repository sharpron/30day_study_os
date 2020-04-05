
#include <stdint.h>

/* 整数转字符串 */
void itoa(char *result, long number);
void uitoa(char *buf, unsigned long number);

/* 整数转字符串，指定进制 */
void itoa_radix(char *result, long number, int radix);
