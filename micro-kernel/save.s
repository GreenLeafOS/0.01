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



thread_sleep:
	popl	(ret_addr)
	call	thread_sleep_handle
save:
	popl	(ret_addr)
	pushal
	push 	%ds
	push 	%es
	push	%fs
	push 	%gs
	movl	%esp,(thread_run_stack_top)
	jmp		*(ret_addr)

restart:
	movl	(thread_run_stack_top),%esp
	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal
	iret
