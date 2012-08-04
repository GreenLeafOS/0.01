/*===============================================================
 *                       gdt.c
 *                	  GDT相关处理函数
 ===============================================================*/
#include "gdt.h"

/************************************************************************/
/*							数据区
/************************************************************************/
Desc 	gdt_table[32][2];
u8 		gdt_tmp;
u8 		gdt_ptr[6];

/************************************************************************/
/*							初始化
/*                           init
/************************************************************************/
void gdt_init()
{
	gdt[0].Init(0,0,0);		// 空描述符
	gdt_add(desc_init(0,0xfffff,DA_CR|DA_32|DA_LIMIT_4K));		// 系统代码段
	gdt_add(desc_init(0,0xfffff,DA_DRW|DA_32|DA_LIMIT_4K));		// 系统数据段
	gdt_add(desc_init(0,0xfffff,DA_CR|DA_32|DA_LIMIT_4K|DA_DPL3));		// 用户代码段
	gdt_add(desc_init(0,0xfffff,DA_DRW|DA_32|DA_LIMIT_4K|DA_DPL3));		// 用户数据段

	// 加载GDT
	gdt_ref();
	return;
}

/************************************************************************/
/*					   gdt操作（针对临时缓冲区）
/*                            gdt
/************************************************************************/

/*
 * 添加
 * avl标志位为1则为占用，为0则空闲
 */
void gdt_add(Desc item)
{
	for(int i=1;i<32;i++)
	{
		DescBit* buf = (DescBit*)&gdt_table[i][gdt_tmp];
		if (!(buf->avl))
		{
			*buf = item;
			buf->avl = 1;
		}
	}
	return;
}

/*
 * 删除
 * 清除avl位即可
 */
void gdt_remove(int index)
{
	DescBit* buf = (DescBit*)&gdt_table[index][gdt_tmp];
	buf->avl = 0;
	return;
}

/*
 * 修改项
 */
void gdt_set(int index,Desc item)
{
	DescBit* buf = (DescBit*)&gdt_table[index][gdt_tmp];
	*buf = item;
	return;
}

/*
 * 读取项
 */
void gdt_get(int index,Desc *item)
{
	DescBit* buf = (DescBit*)&gdt_table[index][gdt_tmp];
	*item = *buf;
}
/************************************************************************/
/*					 		缓冲操作
/*                            gdt
/************************************************************************/
/*
 * 复制当前gdt里的值到缓冲区。
 */
void gdt_save()
{
	DescTable *tmp = (DescTable*)&gdt_table[0][gdt_tmp];
	DescTable *src = (DescTable*)&gdt_table[0][gdt_tmp % 1];
	*tmp = *src;
}

/*
 * 刷新gdt,也可以看作加载gdt
 * 建立了两个gdt，一个作为缓冲区，加载时加载另一个，他们是轮换的。
 * gdt_tmp作为下标，取0,1两个值，每次加载时反转(1变成0，0变成1)
 */
void gdt_ref()
{
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base  = (u32*)(&gdt_ptr[2]);
	char *pGdt = (char*)&gdt_ptr[0];

	*p_gdt_limit = 32 * sizeof(Desc) - 1;
	*p_gdt_base  = (u32)&gdt_table[0][gdt_tmp];

	gdt_tmp %= 1;				/* 下标反转 */

	asm volatile("lgdt %0"
				:
				: "=m"(pGdt)
				);
}
