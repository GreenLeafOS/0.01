/*
 * Kernel.cc
 *
 *  Created on: 2012-8-8
 *      Author: greenleaf
 */

#include "include/kernel.h"

int	kernel_reenter;		// 中断嵌入深度。-1表示线程，0表示正常处理。正数表示嵌套深度。
int *kernel_stack_p;	// 内核栈顶保存地址

void KernelMain()
{
	kernel_reenter = -1;
	kernel_stack_p = &tss.esp0;

	__asm("cli");
	while(1);
}





