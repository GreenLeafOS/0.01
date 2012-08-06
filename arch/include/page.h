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

/* ҳ��ҳĿ¼��ṹ */
typedef struct page_entry
{
	u32 p			: 1;			/* Pλ Bit0 */
	u32 rw			: 1;			/* R/Wλ Bit1*/
	u32 us			: 1;			/* U/Sλ Bit2*/
	u32	pwt			: 1;			/* �������λ Bit3*/
	u32 pcd			: 1;			/* �������λ Bit4*/
	u32 a			: 1;			/* Aλ Bit5*/
	u32 d			: 1;			/* Dλ Bit6*/
	u32	ps			: 1;			/* ҳ��С Bit7*/
	u32 g			: 1;			/* ָʾȫ��ҳ Bit8*/
	u32 avl			: 3;			/* ϵͳʹ�ñ���λ Bit9~Bit11*/
	u32 addr		: 20;			/* �����ַ��20λ Bit12~Bit31*/
}PageEntry;

/*ҳ��ҳĿ¼�ṹ*/
typedef struct page_table
{
	PageEntry items[PAGE_SIZE/sizeof(PageEntry)];
}PageTable;


/* ��ʼ�� */
void page_init();


/* ҳĿ¼����ҳ��ҳ��ӳ�䵽ҳĿ¼�� */
void page_link_table(PageTable* page_dir,PageTable* page_tbl,u16 linear_addr);
/* ҳ�����ӵ�ַ�����Ե�ַӳ�䵽�����ַ�� */
void page_link_addr(PageTable* page_tbl,u32 phy_page_addr,u16 linear_addr);
/* ֱ��ӳ��(��ҳĿ¼������л�ȡҳ���ַ) */
void page_link(PageTable* page_dir,u32 phy_page_addr,u32 linear_addr);


/* ����ҳĿ¼ */
void page_directory_load(u32 phy_addr);

#endif /* PAGE_H_ */
