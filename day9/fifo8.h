#include "stdbool.h"
/*
 * 先进先出的数据结构
 */
struct Fifo8 {
    unsigned char *buf;
    int size;
    int next_r, next_w, free;
};

void fifo8_init(struct Fifo8 *fifo, int size, unsigned char *buf);

/* 写入成功返回true，超出容量返回false */
bool fifo8_write(struct Fifo8 *fifo, unsigned char data);

/* 从队列中读取一个内容 */
int fifo8_read(struct Fifo8 *fifo);

/* 获取实际长度 */
int fifo8_len(struct Fifo8 *fifo);

/* 队列中是否有值 */
bool fifo8_hasval(struct Fifo8 *fifo);
