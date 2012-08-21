/*
 * mod_addr_room.c
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */


#include "include/sysapi.h"
#include "include/module.h"


typedef struct linear_block_object
{
	u32 	flags;			// 线性空间块状态、类型
	Handle	objs[4];		// 一个线性块包含4个资源句柄,可能物理页框描述符，
							// 也可能是其他块设备描述符，这使得文件和虚拟内存在内核看来都是一样的
}MemObj;




typedef struct linear_room
{
	ListHead mem_obj;		// 线性空间块的双向链表头指针
	u32 free_count;			// 剩余的空间还有多少页框
};
