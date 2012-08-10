/*===============================================================
 *                       gdt.c
 *                	  GDT��ش�����
 ===============================================================*/
#include "include/table.h"

/************************************************************************/
/*							������
/************************************************************************/
Desc	gdt_table[32];
u8 		gdt_ptr[6];

/************************************************************************/
/*							��ʼ��
/*                           init
/************************************************************************/
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

/************************************************************************/
/*					   gdt�����������ʱ��������
/*                            gdt
/************************************************************************/


/****************************
 * ���
 * avl��־λΪ1��Ϊռ�ã�Ϊ0�����
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
 * ɾ��
 * ���avlλ����
 ****************************/
void gdt_remove(int index)
{
	Desc* buf = &gdt_table[index];
	buf->avl = 0;
}



/****************************
 * �޸���
 ****************************/
void gdt_set(int index,Desc item)
{
	Desc* buf = &gdt_table[index];
	*buf = item;
	buf->avl = 1;
}



/****************************
 * ��ȡ��
 ****************************/
void gdt_get(int index,Desc *item)
{
	Desc* buf = &gdt_table[index];
	*item = *buf;
}



/************************************************************************/
/*					 		���� gdt
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

