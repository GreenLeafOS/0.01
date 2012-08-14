/*
 * save.s
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */

.include 	"include/head.s"

.global	save
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
 * ѹ��restart��Ϊ�жϴ������ķ��ص�ַ
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
	movl	%esp,(thread_run_stack_top)

	/* �����ں�ջ */
	mov		%ss,%ax				# ss����(kernel_stack)TSS��ȡ���ں����ݶ�
	mov		%ax,%ds
	mov		%ax,%es
	mov		%ax,%fs
	mov		%ax,%gs

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
	movl	%eax,(kernel_stack_p)
	movl	%esp,(%eax)

	/* �ָ����� */
	movl	(thread_run_stack_top),%esp
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

