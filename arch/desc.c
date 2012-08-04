/*===============================================================
 *                      desc.c
 *                	 ����������ز�������
 ===============================================================*/
#include "desc.h"

void desc_init(Desc* p_desc,u32 base,u32 limit,u16 attribute)
{
	p_desc->limit_low		= limit & 0x0FFFF;			// �ν��� 1		(2 �ֽ�)
	p_desc->base_low		= base & 0x0FFFF;			// �λ�ַ 1		(2 �ֽ�)
	p_desc->base_mid		= (base >> 16) & 0x0FF;		// �λ�ַ 2		(1 �ֽ�)
	p_desc->attr1			= attribute & 0xFF;			// ���� 1
	p_desc->limit_high_attr2	= ((limit >> 16) & 0x0F) |
						(attribute >> 8) & 0xF0;		// �ν��� 2 + ���� 2
	p_desc->base_high		= (base >> 24) & 0x0FF;		// �λ�ַ 3		(1 �ֽ�)
}

Desc desc_init(u32 base, u32 limit, u16 attribute)
{
	Desc new_desc;
	desc_init(&desc,base,limit,attribute);
	return new_desc;
}

u32 gdt_get_sel(int index,u8 rpl)
{
	return (index<<3 + rpl);
}
