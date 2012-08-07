/*
 * memory.h
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "type.h"
#include "setbit.h"

#define MAX_PAGE_ENTRY	1024
#define PAGE_SIZE		4096

#define MEM_BMP_MB(i)	8*i/16

#define MB_TO_PAGE(i)	i*256
#define B_TO_PAGE(i)	i/PAGE_SIZE
#define PAGE_TO_NEED_TABLE(i)		i/MAX_PAGE_ENTRY
#define B_TO_NEED_TABLE(i)	PAGE_TO_NEED_TABLE(B_TO_PAGE(i))


struct ARDS
{
	u32 BaseAddrLow;
	u32 BaseAddrHigh;
	u32 LenthLow;
	u32 LenthHigh;
	u32 Type;
};

void mem_init();

void mem_page_busy(u32 index);
void mem_page_free(u32 index);
u32 mem_page_getfree();
void* mem_page_alloc();

extern struct ARDS mem_info[12];
extern int mem_size;
extern u32* mem_used_map;
extern int mem_used_map_max;
extern int mem_mcr_number;

#endif /* MEMORY_H_ */
