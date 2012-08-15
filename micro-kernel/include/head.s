/*
 * head.s
 *
 *  Created on: 2012-8-14
 *      Author: greenleaf
 */

INT_M_CTL		=	0x20	/* I/O port for interrupt controller         <Master> */
INT_M_CTLMASK	=	0x21	/* setting bits in this port disables ints   <Master> */
INT_S_CTL		=	0xA0	/* I/O port for second interrupt controller  <Slave> */
INT_S_CTLMASK	=	0xA1	/* setting bits in this port disables ints   <Slave> */

EOI				=	0x20

/* thread.c */
.extern		thread_schedule
.extern		thread_sleep_id
.extern		thread_sleep

.extern 	thread_run_stack_top
.extern		thread_run

/* save.s */
.extern		save

/* handle.c */
.extern		intr_handle
.extern		exception_handler
.extern 	ret_addr
.extern		err_code

/* kernel.c */
.extern		kernel_reenter
.extern		kernel_stack_p

sel_data		=			16



/*
 * 主8259A的中断
 */
.macro hwint_master irq_num
	/* 如果没有重入，则保存寄存器在线程的内核态栈 */
	/* 如果重入，则保存在内核栈 */
	/* save函数会压入一个返回地址 */
	/* 如果重入，则是restart_reenter */
	/* 如果没重入，则压入restart */
	/* 压入的返回函数作为由ret返回 */
	call	save

	/* 屏蔽当前中断 */
	inb		$INT_M_CTLMASK,%al
	orb		$(1 << \irq_num),%al
	outb	%al,$INT_M_CTLMASK

	/* 重新打开主8259A */
	movb	$EOI,%al
	outb	%al,$INT_M_CTL

	/* 调用中断处理函数 */
	pushl	$\irq_num				/* irq */
	call	intr_handle				/* 如果没有重入则调用调度函数。中断重入时不允许抢占 */
	popl	%ecx					/* 调用者清理堆栈 */

	/* 恢复响应当前中断 */
	inb		$INT_M_CTLMASK,%al
	andb	$(~(1 << \irq_num)),%al
	outb	%al,$INT_M_CTLMASK

	/* 根据栈顶地址进行跳转 */
	/* 如果中断重入，则返回压入的restart_reenter */
	/* 如果没有中断重入，则返回压入的restart */
	ret
.endm



/*
 * 从8259A的中断
 */
.macro hwint_slave irq_num
	call	save

	inb		$INT_S_CTLMASK,%al
	orb		$(1 << (\irq_num-8)),%al
	outb	%al,$INT_S_CTLMASK		/* 屏蔽当前中断 */

	movb	$EOI,%al
	outb	%al,$INT_M_CTL			/* 重新打开主8259A */
	nop
	outb	%al,$INT_S_CTL			/* 重新打开从8259A */

	sti								/* 开中断 */
	pushl	$\irq_num				/* irq */
	call	intr_handle
	popl	%ecx					/* 调用者清理堆栈 */
	cli								/* 关中断 */

	inb		$INT_M_CTLMASK,%al
	andb	$(~(1 << (\irq_num-8))),%al
	outb	%al,$INT_M_CTLMASK		/* 恢复相应当前中断 */

	ret
.endm



/*
 * 异常处理
 */
.macro exception vec_num
	popl	(err_code)
	call	save
	pushl	(err_code)
	pushl	\vec_num
	call	exception_handler
	addl	$8,%esp					/* 清理堆栈 */
	popl	%ecx
	pushl	%eax					/* 压入restart断点(由exception_handler 返回) */
	jmp		thread_schedule			/* 调用调度函数 */
.endm
