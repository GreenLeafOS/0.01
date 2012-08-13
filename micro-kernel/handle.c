/*
 * handle.c
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */


#include "include/kernel.h"


/* 中断异常处理函数 */
void	divide_error();
void	single_step_exception();
void	nmi();
void	breakpoint_exception();
void	overflow();
void	bounds_check();
void	inval_opcode();
void	copr_not_available();
void	double_fault();
void	copr_seg_overrun();
void	inval_tss();
void	segment_not_present();
void	stack_exception();
void	general_protection();
void	page_fault();
void	copr_error();

void	hwint00();
void	hwint01();
void	hwint02();
void	hwint03();
void	hwint04();
void	hwint05();
void	hwint06();
void	hwint07();
void	hwint08();
void	hwint09();
void	hwint10();
void	hwint11();
void	hwint12();
void	hwint13();
void	hwint14();
void	hwint15();



int	err_code;
int ret_addr;

/*
 * 初始化中断异常
 * */
void idt_init()
{
	// 全部初始化成中断门
	idt_init_intr(MSG_EXCP_DE,		divide_error,			0);
	idt_init_intr(MSG_EXCP_DB,		single_step_exception,	0);
	idt_init_intr(MSG_EXCP_NMI,		nmi,					0);
	idt_init_intr(MSG_EXCP_BP,		breakpoint_exception,	0);
	idt_init_intr(MSG_EXCP_OF,		overflow,				0);
	idt_init_intr(MSG_EXCP_BR,		bounds_check,			0);
	idt_init_intr(MSG_EXCP_UD,		inval_opcode,			0);
	idt_init_intr(MSG_EXCP_NM,		copr_not_available,		0);
	idt_init_intr(MSG_EXCP_DF,		double_fault,			0);
	idt_init_intr(MSG_EXCP_9,		copr_seg_overrun,		0);
	idt_init_intr(MSG_EXCP_TS,		inval_tss,				0);
	idt_init_intr(MSG_EXCP_SS,		segment_not_present,	0);
	idt_init_intr(MSG_EXCP_GP,		general_protection,		0);
	idt_init_intr(MSG_EXCP_PF,		page_fault,				0);
	idt_init_intr(MSG_EXCP_MF,		copr_error,				0);

	idt_init_intr(MSG_INTR + 0,		hwint00,				0);
	idt_init_intr(MSG_INTR + 1,		hwint01,				0);
	idt_init_intr(MSG_INTR + 2,		hwint02,				0);
	idt_init_intr(MSG_INTR + 3,		hwint03,				0);
	idt_init_intr(MSG_INTR + 4,		hwint04,				0);
	idt_init_intr(MSG_INTR + 5,		hwint05,				0);
	idt_init_intr(MSG_INTR + 6,		hwint06,				0);
	idt_init_intr(MSG_INTR + 7,		hwint07,				0);
	idt_init_intr(MSG_INTR + 8,		hwint08,				0);
	idt_init_intr(MSG_INTR + 9,		hwint09,				0);
	idt_init_intr(MSG_INTR + 10,	hwint10,				0);
	idt_init_intr(MSG_INTR + 11,	hwint11,				0);
	idt_init_intr(MSG_INTR + 12,	hwint12,				0);
	idt_init_intr(MSG_INTR + 13,	hwint13,				0);
	idt_init_intr(MSG_INTR + 14,	hwint14,				0);
	idt_init_intr(MSG_INTR + 15,	hwint15,				0);
}



/*
 * 中断处理函数
 * 参数：
 * 		irq_num	中断号
 * 返回值：对应restart的地址
 * */
FunAddr intr_handle(int irq_num)
{
	return restart;
}


/*
 * 异常处理函数
 * 参数：
 * 		vec_num 异常向量号
 * 		err_code 错误码
 * 返回值：对应restart的地址
 * */
FunAddr exception_handler(int vec_num, int err_code)
{
	return restart;
}
