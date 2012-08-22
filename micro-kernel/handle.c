/*
 * handle.c
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */

#include "include/sysapi.h"
#include "include/handle.h"

/* handle data */
struct handle_bmp handle_table[HANDLE_TABLE_BLCOK_MAX];



/*
 * SysApi new
 * 参数：无
 * 功能：分配一个对象句柄
 * 返回值：句柄id
 */
u32 new()
{
	/* 外层循环，查找表 */
	for(int i=0;i<HANDLE_TABLE_BLCOK_MAX;i++)
	{
		struct handle_bmp free = handle_table[i] ;
		if (free.bmp_size == 0)
		{
			/* 为空表分配内存 */
			handle_table[i].bmp_base = (u32*)alloc(HANDLE_TABLE_BLOCK_SIZE);
			bmp_set(handle_table[i].bmp_base,0);	// 保留第一位用于区别无效句柄

			handle_table[i].bmp_size = HANDLE_TABLE_BLOCK_ITEMS;
			handle_table[i].free_item = HANDLE_TABLE_BLOCK_ITEMS;
		}
		if (free.free_item == 0) continue;

		/* 查找空闲句柄 */
		int id = bmp_search(free.bmp_base,free.bmp_size);
		if (id == -1)
		{
			return 0;
		}
		else
		{
			free.free_item--;
			return (u32)id;
		}
	}
	return 0;
}





/*
 * SysApi delete
 * 参数：句柄指针
 * 功能：释放句柄
 * 返回值：无
 */
void delete(u32 handle)
{
	bmp_clear(handle_table[handle/HANDLE_TABLE_BLOCK_ITEMS].bmp_base,
			handle%HANDLE_TABLE_BLOCK_ITEMS);
	return;
}
