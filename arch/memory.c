/*
 * memory.c
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#include "memory.h"

/*
 * 初始化
 * 获得内存大小
 * 计算位图大小
 * 位图中一个位表示一个页的使用情况
 * 则一个项可表示32个页。32*4096/1024 = 128 Kb = 1/8 M
 * 8个表项就是 128*8/1024 = 1 M
 */

void mem_init()
{
	/* 获得内存大小 */
	for(int i=0;i<mem_mcr_number;i++)
	{
		if(mem_info[i].Type == 1)
			if (mem_info[i].BaseAddrLow + mem_info[i].LenthLow > mem_size)
				mem_size = mem_info[i].BaseAddrLow + mem_info[i].LenthLow;
	}

	/* 计算位图大小 */
	mem_used_map_max = (mem_size/PAGE_SIZE)/32 + 1;
	mem_size = mem_used_map_max*32*PAGE_SIZE;

	/* 给指针赋值 */
	mem_used_map =(u32*)0x16000;

	/* 保留4M的内核空间 */
	for(int i=0;i<MEM_BMP_MB(4);i++)
		*(mem_used_map + i) = ~0;

	/* 其余设置为0 */
	for(int i=MEM_BMP_MB(4);i<mem_used_map_max;i++)
		*(mem_used_map + i) = 0;
}



/*
 * 设置某个页为使用
 * 在位图中，1表示使用，0表示空闲
 * */
void mem_page_busy(u32 index)
{
	if ((index * PAGE_SIZE) > mem_size) return;
	BITSET(mem_used_map,index);
}



/*
 * 设置某个页为空闲
 * 在位图中，1表示使用，0表示空闲
 * */
void mem_page_free(u32 index)
{
	if ((index * PAGE_SIZE) > mem_size) return;
	BITCLEAR(mem_used_map,index);
}



/*
 * 查找一个空闲页
 * 在位图中，1表示使用，0表示空闲
 * 返回页标号
 * */
u32 mem_page_getfree()
{
	for(int i=0;i<mem_used_map_max;i++)
	{
		u32 l_bmp = *(mem_used_map + i);
		if (l_bmp == 0xffffffff) continue;
		for(int j=0;j<32;j++)
		{
			if (!(BITTEST(&l_bmp,j)))
			{
				return ((i*32)+j);
			}/* end if */
		}/* end for */
	}/* end for*/
	return -1;
}


/*
 * 分配一个页
 * 返回地址
 * ebp - 20 j
 */
void* mem_page_alloc()
{
	for(int i=0;i<mem_used_map_max;i++)
	{
		u32 l_bmp = *(mem_used_map + i);
		if (l_bmp == 0xffffffff) continue;
		for(int j=0;j<32;j++)
		{
			if (!(BITTEST(&l_bmp,j)))
			{
				int index = (i*32)+j;
				BITSET(mem_used_map,index);
				return (index*PAGE_SIZE);
			}
		}
	}
	return 0xffffffff;
}
