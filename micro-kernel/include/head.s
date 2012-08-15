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
 * ��8259A���ж�
 */
.macro hwint_master irq_num
	/* ���û�����룬�򱣴�Ĵ������̵߳��ں�̬ջ */
	/* ������룬�򱣴����ں�ջ */
	/* save������ѹ��һ�����ص�ַ */
	/* ������룬����restart_reenter */
	/* ���û���룬��ѹ��restart */
	/* ѹ��ķ��غ�����Ϊ��ret���� */
	call	save

	/* ���ε�ǰ�ж� */
	inb		$INT_M_CTLMASK,%al
	orb		$(1 << \irq_num),%al
	outb	%al,$INT_M_CTLMASK

	/* ���´���8259A */
	movb	$EOI,%al
	outb	%al,$INT_M_CTL

	/* �����жϴ����� */
	pushl	$\irq_num				/* irq */
	call	intr_handle				/* ���û����������õ��Ⱥ������ж�����ʱ��������ռ */
	popl	%ecx					/* �����������ջ */

	/* �ָ���Ӧ��ǰ�ж� */
	inb		$INT_M_CTLMASK,%al
	andb	$(~(1 << \irq_num)),%al
	outb	%al,$INT_M_CTLMASK

	/* ����ջ����ַ������ת */
	/* ����ж����룬�򷵻�ѹ���restart_reenter */
	/* ���û���ж����룬�򷵻�ѹ���restart */
	ret
.endm



/*
 * ��8259A���ж�
 */
.macro hwint_slave irq_num
	call	save

	inb		$INT_S_CTLMASK,%al
	orb		$(1 << (\irq_num-8)),%al
	outb	%al,$INT_S_CTLMASK		/* ���ε�ǰ�ж� */

	movb	$EOI,%al
	outb	%al,$INT_M_CTL			/* ���´���8259A */
	nop
	outb	%al,$INT_S_CTL			/* ���´򿪴�8259A */

	sti								/* ���ж� */
	pushl	$\irq_num				/* irq */
	call	intr_handle
	popl	%ecx					/* �����������ջ */
	cli								/* ���ж� */

	inb		$INT_M_CTLMASK,%al
	andb	$(~(1 << (\irq_num-8))),%al
	outb	%al,$INT_M_CTLMASK		/* �ָ���Ӧ��ǰ�ж� */

	ret
.endm



/*
 * �쳣����
 */
.macro exception vec_num
	popl	(err_code)
	call	save
	pushl	(err_code)
	pushl	\vec_num
	call	exception_handler
	addl	$8,%esp					/* �����ջ */
	popl	%ecx
	pushl	%eax					/* ѹ��restart�ϵ�(��exception_handler ����) */
	jmp		thread_schedule			/* ���õ��Ⱥ��� */
.endm
