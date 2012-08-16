/*
 * init.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/sysapi.h"
#include "include/handle.h"

#include "arch/include/desc.h"
#include "arch/include/page.h"
#include "arch/include/table.h"

/*
 * 初始化内存管理
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
	for(int i=0;i<KERNEL_USED_MEM_ITEM;i++)
		*(mem_used_map + i) = ~0;
}







/*
 * 初始化分页
 * 建立对等映射
 */
void page_init()
{
	/* 页表大小 */
	int page_table_size = B_TO_NEED_TABLE(mem_size);

	/* 外层循环，循环建立页表和页目录的链接 */
	for(int i=0;i<page_table_size;i++)
	{
		u32 page_tbl = 0x400000+(i*PAGE_SIZE);

		((PageTable*)0x3ff000)->items[i].p = 1;	// pde项P位设置为1
		page_link_table(0x3ff000,page_tbl,i);	// pde项addr设置为页表地址

		/* 内层循环，循环建立物理地址与页表的链接 */
		for(int j=0;j<1024;j++)
		{
			((PageTable*)page_tbl)->items[j].p = 1;		// pte项P位设置为1
			page_link_addr(page_tbl,(i*MAX_PAGE_ENTRY+j)*PAGE_SIZE,j);	// pte项addr设置为物理页框地址
		}
	}

	/* 保留页表使用的页 */
	for (int i=0;i<(page_table_size/32);i++)
	{
		*(mem_used_map+i+KERNEL_USED_MEM_ITEM) = 0xffffffff;
	}

	/* 加载 */
	page_directory_load((u32)(0x3ff000));
}







/*
 * 初始化gdt
 */

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








/*
 * 初始化tss
 */
void tss_init()
{
	int i = gdt_add(desc_create((u32)&tss,sizeof(tss)-1,DA_386TSS));
	gdt_load();
	tss.iobase = sizeof(tss);
	tss.ss0 = gdt_get_sel(1,0);
	tss.esp0 = 0x80000;

	tss_sel = gdt_get_sel(i,0);
	tss_load(tss_sel);
}




/*
 * 初始化中断异常
 * */
void idt_init()
{
	// 全部初始化成中断门
	idt_init_intr(MSG_EXCP_DE,		divide_error,			0);
	idt_init_intr(MSG_EXCP_DB,		single_step_exception,	0);
	idt_init_intr(MSG_EXCP_NMI,		nmi,					0);
	idt_init_intr(MSG_EXCP_BP,		breakpoint_exception,	0);
	idt_init_intr(MSG_EXCP_OF,		overflow,				0);
	idt_init_intr(MSG_EXCP_BR,		bounds_check,			0);
	idt_init_intr(MSG_EXCP_UD,		inval_opcode,			0);
	idt_init_intr(MSG_EXCP_NM,		copr_not_available,		0);
	idt_init_intr(MSG_EXCP_DF,		double_fault,			0);
	idt_init_intr(MSG_EXCP_9,		copr_seg_overrun,		0);
	idt_init_intr(MSG_EXCP_TS,		inval_tss,				0);
	idt_init_intr(MSG_EXCP_SS,		segment_not_present,	0);
	idt_init_intr(MSG_EXCP_GP,		general_protection,		0);
	idt_init_intr(MSG_EXCP_PF,		page_fault,				0);
	idt_init_intr(MSG_EXCP_MF,		copr_error,				0);

	idt_init_intr(MSG_INTR + 0,		hwint00,				0);
	idt_init_intr(MSG_INTR + 1,		hwint01,				0);
	idt_init_intr(MSG_INTR + 2,		hwint02,				0);
	idt_init_intr(MSG_INTR + 3,		hwint03,				0);
	idt_init_intr(MSG_INTR + 4,		hwint04,				0);
	idt_init_intr(MSG_INTR + 5,		hwint05,				0);
	idt_init_intr(MSG_INTR + 6,		hwint06,				0);
	idt_init_intr(MSG_INTR + 7,		hwint07,				0);
	idt_init_intr(MSG_INTR + 8,		hwint08,				0);
	idt_init_intr(MSG_INTR + 9,		hwint09,				0);
	idt_init_intr(MSG_INTR + 10,	hwint10,				0);
	idt_init_intr(MSG_INTR + 11,	hwint11,				0);
	idt_init_intr(MSG_INTR + 12,	hwint12,				0);
	idt_init_intr(MSG_INTR + 13,	hwint13,				0);
	idt_init_intr(MSG_INTR + 14,	hwint14,				0);
	idt_init_intr(MSG_INTR + 15,	hwint15,				0);

	idt_load();
	return;
}


