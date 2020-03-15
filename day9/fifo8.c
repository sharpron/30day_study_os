#include "fifo8.h"

//初始化fifo
void fifo8_init(struct Fifo8 *fifo, int size, unsigned char *buf) {
	fifo->size = size;
	fifo->free = size;
	fifo->buf = buf;
	fifo->next_r = 0;
	fifo->next_w = 0;
}	

/*
 * 向队列中添加数据
 */
bool fifo8_write(struct Fifo8 *fifo, unsigned char data) {
	if (fifo->free == 0) {
		return false;
	}
	
	fifo->buf[fifo->next_w] = data;
	fifo->next_w++;
	fifo->free--;

	
	if (fifo->next_w == fifo->size) {
		fifo->next_w = 0;
	}
	return true;
}

/*
 * 从队列中读取一个数据
 */
int fifo8_read(struct Fifo8 *fifo) {
	if (fifo->free == fifo->size) {
		return -1;
	}
	
	unsigned char val = fifo->buf[fifo->next_r];
	fifo->next_r++;
	fifo->free++;	
	if (fifo->next_r == fifo->size) {
		fifo->next_r = 0;
	}
	return val;
}

int fifo8_len(struct Fifo8 *fifo) {
	return fifo->size - fifo->free;
}

bool fifo8_hasval(struct Fifo8 *fifo) {
	return fifo->free < fifo->size;
}

