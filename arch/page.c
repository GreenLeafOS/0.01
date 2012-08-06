/*
 * page.c
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#include "page.h"

void page_init()
{

}

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
 * */




/*
 * 页目录链接页表（页表映射到页目录）
 * 参数：
 * 		page_dir 页目录表指针
 * 		page_tbl 页表指针
 * 		linear_addr	页表映射到的高位的10位地址,取低10位，高位丢弃。
 * */
void page_link_table(PageTable* page_dir,PageTable* page_tbl,u16 linear_addr)
{
	page_dir->items[linear_addr & 0x3FF].addr = (u32)page_tbl;
}





/* 页表链接地址（线性地址映射到物理地址）
 * 参数：
 * 		page_tbl 页表指针
 * 		phy_page_addr 物理页的地址
 * 		linear_addr	页表映射到的高位的10位地址,取低10位，高位丢弃。
 * */
void page_link_addr(PageTable* page_tbl,u32 phy_page_addr,u16 linear_addr)
{
	page_tbl->items[linear_addr & 0x3FF].addr = phy_page_addr;
}





/* 直接映射(从页目录表的项中获取页表地址)
 * 参数：
 * 		page_dir 页目录表指针
 * 		phy_page_addr 物理页的地址
 * 		linear_addr	页表映射到的线性地址高位的10位地址,取低10位，高位丢弃。
 * */
void page_link(PageTable* page_dir,u32 phy_page_addr,u32 linear_addr)
{
	u16 addr_high = linear_addr >> 22;						/* 线性地址高10位 */
	u16 addr_mid =  (linear_addr >> 12) & 0x3FF;			/* 线性地址中10位 */

	u32 page_tbl_addr = page_dir->items[addr_high].addr;	/* 获取页表地址高20位 */
	PageTable* page_tbl = (PageTable*)(page_tbl_addr << 12);	/* 计算页表指针 */
	page_tbl->items[addr_mid].addr = phy_page_addr;			/* 映射 */
}




/* 加载页目录
 * 参数；
 * 		phy_addr 页目录的物理地址
 * */
void page_load(u32 phy_addr)
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


