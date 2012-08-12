/*
 * save.s
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */

.global	save
.global	restart

.global	restart_from_exception		/* ¥”“Ï≥£∑µªÿ */

.extern ret_addr

save:
	popl	(ret_addr)
	pushal
	push 	%ds
	push 	%es
	push	%fs
	push 	%gs
	jmp		*(ret_addr)

restart:
	pop		%gs
	pop		%fs
	pop		%es
	pop		%ds
	popal
	iret
