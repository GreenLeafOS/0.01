/*
 * page.h
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#ifndef PAGE_H_
#define PAGE_H_

#include "Type.h"

#define MAX_PAGE_ENTRY	1024
#define PAGE_SIZE		4096

/* 页表页目录项结构 */
typedef struct page_entry
{
	u32 p			: 1;			/* P位 Bit0 */
	u32 rw			: 1;			/* R/W位 Bit1*/
	u32 us			: 1;			/* U/S位 Bit2*/
	u32	pwt			: 1;			/* 缓冲策略位 Bit3*/
	u32 pcd			: 1;			/* 缓冲许可位 Bit4*/
	u32 a			: 1;			/* A位 Bit5*/
	u32 d			: 1;			/* D位 Bit6*/
	u32	ps			: 1;			/* 页大小 Bit7*/
	u32 g			: 1;			/* 指示全局页 Bit8*/
	u32 avl			: 3;			/* 系统使用保留位 Bit9~Bit11*/
	u32 addr		: 20;			/* 物理地址高20位 Bit12~Bit31*/
}PageEntry;

/*页表页目录结构*/
typedef struct page_table
{
	PageEntry items[PAGE_SIZE/sizeof(PageEntry)];
}PageTable;


/* 初始化 */
void page_init();


/* 页目录链接页表（页表映射到页目录） */
void page_link_table(PageTable* page_dir,PageTable* page_tbl,u16 linear_addr);
/* 页表链接地址（线性地址映射到物理地址） */
void page_link_addr(PageTable* page_tbl,u32 phy_page_addr,u16 linear_addr);
/* 直接映射(从页目录表的项中获取页表地址) */
void page_link(PageTable* page_dir,u32 phy_page_addr,u32 linear_addr);


/* 加载页目录 */
void page_directory_load(u32 phy_addr);

#endif /* PAGE_H_ */
