/*
 * page.c
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#include "page.h"

/*
 * ��д����֮ǰ����������һ���ˡ�
 * ����: 4 b
 * ҳĿ¼:4 kb = 4096 b
 * ҳ��: 4 kb = 4096 b
 * ���Ե�ַ�ĸ�10λ��ҳĿ¼���ƫ��
 * ��Ϊ����4kb����������PDE�ڱ��е�ƫ�Ʋ��ᳬ��4kb,��4096b,����ֻ��Ҫ12λ���ɡ�
 * ��Ϊ������4b������PDE�ڱ��е�ƫ���ܻ���4�ı�������ƫ�Ƶĵ�2λ����0�����ڿ�����10λ�㹻�ˡ�
 * ��ô������16λƫ��Ӧ���� 10λƫ������2λ����λ��0�õ���
 * ��������10λ����ƫ������� 16λƫ�����ƣ���λ�ضϵõ���
 * ��ʱ��10λƫ��ǡ����������±ꡣ���ú����룬�Ͷ��ˣ�
 * ��ҳ��Ԫ��Ѱַʱ�����Կ�����ִ������ôһ������(C����α����)
 * bits������������ȡ��Ӧλ��bit10ָ10λ�����Դ����ơ�
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
 * �˽��������д�����ǲ��������������أ�
 * �������
 * 		x /256bx 0x16000
 * 		b 0x10bd4
 * 		b 0x10c07
 * */




void page_init()
{
	PageTable *page_dir =(PageTable*)mem_page_alloc();

	for(int i=0;i<MB_TO_PAGE(4);i++)
	{
		PageTable* page_tbl = (PageTable*)mem_page_alloc();
		page_link_table(page_dir,page_tbl,i);
		for(int j=0;j<1024;j++)
		{
			int phy_addr = (i * MAX_PAGE_ENTRY + j) * PAGE_SIZE;
			page_link_addr(page_tbl,phy_addr,j);
		}
	}
	page_directory_load((u32)(page_dir));
}

/*
 * ҳĿ¼����ҳ��ҳ��ӳ�䵽ҳĿ¼��
 * ������
 * 		page_dir ҳĿ¼��ָ��
 * 		page_tbl ҳ��ָ��
 * 		linear_addr	ҳ��ӳ�䵽�ĸ�λ��10λ��ַ,ȡ��10λ����λ������
 * */
void page_link_table(PageTable* page_dir,PageTable* page_tbl,u16 linear_addr)
{
	page_dir->items[linear_addr & 0x3FF].addr = (u32)page_tbl;
}





/* ҳ�����ӵ�ַ�����Ե�ַӳ�䵽�����ַ��
 * ������
 * 		page_tbl ҳ��ָ��
 * 		phy_page_addr ����ҳ�ĵ�ַ
 * 		linear_addr	ҳ��ӳ�䵽�ĸ�λ��10λ��ַ,ȡ��10λ����λ������
 * */
void page_link_addr(PageTable* page_tbl,u32 phy_page_addr,u16 linear_addr)
{
	page_tbl->items[linear_addr & 0x3FF].addr = phy_page_addr;
}





/* ֱ��ӳ��(��ҳĿ¼������л�ȡҳ���ַ)
 * ������
 * 		page_dir ҳĿ¼��ָ��
 * 		phy_page_addr ����ҳ�ĵ�ַ
 * 		linear_addr	ҳ��ӳ�䵽�����Ե�ַ��λ��10λ��ַ,ȡ��10λ����λ������
 * 	����ֵ��
 * 		0��ʾ�ɹ���-1��ʾҳ������
 * */
int page_link(PageTable* page_dir,u32 phy_page_addr,u32 linear_addr)
{
	u16 addr_high = linear_addr >> 22;						/* ���Ե�ַ��10λ */
	u16 addr_mid =  (linear_addr >> 12) & 0x3FF;			/* ���Ե�ַ��10λ */

	if (page_dir->items[addr_high].p == 1) return -1;		/* ҳ������ */

	u32 page_tbl_addr = page_dir->items[addr_high].addr;	/* ��ȡҳ���ַ��20λ */
	PageTable* page_tbl = (PageTable*)(page_tbl_addr << 12);	/* ����ҳ��ָ�� */
	page_tbl->items[addr_mid].addr = phy_page_addr;			/* ӳ�� */

	return 0;
}




/* ����ҳĿ¼
 * ������
 * 		phy_addr ҳĿ¼�������ַ
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


