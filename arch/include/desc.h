#ifndef DESC_H_
#define DESC_H_
/*===============================================================
 *                      desc.h
 *                	 ����������ز�������
 ===============================================================*/
#include <Type.h>

/* ����������ֵ˵�� */
#define	DA_32			0x4000	/* 32 λ��				*/
#define	DA_LIMIT_4K		0x8000	/* �ν�������Ϊ 4K �ֽ�	*/
#define	DA_DPL0			0x00	/* DPL = 0				*/
#define	DA_DPL1			0x20	/* DPL = 1				*/
#define	DA_DPL2			0x40	/* DPL = 2				*/
#define	DA_DPL3			0x60	/* DPL = 3				*/
/* �洢������������ֵ˵�� */
#define	DA_DR			0x90	/* ���ڵ�ֻ�����ݶ�����ֵ		*/
#define	DA_DRW			0x92	/* ���ڵĿɶ�д���ݶ�����ֵ		*/
#define	DA_DRWA			0x93	/* ���ڵ��ѷ��ʿɶ�д���ݶ�����ֵ	*/
#define	DA_C			0x98	/* ���ڵ�ִֻ�д��������ֵ		*/
#define	DA_CR			0x9A	/* ���ڵĿ�ִ�пɶ����������ֵ		*/
#define	DA_CCO			0x9C	/* ���ڵ�ִֻ��һ�´��������ֵ		*/
#define	DA_CCOR			0x9E	/* ���ڵĿ�ִ�пɶ�һ�´��������ֵ	*/
/* ϵͳ������������ֵ˵�� */
#define	DA_LDT			0x82	/* �ֲ��������������ֵ			*/
#define	DA_TaskGate		0x85	/* ����������ֵ				*/
#define	DA_386TSS		0x89	/* ���� 386 ����״̬������ֵ	*/
#define	DA_386CGate		0x8C	/* 386 ����������ֵ			*/
#define	DA_386IGate		0x8E	/* 386 �ж�������ֵ			*/
#define	DA_386TGate		0x8F	/* 386 ����������ֵ			*/


// ����Ϊ�ֽڵĶ���
typedef struct s_desc		/* �� 8 ���ֽ� */
{
	u16	limit_low;			/* Limit */
	u16	base_low;			/* Base */
	u8	base_mid;			/* Base */
	u8	attr1;				/* P(1) DPL(2) DT(1) TYPE(4) */
	u8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	u8	base_high;			/* Base */
}Desc;

// ����Ϊλ�Ķ���
typedef struct s_desc_bit	/* �� 8 ���ֽ� */
{
    u16 limit_low		: 16;			/* �γ���16λ Bit0~Bit15: 2���ֽڿ� */
    u16 base_low		: 16;			/* �λ�ַ��16λ Bit16~Bit31�� 16λ�� */
    u16 base_mid		: 8;			/* �λ�ַ31~39λ Bit31~Bit39�� 8λ�� */
    u16 type			: 4;			/* ������ Bit40~Bit43: 4λ��ʾ�ε����� */
    u16 s				: 1;			/* ϵͳ�� Bit44: 0 ��ʾϵͳ������ 1 ��ʾ�������� */
    u16 dpl				: 2;			/* ������Ȩ�� Bit45~Bit46: ���ʶ������Ȩ�� */
    u16 p				: 1;			/* Preset λ Bit47: ��ʾ�Ƿ��ڲ��������� */
    u16 limit_high 		: 4;			/* �γ���4λ Bit48~Bit51: �γ���20λ */
    u16 avl				: 1;			/* AVLλ Bit52: ��λ�����ʹ�ã���������ʹ�� */
    u16					: 1;			/* Bit53: ��λ��������ʹ�� */
    u16 d				: 1;			/* Dλ Bit54��ȱʡ������λָʾȱʡ�Ĳ�������С 1 ��ʾȱʡΪ32λ 0 ��ʾȱʡΪ16λ */
    u16 g				: 1;			/* Gλ Bit55������λ,0 ��ʾ����Ϊ�ֽ� 1 ��ʾ����Ϊ 4K �ֽ�*/
    u16 base_high		: 8;			/* �λ�ַ��8λ Bit56~Bit63: ��32λ�λ�ַ */
}DescBit;

typedef struct desc_table
{
	Desc desc_item[32];
}DescTable;

/* ���������Ĳ��� */
void desc_init(Desc * p_desc, u32 base, u32 limit, u16 attribute);
Desc desc_init(u32 base, u32 limit, u16 attribute);

/* ����gdtѡ���� */
u32 gdt_get_sel(int index,u8 rpl);

#endif /* DESC_H_ */
