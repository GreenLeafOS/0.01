/*
 * phypage.h
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#ifndef PHYPAGE_H_
#define PHYPAGE_H_
#include "lib.h"



/* PhyPage */
typedef struct phy_page
{
	ListNode node;
	u32 flags;
	u32 private;			/* 计数器为0，则页处于块中间，块起始页的计数器保存块大小 */
}PhyPage;


/* PhyPage.flags bit */
#define	PG_BUSY			0	/* 块正在使用,起始页有效 */



/* free_area */
struct free_area
{
	ListHead free_list;
	int nr_free;
};


/* zone */
struct zone_t
{
	struct free_area free_area[11];
	int free_pages;
	int page_count;
	PhyPage* page_base;
};

/* phypage data */
extern struct zone_t zone;

/* phypage function */
void 	buddy_init();
int		page_search(int order);
PhyPage* page_alloc(int order);
void 	page_free(PhyPage* page,int order);
Bool	page_is_buddy(PhyPage *page, int order);


/* phypage macro */
#define Page_To_Index(page) 	(((PhyPage*)page) - zone.page_base)
#define Page_To_Addr(page) 		(Page_To_Index(page) * PAGE_SIZE)


#define Addr_To_Index(addr) 	(((u32)addr)/PAGE_SIZE)
#define Addr_To_Page(addr)		(PhyPage*)(zone.page_base + (Addr_To_Index(addr)))


#define Index_To_Addr(index)	(index * PAGE_SIZE)
#define Index_To_Page(index)	(PhyPage*)(zone.page_base + index)


#endif /* PHYPAGE_H_ */
