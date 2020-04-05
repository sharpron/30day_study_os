#ifndef _mem_h
#define _mem_h

#define MAX_FREE	1000

/* 定义空闲的内存空间段 */
struct Free {
	unsigned int start, size;
};

/* 内存管理 */
struct MemMan {
	/* 空闲的内存空间段 */
	int free_cnt;

	/* 内存空间段 */
	struct Free frees[MAX_FREE];
	
};


/* 初始化内存管理器 */
void memman_init(struct MemMan *memman, unsigned int start, unsigned int size);

/* 分配指定大小的内存 */
unsigned int memman_alloc(struct MemMan *memman, unsigned int size);

/* 释放内存，指定大小 */
int memman_free(struct MemMan *memman, unsigned int start, unsigned int size);

/* 统计可用内存大小 */
unsigned int memman_total(struct MemMan *memman);

#endif
