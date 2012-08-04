/*===============================================================
 *                       gdt.c
 *                	  GDT��ش�����
 ===============================================================*/
#include "gdt.h"

/************************************************************************/
/*							������
/************************************************************************/
Desc 	gdt_table[32][2];
u8 		gdt_tmp;
u8 		gdt_ptr[6];

/************************************************************************/
/*							��ʼ��
/*                           init
/************************************************************************/
void gdt_init()
{
	gdt[0].Init(0,0,0);		// ��������
	gdt_add(desc_init(0,0xfffff,DA_CR|DA_32|DA_LIMIT_4K));		// ϵͳ�����
	gdt_add(desc_init(0,0xfffff,DA_DRW|DA_32|DA_LIMIT_4K));		// ϵͳ���ݶ�
	gdt_add(desc_init(0,0xfffff,DA_CR|DA_32|DA_LIMIT_4K|DA_DPL3));		// �û������
	gdt_add(desc_init(0,0xfffff,DA_DRW|DA_32|DA_LIMIT_4K|DA_DPL3));		// �û����ݶ�

	// ����GDT
	gdt_ref();
	return;
}

/************************************************************************/
/*					   gdt�����������ʱ��������
/*                            gdt
/************************************************************************/

/*
 * ���
 * avl��־λΪ1��Ϊռ�ã�Ϊ0�����
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
 * ɾ��
 * ���avlλ����
 */
void gdt_remove(int index)
{
	DescBit* buf = (DescBit*)&gdt_table[index][gdt_tmp];
	buf->avl = 0;
	return;
}

/*
 * �޸���
 */
void gdt_set(int index,Desc item)
{
	DescBit* buf = (DescBit*)&gdt_table[index][gdt_tmp];
	*buf = item;
	return;
}

/*
 * ��ȡ��
 */
void gdt_get(int index,Desc *item)
{
	DescBit* buf = (DescBit*)&gdt_table[index][gdt_tmp];
	*item = *buf;
}
/************************************************************************/
/*					 		�������
/*                            gdt
/************************************************************************/
/*
 * ���Ƶ�ǰgdt���ֵ����������
 */
void gdt_save()
{
	DescTable *tmp = (DescTable*)&gdt_table[0][gdt_tmp];
	DescTable *src = (DescTable*)&gdt_table[0][gdt_tmp % 1];
	*tmp = *src;
}

/*
 * ˢ��gdt,Ҳ���Կ�������gdt
 * ����������gdt��һ����Ϊ������������ʱ������һ�����������ֻ��ġ�
 * gdt_tmp��Ϊ�±꣬ȡ0,1����ֵ��ÿ�μ���ʱ��ת(1���0��0���1)
 */
void gdt_ref()
{
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base  = (u32*)(&gdt_ptr[2]);
	char *pGdt = (char*)&gdt_ptr[0];

	*p_gdt_limit = 32 * sizeof(Desc) - 1;
	*p_gdt_base  = (u32)&gdt_table[0][gdt_tmp];

	gdt_tmp %= 1;				/* �±귴ת */

	asm volatile("lgdt %0"
				:
				: "=m"(pGdt)
				);
}
