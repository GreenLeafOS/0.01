/*
 * save.s
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */

.include 	"include/head.s"

.global	save
.global thread_sleep
.global	thread_sleep_self
.global	restart
/*
 * �ж��쳣�������ͷ�����Ա���Ĵ���ֵ
 */
save:
	/* �����������ʼֵΪ-1,���û�����룬���1��Ϊ0 */
	incl	(kernel_reenter)		# if (++kernel_reenter != 0)
	jnz		save_reenter			# 		save_reenter();
									# Else
									#		save();
/*
 * save
 * û�з����ж�����ʱ����
 * �����߳�������
 * �����ں�ջ
 * ѹ��restart��Ϊ�жϴ������ķ��ص�ַ reg addr
 */
#save:

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
	movl	(thread_run_stack_top),%eax
	movl	%esp,(%eax)

	/* �����ں�ջ */
	mov		%ss,%ax				# ss����(kernel_stack)TSS��ȡ���ں����ݶ�
	mov		%ax,%ds
	mov		%ax,%es
	mov		%ax,%fs

	mov		$40,%ax
	mov		%ax,%gs

	movl	(kernel_stack_p),%eax
	movl	(%eax),%esp

	/* ѹ��restart */
	pushl	$restart

	/* ���� */
	jmp		*(ret_addr)


/*
 * save_reenter
 * �����ж�����ʱ����
 * �����ں�������
 * ѹ��restart_reenter��Ϊ�жϴ������ķ��ص�ַ
 */
save_reenter:

	/* �������ص�ַ */
	popl	(ret_addr)			# hwint__ѹ��

	/* ��������ͨ�üĴ��� */
	pushal

	/* ѹ��restart_reenter */
	pushl	$restart_reenter

	/* ���� */
	jmp		*(ret_addr)




restart:
	/* �����������һ */
	decl	(kernel_reenter)

	/* �����ں�esp��tss */
	movl	(kernel_stack_p),%eax
	movl	%esp,(%eax)

	/* �ָ����� */
	movl	(thread_run_stack_top),%eax
	movl	(%eax),%esp

	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal

	/* ���� */
	iret

restart_reenter:
	/* �����������һ */
	decl	(kernel_reenter)

	/* �����Ĵ��� */
	popal

	/* ���� */
	iret

thread_sleep_self:
	/* �����������1 */
	incl	(kernel_reenter)

	/* �������ص�ַ */
	popl	(ret_addr)

	/* ����ջ */
	push	%ss
	pushl	%esp
	/* ����eflags */
	pushf
	/* ���ж� */
	cli
	/* �������ָ�� */
	push	%cs
	pushl	(ret_addr)
	/* ����Ĵ��� */
	pushal
	/* ����μĴ��� */
	push	%ds
	push	%es
	push	%fs
	push	%gs
	/* ����ջָ�� */
	movl	(thread_run_stack_top),%eax
	movl	%esp,(%eax)

	/* �����ں�ջ */
	mov		%ss,%ax
	mov		%ax,%ds
	mov		%ax,%es
	mov		%ax,%fs

	mov		$40,%ax
	mov		%ax,%gs

	movl	(kernel_stack_p),%eax
	movl	(%eax),%esp

	/* ѹ����� */
	pushl	(thread_run)
	/* ѹ��restart��Ϊ˯�ߴ������ķ��ص�ַ */
	pushl	$restart

	/* ����˯�ߴ������ */
	jmp		thread_sleep

