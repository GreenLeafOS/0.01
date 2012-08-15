/*
 * init.c
 *
 *  Created on: 2012-8-15
 *      Author: greenleaf
 */



#include "include/kernel.h"

#include "arch/include/desc.h"
#include "arch/include/page.h"
#include "arch/include/table.h"

/*
 * ��ʼ���ڴ����
 * ����ڴ��С
 * ����λͼ��С
 * λͼ��һ��λ��ʾһ��ҳ��ʹ�����
 * ��һ����ɱ�ʾ32��ҳ��32*4096/1024 = 128 Kb = 1/8 M
 * 8��������� 128*8/1024 = 1 M
 */

void mem_init()
{
	/* ����ڴ��С */
	for(int i=0;i<mem_mcr_number;i++)
	{
		if(mem_info[i].Type == 1)
			if (mem_info[i].BaseAddrLow + mem_info[i].LenthLow > mem_size)
				mem_size = mem_info[i].BaseAddrLow + mem_info[i].LenthLow;
	}

	/* ����λͼ��С */
	mem_used_map_max = (mem_size/PAGE_SIZE)/32 + 1;
	mem_size = mem_used_map_max*32*PAGE_SIZE;

	/* ��ָ�븳ֵ */
	mem_used_map =(u32*)0x16000;

	/* ����4M���ں˿ռ� */
	for(int i=0;i<KERNEL_USED_MEM_ITEM;i++)
		*(mem_used_map + i) = ~0;
}








/*
 * ��ʼ����ҳ
 * �����Ե�ӳ��
 */
void page_init()
{
	/* ҳ����С */
	int page_table_size = B_TO_NEED_TABLE(mem_size);

	/* ���ѭ����ѭ������ҳ����ҳĿ¼������ */
	for(int i=0;i<page_table_size;i++)
	{
		u32 page_tbl = 0x400000+(i*PAGE_SIZE);

		((PageTable*)0x3ff000)->items[i].p = 1;	// pde��Pλ����Ϊ1
		page_link_table(0x3ff000,page_tbl,i);	// pde��addr����Ϊҳ����ַ

		/* �ڲ�ѭ����ѭ������������ַ��ҳ�������� */
		for(int j=0;j<1024;j++)
		{
			((PageTable*)page_tbl)->items[j].p = 1;		// pte��Pλ����Ϊ1
			page_link_addr(page_tbl,(i*MAX_PAGE_ENTRY+j)*PAGE_SIZE,j);	// pte��addr����Ϊ����ҳ���ַ
		}
	}

	/* ����ҳ��ʹ�õ�ҳ */
	for (int i=0;i<(page_table_size/32);i++)
	{
		*(mem_used_map+i+KERNEL_USED_MEM_ITEM) = 0xffffffff;
	}

	/* ���� */
	page_directory_load((u32)(0x3ff000));
}







/*
 * ��ʼ��gdt
 */

void gdt_init()
{
	gdt_add(desc_create(0,			0xfffff,	DA_CR|DA_32|DA_LIMIT_4K));				// ϵͳ�����
	gdt_add(desc_create(0,			0xfffff,	DA_DRW|DA_32|DA_LIMIT_4K));				// ϵͳ���ݶ�
	gdt_add(desc_create(0,			0xfffff,	DA_CR|DA_32|DA_LIMIT_4K|DA_DPL3));		// �û������
	gdt_add(desc_create(0,			0xfffff,	DA_DRW|DA_32|DA_LIMIT_4K|DA_DPL3));		// �û����ݶ�
	gdt_add(desc_create(0xb8000,	0xfffff,	DA_DRW|DA_DPL3));						// �Դ��

	// ����GDT
	gdt_load();
}








/*
 * ��ʼ��tss
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