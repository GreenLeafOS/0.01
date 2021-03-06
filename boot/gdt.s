/*===============================================================
 *                     	 gdt.s
 *					gdt相关常量、宏
 ===============================================================*/
/************************************************************************/
/*                     	   常量
/************************************************************************/
/*
; 描述符类型值说明
; 其中:
;       DA_  : Descriptor Attribute
;       D    : 数据段
;       C    : 代码段
;       S    : 系统段
;       R    : 只读
;       RW   : 读写
;       A    : 已访问
;       其它 : 可按照字面意思理解
*/
DA_32		=	0x4000		/* 32 位段 */
DA_LIMIT_4K	=	0x8000		/* 段界限粒度为 4K 字节 */
/* DPL值 */
DA_DPL0		=	0x00
DA_DPL1		=	0x20
DA_DPL2		=	0x40
DA_DPL3		=	0x60
/* 存储段描述符类型值说明 */
DA_DR		=	0x90		/* 存在的只读数据段类型值 */
DA_DRW		=	0x92		/* 存在的可读写数据段属性值 */
DA_DRWA		=	0x93		/* 存在的已访问可读写数据段类型值 */
DA_C		=	0x98		/* 存在的只执行代码段属性值 */
DA_CR		=	0x9A		/* 存在的可执行可读代码段属性值 */
DA_CCO		=	0x9C		/* 存在的只执行一致代码段属性值 */
DA_CCOR		=	0x9E		/* 存在的可执行可读一致代码段属性值 */
/* 系统段描述符类型值说明 */
DA_LDT		=	0x82		/* 局部描述符表段类型值 */
DA_TaskGate	=	0x85		/* 任务门类型值 */
DA_386TSS	=	0x89		/* 可用 386 任务状态段类型值 */
DA_386CGate	=	0x8C		/* 386 调用门类型值 */
DA_386IGate	=	0x8E		/* 386 中断门类型值 */
DA_386TGate	=	0x8F		/* 386 陷阱门类型值 */
/*
; 选择子类型值说明
; 其中:
; 		SA_  : Selector Attribute
*/
SA_RPL0		=	0x0
SA_RPL1		=	0x1
SA_RPL2		=	0x2
SA_RPL3		=	0x3

SA_TIG		=	0x0
SA_TIL		=	0x4

/************************************************************************/
/*                     	   宏
/************************************************************************/
/*
;
; 描述符
; usage: Descriptor Base, Limit, Attr
;        Base:  dd
;        Limit: dd (low 20 bits available)
;        Attr:  dw (lower 4 bits of higher byte are always 0)
*/
.macro Descriptor base,limit,attr
	.word \limit & 0x0FFFF			/* 段界限 1				(2 字节) */
	.word \base & 0x0FFFF			/* 段基址 1				(2 字节) */
	.byte (\base >> 16) & 0x0FF		/* 段基址 2				(1 字节) */
	.word ((\limit >> 8) & 0x0F00) | (\attr & 0x0F0FF)	/* 属性 1 + 段界限 2 + 属性 2		(2 字节) */
	.byte (\base >> 24) & 0x0FF		/* 段基址 3				(1 字节) */
.endm
