#ifndef DESC_H_
#define DESC_H_
/*===============================================================
 *                      desc.h
 *                	 段描述符相关操作函数
 ===============================================================*/
#include <Type.h>

/* 描述符类型值说明 */
#define	DA_32			0x4000	/* 32 位段				*/
#define	DA_LIMIT_4K		0x8000	/* 段界限粒度为 4K 字节	*/
#define	DA_DPL0			0x00	/* DPL = 0				*/
#define	DA_DPL1			0x20	/* DPL = 1				*/
#define	DA_DPL2			0x40	/* DPL = 2				*/
#define	DA_DPL3			0x60	/* DPL = 3				*/
/* 存储段描述符类型值说明 */
#define	DA_DR			0x90	/* 存在的只读数据段类型值		*/
#define	DA_DRW			0x92	/* 存在的可读写数据段属性值		*/
#define	DA_DRWA			0x93	/* 存在的已访问可读写数据段类型值	*/
#define	DA_C			0x98	/* 存在的只执行代码段属性值		*/
#define	DA_CR			0x9A	/* 存在的可执行可读代码段属性值		*/
#define	DA_CCO			0x9C	/* 存在的只执行一致代码段属性值		*/
#define	DA_CCOR			0x9E	/* 存在的可执行可读一致代码段属性值	*/
/* 系统段描述符类型值说明 */
#define	DA_LDT			0x82	/* 局部描述符表段类型值			*/
#define	DA_TaskGate		0x85	/* 任务门类型值				*/
#define	DA_386TSS		0x89	/* 可用 386 任务状态段类型值	*/
#define	DA_386CGate		0x8C	/* 386 调用门类型值			*/
#define	DA_386IGate		0x8E	/* 386 中断门类型值			*/
#define	DA_386TGate		0x8F	/* 386 陷阱门类型值			*/


// 粒度为字节的定义
typedef struct s_desc		/* 共 8 个字节 */
{
	u16	limit_low;			/* Limit */
	u16	base_low;			/* Base */
	u8	base_mid;			/* Base */
	u8	attr1;				/* P(1) DPL(2) DT(1) TYPE(4) */
	u8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	u8	base_high;			/* Base */
}Desc;

// 粒度为位的定义
typedef struct s_desc_bit	/* 共 8 个字节 */
{
    u16 limit_low		: 16;			/* 段长低16位 Bit0~Bit15: 2个字节宽 */
    u16 base_low		: 16;			/* 段基址低16位 Bit16~Bit31： 16位宽 */
    u16 base_mid		: 8;			/* 段基址31~39位 Bit31~Bit39： 8位宽 */
    u16 type			: 4;			/* 段类型 Bit40~Bit43: 4位表示段的类型 */
    u16 s				: 1;			/* 系统段 Bit44: 0 表示系统描述符 1 表示段描述符 */
    u16 dpl				: 2;			/* 描述符权限 Bit45~Bit46: 访问段所需的权限 */
    u16 p				: 1;			/* Preset 位 Bit47: 表示是否在不在主存中 */
    u16 limit_high 		: 4;			/* 段长高4位 Bit48~Bit51: 段长共20位 */
    u16 avl				: 1;			/* AVL位 Bit52: 这位被软件使用，处理器不使用 */
    u16					: 1;			/* Bit53: 此位保留不被使用 */
    u16 d				: 1;			/* D位 Bit54：缺省操作数位指示缺省的操作数大小 1 表示缺省为32位 0 表示缺省为16位 */
    u16 g				: 1;			/* G位 Bit55：粒度位,0 表示粒度为字节 1 表示粒度为 4K 字节*/
    u16 base_high		: 8;			/* 段基址高8位 Bit56~Bit63: 共32位段基址 */
}DescBit;

typedef struct desc_table
{
	Desc desc_item[32];
}DescTable;

/* 段描述符的操作 */
void desc_init(Desc * p_desc, u32 base, u32 limit, u16 attribute);
Desc desc_init(u32 base, u32 limit, u16 attribute);

/* 生成gdt选择子 */
u32 gdt_get_sel(int index,u8 rpl);

#endif /* DESC_H_ */
