/*
 * intr.s
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */

.extern		thread_schedule
.extern		save
.extern		intr_handle


.global		hwint00
.global		hwint01
.global		hwint02
.global		hwint03
.global		hwint04
.global		hwint05
.global		hwint06
.global		hwint07
.global		hwint08
.global		hwint09
.global		hwint10
.global		hwint11
.global		hwint12
.global		hwint13
.global		hwint14
.global		hwint15



INT_M_CTL		=	0x20	/* I/O port for interrupt controller         <Master> */
INT_M_CTLMASK	=	0x21	/* setting bits in this port disables ints   <Master> */
INT_S_CTL		=	0xA0	/* I/O port for second interrupt controller  <Slave> */
INT_S_CTLMASK	=	0xA1	/* setting bits in this port disables ints   <Slave> */

EOI				=	0x20
/*
 * 主8259A的中断
 */
.macro hwint_master irq_num
	call	save					/* 保存上下文 */

	inb		$INT_M_CTLMASK,%al
	orb		$(1 << \irq_num),%al
	outb	%al,$INT_M_CTLMASK		/* 屏蔽当前中断 */

	movb	$EOI,%al
	outb	%al,$INT_M_CTL			/* 重新打开主8259A */

	sti								/* 开中断 */
	pushl	\irq_num				/* irq */
	call	intr_handle
	popl	%ecx					/* 调用者清理堆栈 */
	cli								/* 关中断 */

	pushl	%eax					/* 压入restart断点(由intr_handle 返回) */

	inb		$INT_M_CTLMASK,%al
	andb	$(~(1 << \irq_num)),%al
	outb	%al,$INT_M_CTLMASK		/* 恢复相应当前中断 */

	jmp		thread_schedule			/* 调用调度函数 */
.endm



/*
 * 从8259A的中断
 */
.macro hwint_slave irq_num
	call	save					/* 保存上下文 */

	inb		$INT_S_CTLMASK,%al
	orb		(1 << \irq_num),%al
	outb	%al,$INT_S_CTLMASK		/* 屏蔽当前中断 */

	movb	$EOI,%al
	outb	%al,$INT_M_CTL			/* 重新打开主8259A */
	nop
	outb	%al,$INT_S_CTL			/* 重新打开从8259A */

	sti								/* 开中断 */
	pushl	\irq_num				/* irq */
	call	intr_handle
	popl	%ecx					/* 调用者清理堆栈 */
	cli								/* 关中断 */

	pushl	%eax					/* 压入restart断点(由intr_handle 返回) */

	inb		$INT_M_CTLMASK,%al
	andb	$(~(1 << \irq_num)),%al
	outb	%al,$INT_M_CTLMASK		/* 恢复相应当前中断 */

	jmp		thread_schedule			/* 调用调度函数 */
.endm


/*
 * irq0 - irq7 主8259A的8个中断
 */
.align	16
hwint00:		/* Interrupt routine for irq 0 (the clock). */
	hwint_master	0

.align	16
hwint01:		/* Interrupt routine for irq 1 (keyboard) */
	hwint_master	1

.align	16
hwint02:		/* Interrupt routine for irq 2 (cascade!) */
	hwint_master	2

.align	16
hwint03:		/* Interrupt routine for irq 3 (second serial) */
	hwint_master	3

.align	16
hwint04:		/* Interrupt routine for irq 4 (first serial) */
	hwint_master	4

.align	16
hwint05:		/* Interrupt routine for irq 5 (XT winchester) */
	hwint_master	5

.align	16
hwint06:		/* Interrupt routine for irq 6 (floppy) */
	hwint_master	6

.align	16
hwint07:		/* Interrupt routine for irq 7 (printer) */
	hwint_master	7



/*
 * irq8 - irq15 从8259A的8个中断
 */
.align	16
hwint08:		/* Interrupt routine for irq 8 (realtime clock) */
	hwint_slave	8

.align	16
hwint09:		/* Interrupt routine for irq 9 (irq 2 redirected) */
	hwint_slave	9

.align	16
hwint10:		/* Interrupt routine for irq 10 */
	hwint_slave	10

.align	16
hwint11:		/* Interrupt routine for irq 11 */
	hwint_slave	11

.align	16
hwint12:		/* Interrupt routine for irq 12 */
	hwint_slave	12

.align	16
hwint13:		/* Interrupt routine for irq 13 (FPU exception) */
	hwint_slave	13

.align	16
hwint14:		/* Interrupt routine for irq 14 (AT winchester) */
	hwint_slave	14

.align	16
hwint15:		/* Interrupt routine for irq 15 */
	hwint_slave	15
