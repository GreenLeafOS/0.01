/*
 * save.s
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */

.include 	"include/head.s"

.global	save
/*
 * 中断异常处理程序开头调用以保存寄存器值
 */
save:
	/* 重入计数器初始值为-1,如果没有重入，则加1后为0 */
	incl	(kernel_reenter)		# if (++kernel_reenter != 0)
	jnz		save_reenter			# 		save_reenter();
									# Else
									#		save();
/*
 * save
 * 没有发生中断重入时调用
 * 保存线程上下文
 * 进入内核栈
 * 压入restart作为中断处理程序的返回地址
 */
#save:

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

	/* 进入内核栈 */
	mov		%ss,%ax				# ss是在(kernel_stack)TSS里取的内核数据段
	mov		%ax,%ds
	mov		%ax,%es
	mov		%ax,%fs
	mov		%ax,%gs

	/* 压入restart */
	pushl	$restart

	/* 返回 */
	jmp		*(ret_addr)


/*
 * save_reenter
 * 发生中断重入时调用
 * 保存内核上下文
 * 压入restart_reenter作为中断处理程序的返回地址
 */
save_reenter:

	/* 弹出返回地址 */
	popl	(ret_addr)			# hwint__压入

	/* 保存所有通用寄存器 */
	pushal

	/* 压入restart_reenter */
	pushl	$restart_reenter

	/* 返回 */
	jmp		*(ret_addr)




restart:
	/* 重入计数器减一 */
	decl	(kernel_reenter)

	/* 保存内核esp到tss */
	movl	%eax,(kernel_stack_p)
	movl	%esp,(%eax)

	/* 恢复运行 */
	movl	(thread_run_stack_top),%esp
	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal

	/* 返回 */
	iret

restart_reenter:
	/* 重入计数器减一 */
	decl	(kernel_reenter)

	/* 弹出寄存器 */
	popal

	/* 返回 */
	iret

