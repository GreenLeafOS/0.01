/*
 * memory.h
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "Type.h"
#include "setbit.h"


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

extern struct ARDS mem_info[12];
extern int mem_size;
extern u32* mem_used_map;
extern int mem_used_map_max;
extern int mem_mcr_number;

#endif /* MEMORY_H_ */
