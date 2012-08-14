/*
 * excp.s
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */

.include 	"include/head.s"

.global	divide_error
.global	single_step_exception
.global	nmi
.global	breakpoint_exception
.global	overflow
.global	bounds_check
.global	inval_opcode
.global	copr_not_available
.global	double_fault
.global	copr_seg_overrun
.global	inval_tss
.global	segment_not_present
.global	stack_exception
.global	general_protection
.global	page_fault
.global	copr_error


divide_error:
	pushl	$0xFFFFFFFF
	exception 0
single_step_exception:
	pushl	$0xFFFFFFFF
	exception 1
nmi:
	pushl	$0xFFFFFFFF
	exception 2
breakpoint_exception:
	pushl	$0xFFFFFFFF
	exception 3
overflow:
	pushl	$0xFFFFFFFF
	exception 4
bounds_check:
	pushl	$0xFFFFFFFF
	exception 5
inval_opcode:
	pushl	$0xFFFFFFFF
	exception 6
copr_not_available:
	pushl	$0xFFFFFFFF
	exception 7
double_fault:
	exception 8
copr_seg_overrun:
	pushl	$0xFFFFFFFF
	exception 9
inval_tss:
	exception 10
segment_not_present:
	exception 11
stack_exception:
	exception 12
general_protection:
	exception 13
page_fault:
	exception 14
copr_error:
	pushl	$0xFFFFFFFF
	exception 16
