
#include "mem.h"

void memman_init(struct MemMan *memman, unsigned int start, unsigned int size) {
	memman->free_cnt = 1;
	memman->frees[0].start = start;
	memman->frees[0].size = size;
}

unsigned int memman_alloc(struct MemMan *memman, unsigned int size) {
	for (int i = 0; i < memman->free_cnt; i++) {
		struct Free *free = &memman->frees[i];
		if (free->size >= size) {
			unsigned int start = free->start;
			free->size -= size;
			free->start += size;
			
			if (free->size == 0) {
				memman->free_cnt--;
				/* 移动数组 */
				for (int j = i; j < memman->free_cnt; j++) {
					memman->frees[j] = memman->frees[j+1];
				}
			}

			return start;
		}
	}

	return 0; 
}

int memman_free(struct MemMan *memman, unsigned int start, unsigned int size) {
	int i;
	for (i = 0; i < memman->free_cnt; i++) {
		if (memman->frees[i].start > start) {
			break;
		}
	}
	
	struct Free *curfree = &memman->frees[i];

	// 当前段尝试向前合并
	if (start + size == curfree->start) {
		curfree->start -= size;
		curfree->size += size;	
		return 0;
	}

	// 前一个段尝试向后合并
	if (i > 0) {
		struct Free *prefree = &memman->frees[i-1];
		if (prefree->start + prefree->size == start) {
			prefree->size += size;
			return 0;
		}
	}	

	// 前后段都不能合并到
	if (memman->free_cnt < MAX_FREE) {
		memman->free_cnt++;
		
		for (int k = memman->free_cnt-1; k > i; k--) {
			memman->frees[k] = memman->frees[k-1];			
		}
		curfree->start = start;
		curfree->size = size;
	}

	return -1;
}

unsigned int memman_total(struct MemMan *memman) {
	unsigned int count = 0;
	for (int i = 0; i < memman->free_cnt; i++) {
		count += memman->frees[i].size;
	}
	return count;
}
