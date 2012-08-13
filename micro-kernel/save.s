/*
 * save.s
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */

.global	save
.global	restart
.global	thread_sleep

.extern ret_addr

.extern thread_schedule
.extern	thread_sleep_handle
.extern	kernel_reenter
.extern kernel_stack

thread_sleep:
	popl	(ret_addr)
	call	thread_sleep_handle


save:
	/* �����������ʼֵΪ-1,���û�����룬���1��Ϊ0 */
	incl	(kernel_reenter)		// if (++kernel_reenter != 0)
	jnz		save_reenter			// 		save_reenter();
									// else
									//		save();
	/* �������ص�ַ */
	popl	(ret_addr)

	/* ��������ͨ�üĴ��� */
	pushal
	/* ����μĴ��� */
	push	%ds
	push	%es
	push	%fs
	push	%gs
	/* ����ջָ�� */
	movl	%esp,(thread_run_stack_top)
	/* ѹ��restart */
	pushl	$restart

	/* �����ں�ջ */
	movl	(kernel_stack),%esp
	/* ���� */
	jmp		*(ret_addr)

save_reenter:
	/* �������ص�ַ */
	popl	(ret_addr)				// hwint__ѹ��
	/* ��������ͨ�üĴ��� */
	pushal							// �ں˲���Ҫ����μĴ���
	/* ѹ��restart_reenter��Ϊ���Ⱥ����ķ��ص�ַ */
	pushl	$restart_reenter
	/* ѹ����Ⱥ��� */
	pushl	$thread_schedule
	/* ���� */
	jmp		*(ret_addr)

restart:
	movl	(thread_run_stack_top),%esp
	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal
	iret
restart_reenter:

