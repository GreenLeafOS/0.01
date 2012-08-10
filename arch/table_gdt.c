/*===============================================================
 *                       gdt.c
 *                	  GDT相关处理函数
 ===============================================================*/
#include "include/table.h"

/************************************************************************/
/*							数据区
/************************************************************************/
Desc	gdt_table[32];
u8 		gdt_ptr[6];

/************************************************************************/
/*							初始化
/*                           init
/************************************************************************/
void gdt_init()
{
	gdt_add(desc_create(0,			0xfffff,	DA_CR|DA_32|DA_LIMIT_4K));				// 系统代码段
	gdt_add(desc_create(0,			0xfffff,	DA_DRW|DA_32|DA_LIMIT_4K));				// 系统数据段
	gdt_add(desc_create(0,			0xfffff,	DA_CR|DA_32|DA_LIMIT_4K|DA_DPL3));		// 用户代码段
	gdt_add(desc_create(0,			0xfffff,	DA_DRW|DA_32|DA_LIMIT_4K|DA_DPL3));		// 用户数据段
	gdt_add(desc_create(0xb8000,	0xfffff,	DA_DRW|DA_DPL3));						// 显存段

	// 加载GDT
	gdt_load();
}

/************************************************************************/
/*					   gdt操作（针对临时缓冲区）
/*                            gdt
/************************************************************************/


/****************************
 * 添加
 * avl标志位为1则为占用，为0则空闲
 ****************************/
int gdt_add(Desc item)
{
	for(int i=1;i<32;i++)
	{
		Desc* buf = &gdt_table[i];
		if (!(buf->avl))
		{
			gdt_set(i,item);
			return i;
		}
	}
	return -1;
}



/****************************
 * 删除
 * 清除avl位即可
 ****************************/
void gdt_remove(int index)
{
	Desc* buf = &gdt_table[index];
	buf->avl = 0;
}



/****************************
 * 修改项
 ****************************/
void gdt_set(int index,Desc item)
{
	Desc* buf = &gdt_table[index];
	*buf = item;
	buf->avl = 1;
}



/****************************
 * 读取项
 ****************************/
void gdt_get(int index,Desc *item)
{
	Desc* buf = &gdt_table[index];
	*item = *buf;
}



/************************************************************************/
/*					 		加载 gdt
/*                            gdt
/************************************************************************/
void gdt_load()
{
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base  = (u32*)(&gdt_ptr[2]);
	char *pGdt = (char*)&gdt_ptr[0];

	*p_gdt_limit = 32 * sizeof(Desc) - 1;
	*p_gdt_base  = (u32)&gdt_table[0];

	__asm volatile(
			"movl %0,%%eax		\n"
			"lgdt (%%eax)"
			:
			: "g"(pGdt));
}

