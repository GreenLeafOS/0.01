/*
 * handle.c
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */

#include "include/sysapi.h"
#include "include/handle.h"

/* handle data */
Handle*	handle_table[HANDLE_TABLE_BLCOK_MAX];




/*
 * SysApi new
 * 参数：句柄信息（不用填写id）
 * 功能：分配一个对象句柄
 * 返回值：句柄指针
 */
Handle*	new(Handle handle)
{
	/* 外层循环，查找表 */
	for(int i=0;i<HANDLE_TABLE_BLCOK_MAX;i++)
	{
		Handle *free = handle_table[i] ;
		if (free == NULL)
		{
			/* 为空表分配内存 */
			handle_table[i] = (Handle*)alloc(HANDLE_TABLE_BLOCK_SIZE);
		}
		/* 内层循环，查找空闲句柄 */
		for(int j=0;j< HANDLE_TABLE_ITEM_COUNT;j++)
		{
			if (free++->id == 0)
			{
				handle.id = (i*HANDLE_TABLE_ITEM_COUNT)+j;
				*(handle_table[i] + j) = handle;
				return (handle_table[i] + j);
			}
		}
	}
	return NULL;
}





/*
 * SysApi delete
 * 参数：句柄指针
 * 功能：释放句柄
 * 返回值：无
 */
void delete(Handle *handle)
{
	(handle_table[0] + handle->id)->id = NULL;
	return;
}





/*
 * SysApi set
 * 参数：无
 * 功能：写入句柄
 * 返回值：无
 */
void set(Handle handle)
{
	*(handle_table[0] + handle.id) = handle;
	return;
}




/*
 * SysApi get
 * 参数：无
 * 功能：获取句柄
 * 返回值：句柄指针
 */
Handle get(u32 id)
{
	return *(handle_table[0] + id);
}
