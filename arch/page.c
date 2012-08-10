/*
 * page.c
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#include "include/page.h"

/*
 * 在写代码之前，我们先算一笔账。
 * 表项: 4 b
 * 页目录:4 kb = 4096 b
 * 页表: 4 kb = 4096 b
 * 线性地址的高10位是页目录表的偏移
 * 因为表是4kb大，所以所以PDE在表中的偏移不会超过4kb,即4096b,所以只需要12位即可。
 * 因为表项是4b大，所以PDE在表中的偏移总会是4的倍数，即偏移的低2位总是0。现在看来，10位足够了。
 * 那么真正的16位偏移应该是 10位偏移左移2位，高位补0得到。
 * 反过来，10位线性偏移则等于 16位偏移右移，高位截断得到。
 * 这时的10位偏移恰好是数组的下标。（好好想想，就懂了）
 * 分页单元在寻址时，可以看作是执行了这么一个函数(C语言伪代码)
 * bits函数的作用是取相应位。bit10指10位数，以此类推。
 * u32 linear_to_phy(u32 linear)
 * {
 *		bit10 high = bits(linear,22,31);
 *		bit10 mid = bits(linear,12,21);
 *		bit12 low = bits(linear,0,11);
 *
 *		pde = page_dir->item[high];
 *		page_tbl = (pde->addr) << 12;
 *		pte = page_tbl->item[mid];
 *		phy_page = (pte->addr) << 12;
 *		phy_addr = phy_page + low;
 *		return phy_addr;
 * }
 *
 * 了解了这个，写起来是不是游刃有余了呢？
 * 调试命令：
 * 		x /256bx 0x16000
 * 		b 0x10bd4
 * 		b 0x10c07
 *
 * */



/*
 * ebp - 20 : *page_dir
 * ebp - 28 : i
 * ebp - 16 : *page_tbl
 * ebp - 24 : j
 * 参数
 * esp + 8  : i
 * esp + 4  : *page_tbl
 * esp + 0  : *page_dir
 * 参数
 * esp + 8  : j
 * esp + 4  : phy_addr
 * esp + 0  : *page_tbl
 */


/*
 * 页目录链接页表（页表映射到页目录）
 * 参数：
 * 		page_dir 页目录表指针
 * 		page_tbl 页表指针。取高20位
 * 		linear_index	页表映射到的高位的10位地址,取低10位，高位丢弃。
 * */
#define page_link_table(page_dir,page_tbl,linear_index)	\
	((PageTable*)page_dir)->items[(u16)linear_index & 0x3FF].addr = (u32)((page_tbl)>>12);


/* 页表链接地址（线性地址映射到物理地址）
 * 参数：
 * 		page_tbl 页表指针
 * 		phy_page_addr 物理页的地址.取高20位
 * 		linear_index	页表映射到的高位的10位地址,取低10位，高位丢弃。
 * */
#define page_link_addr(page_tbl,phy_page_addr,linear_index) \
	((PageTable*)page_tbl)->items[(u16)linear_index & 0x3FF].addr = (u32)((phy_page_addr)>>12);


/* 直接映射(从页目录表的项中获取页表地址)
 * 参数：
 * 		page_dir 页目录表指针
 * 		phy_page_addr 物理页的地址
 * 		linear_addr	页表映射到的线性地址高位的10位地址,取低10位，高位丢弃。
 * 	返回值：
 * 		0表示成功，-1表示页表不存在
 * */
int page_link(PageTable* page_dir,u32 phy_page_addr,u32 linear_addr)
{
	u16 addr_high = linear_addr >> 22;						/* 线性地址高10位 */
	u16 addr_mid =  (linear_addr >> 12) & 0x3FF;			/* 线性地址中10位 */

	if (page_dir->items[addr_high].p == 1) return -1;		/* 页表不存在 */

	u32 page_tbl_addr = page_dir->items[addr_high].addr;	/* 获取页表地址高20位 */
	PageTable* page_tbl = (PageTable*)(page_tbl_addr << 12);	/* 计算页表指针 */
	page_tbl->items[addr_mid].addr = phy_page_addr;			/* 映射 */

	return 0;
}


/*
 * 初始化
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


/* 加载页目录
 * 参数；
 * 		phy_addr 页目录的物理地址
 * */
void page_directory_load(u32 phy_addr)
{
	__asm volatile(
			"movl	%0,%%eax		\n"
			"movl	%%eax,%%cr3		\n"
			"movl	%%cr0,%%eax		\n"
			"orl	$0x80000000,%%eax	\n"
			"movl	%%eax,%%cr0		\n"
			: : "g"(phy_addr)
			);
}


