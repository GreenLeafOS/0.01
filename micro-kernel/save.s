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
	/* 重入计数器初始值为-1,如果没有重入，则加1后为0 */
	incl	(kernel_reenter)		// if (++kernel_reenter != 0)
	jnz		save_reenter			// 		save_reenter();
									// else
									//		save();
	/* 弹出返回地址 */
	popl	(ret_addr)

	/* 保存所有通用寄存器 */
	pushal
	/* 保存段寄存器 */
	push	%ds
	push	%es
	push	%fs
	push	%gs
	/* 保存栈指针 */
	movl	%esp,(thread_run_stack_top)
	/* 压入restart */
	pushl	$restart

	/* 进入内核栈 */
	movl	(kernel_stack),%esp
	/* 返回 */
	jmp		*(ret_addr)

save_reenter:
	/* 弹出返回地址 */
	popl	(ret_addr)				// hwint__压入
	/* 保存所有通用寄存器 */
	pushal							// 内核不需要保存段寄存器
	/* 压入restart_reenter作为调度函数的返回地址 */
	pushl	$restart_reenter
	/* 压入调度函数 */
	pushl	$thread_schedule
	/* 返回 */
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

